// Copyright Recursoft LLC 2019-2022. All Rights Reserved.

#include "SGraphNode_StateNode.h"
#include "Graph/Nodes/SMGraphNode_StateNode.h"
#include "Graph/Nodes/SMGraphNode_ConduitNode.h"
#include "Graph/Pins/SGraphPin_StatePin.h"
#include "Graph/Nodes/SMGraphNode_StateMachineStateNode.h"
#include "Graph/Nodes/SMGraphNode_StateMachineParentNode.h"
#include "Graph/Nodes/SMGraphNode_AnyStateNode.h"
#include "Utilities/SMBlueprintEditorUtils.h"
#include "Utilities/SMNodeInstanceUtils.h"
#include "Configuration/SMEditorStyle.h"

#include "SMConduit.h"

#include "SLevelOfDetailBranchNode.h"
#include "SCommentBubble.h"
#include "SlateOptMacros.h"
#include "SGraphPin.h"
#include "Components/VerticalBox.h"
#include "SGraphPreviewer.h"
#include "GraphEditorSettings.h"
#include "Templates/SharedPointer.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/SToolTip.h"
#include "Widgets/SWidget.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"
#include "Kismet2/KismetEditorUtilities.h"

#define LOCTEXT_NAMESPACE "SGraphStateNode"

void SGraphNode_StateNode::Construct(const FArguments& InArgs, USMGraphNode_StateNodeBase* InNode)
{
	SGraphNode_BaseNode::Construct(SGraphNode_BaseNode::FArguments(), InNode);
	ContentPadding = InArgs._ContentPadding;
	CastChecked<USMGraphNode_Base>(GraphNode)->OnWidgetConstruct();
	
	UpdateGraphNode();
	SetCursor(EMouseCursor::CardinalCross);

	const USMEditorSettings* EditorSettings = FSMBlueprintEditorUtils::GetEditorSettings();
	{
		const FSlateBrush* FastPathImageBrush = FSMEditorStyle::Get()->GetBrush(TEXT("SMGraph.FastPath"));
		
		FastPathWidget =
			SNew(SImage)
			.Image(FastPathImageBrush)
			.ToolTipText(NSLOCTEXT("StateNode", "StateNodeFastPathTooltip", "Fast path enabled: All execution points avoid going through the blueprint graph."))
			.Visibility(EVisibility::Visible);
	}
}

void SGraphNode_StateNode::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SGraphNode::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	USMGraphNode_StateNodeBase* StateNode = CastChecked<USMGraphNode_StateNodeBase>(GraphNode);
	StateNode->UpdateTime(InDeltaTime);
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SGraphNode_StateNode::UpdateGraphNode()
{
	InputPins.Empty();
	OutputPins.Empty();

	// Reset variables that are going to be exposed, in case we are refreshing an already setup node.
	RightNodeBox.Reset();
	LeftNodeBox.Reset();

	const FLinearColor TitleShadowColor(0.6f, 0.6f, 0.6f);

	const float PinPadding = FSMBlueprintEditorUtils::GetEditorSettings()->StateConnectionSize;
	
	SetupErrorReporting();
	TSharedPtr<SErrorText> ErrorText;
	const TSharedPtr<SWidget> ContentBox = CreateContentBox();
	const TAttribute<const FSlateBrush*> SelectedBrush = TAttribute<const FSlateBrush*>::Create(TAttribute<const FSlateBrush*>::FGetter::CreateSP(this, &SGraphNode_StateNode::GetNameIcon));
	
	this->ContentScale.Bind(this, &SGraphNode::GetContentScale);
	this->GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SBorder)
			.BorderImage(FEditorStyle::GetBrush("Graph.StateNode.Body"))
			.Padding(0)
			.BorderBackgroundColor(this, &SGraphNode_StateNode::GetBorderBackgroundColor)
			[
				SNew(SOverlay)

				// PIN AREA
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SAssignNew(RightNodeBox, SVerticalBox)
				]

				// STATE NAME AREA
				+ SOverlay::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.Padding(PinPadding)
				[
					SNew(SBorder)
					.BorderImage(FEditorStyle::GetBrush("Graph.StateNode.ColorSpill"))
					.BorderBackgroundColor(TitleShadowColor)
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.Visibility(EVisibility::SelfHitTestInvisible)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							// POPUP ERROR MESSAGE
							SAssignNew(ErrorText, SErrorText)
							.BackgroundColor(this, &SGraphNode_StateNode::GetErrorColor)
							.ToolTipText(this, &SGraphNode_StateNode::GetErrorMsgToolTip)
						]
						+ SHorizontalBox::Slot()

							.AutoWidth()
							.VAlign(VAlign_Center)
							[
								SNew(SImage)
								.Image(SelectedBrush)
							]
						+ SHorizontalBox::Slot()
						.Padding(ContentPadding)
						[
							ContentBox.ToSharedRef()
						]
					]
				]
			]
		];

	// Create comment bubble
	TSharedPtr<SCommentBubble> CommentBubble;
	const FSlateColor CommentColor = GetDefault<UGraphEditorSettings>()->DefaultCommentNodeTitleColor;

	SAssignNew(CommentBubble, SCommentBubble)
		.GraphNode(GraphNode)
		.Text(this, &SGraphNode::GetNodeComment)
		.OnTextCommitted(this, &SGraphNode::OnCommentTextCommitted)
		.ColorAndOpacity(CommentColor)
		.AllowPinning(true)
		.EnableTitleBarBubble(true)
		.EnableBubbleCtrls(true)
		.GraphLOD(this, &SGraphNode::GetCurrentLOD)
		.IsGraphNodeHovered(this, &SGraphNode::IsHovered);

	GetOrAddSlot(ENodeZone::TopCenter)
		.SlotOffset(TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetOffset))
		.SlotSize(TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetSize))
		.AllowScaling(TAttribute<bool>(CommentBubble.Get(), &SCommentBubble::IsScalingAllowed))
		.VAlign(VAlign_Top)
		[
			CommentBubble.ToSharedRef()
		];
	
	ErrorReporting = ErrorText;
	ErrorReporting->SetError(ErrorMsg);
	CreatePinWidgets();

	CalculateAnyStateImpact();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SGraphNode_StateNode::CreatePinWidgets()
{
	USMGraphNode_StateNodeBase* StateNode = CastChecked<USMGraphNode_StateNodeBase>(GraphNode);

	UEdGraphPin* CurPin = StateNode->GetOutputPin();
	if (!CurPin->bHidden)
	{
		TSharedPtr<SGraphPin> NewPin = SNew(SSMGraphPin_StatePin, CurPin);

		this->AddPin(NewPin.ToSharedRef());
	}
}

void SGraphNode_StateNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));
	RightNodeBox->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.FillHeight(1.0f)
		[
			PinToAdd
		];
	OutputPins.Add(PinToAdd);
}

TSharedPtr<SToolTip> SGraphNode_StateNode::GetComplexTooltip()
{
	/* Display a pop-up on mouse hover with useful information. */
	const TSharedPtr<SVerticalBox> Widget = BuildComplexTooltip();

	return SNew(SToolTip)
		[
			Widget.ToSharedRef()
		];
}

TArray<FOverlayWidgetInfo> SGraphNode_StateNode::GetOverlayWidgets(bool bSelected, const FVector2D& WidgetSize) const
{
	TArray<FOverlayWidgetInfo> Widgets;

	const USMEditorSettings* EditorSettings = FSMBlueprintEditorUtils::GetEditorSettings();
	if (!EditorSettings->bDisableVisualCues)
	{
		if (const USMGraphNode_StateNodeBase* StateNode = Cast<USMGraphNode_StateNodeBase>(GraphNode))
		{
			const FSlateBrush* AnyStateImageBrush = FSMEditorStyle::Get()->GetBrush(TEXT("SMGraph.AnyState"));
			for (const TSharedPtr<SWidget>& AnyStateWidget : AnyStateImpactWidgets)
			{
				FOverlayWidgetInfo Info;
				Info.OverlayOffset = FVector2D(WidgetSize.X - (AnyStateImageBrush->ImageSize.X * 0.5f) - (Widgets.Num() * OverlayWidgetPadding),
					-(AnyStateImageBrush->ImageSize.Y * 0.5f));
				Info.Widget = AnyStateWidget;

				Widgets.Add(Info);
			}
			if (EditorSettings->bDisplayFastPath && StateNode->IsNodeFastPathEnabled())
			{
				const FSlateBrush* FastPathImageBrush = FSMEditorStyle::Get()->GetBrush(TEXT("SMGraph.FastPath"));

				FOverlayWidgetInfo Info;
				Info.OverlayOffset = FVector2D(WidgetSize.X - (FastPathImageBrush->ImageSize.X * 0.5f) - (Widgets.Num() * OverlayWidgetPadding),
					-(FastPathImageBrush->ImageSize.Y * 0.5f));
				Info.Widget = FastPathWidget;

				Widgets.Add(Info);
			}
		}
	}

	return Widgets;
}

FReply SGraphNode_StateNode::OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	return SGraphNode::OnMouseButtonDoubleClick(InMyGeometry, InMouseEvent);
}

void SGraphNode_StateNode::RequestRenameOnSpawn()
{
	SGraphNode::RequestRenameOnSpawn();
}

FReply SGraphNode_StateNode::OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	return FReply::Handled();
}

TSharedPtr<SVerticalBox> SGraphNode_StateNode::BuildComplexTooltip()
{
	USMGraphNode_StateNodeBase* StateNode = CastChecked<USMGraphNode_StateNodeBase>(GraphNode);

	const bool bCanExecute = StateNode->HasInputConnections();
	const bool bIsEndState = StateNode->IsEndState(false);
	bool bIsAnyState = false;
	
	FString NodeType = "State";
	if (StateNode->IsA<USMGraphNode_StateMachineParentNode>())
	{
		NodeType = "Parent";
	}
	else if (const USMGraphNode_StateMachineStateNode* StateMachineNode = Cast<USMGraphNode_StateMachineStateNode>(StateNode))
	{
		NodeType = StateMachineNode->IsStateMachineReference() ? "State Machine Reference" : "State Machine";
	}
	else if (USMGraphNode_AnyStateNode* AnyStateNode = Cast<USMGraphNode_AnyStateNode>(StateNode))
	{
		NodeType = "Any State";
		bIsAnyState = true;
	}

	const bool bAnyStateImpactsThisNode = !bIsAnyState && FSMBlueprintEditorUtils::IsNodeImpactedFromAnyStateNode(StateNode);
	
	const FSlateBrush* FastPathImageBrush = FSMEditorStyle::Get()->GetBrush(TEXT("SMGraph.FastPath_32x"));
	
	TSharedPtr<SVerticalBox> Widget = SNew(SVerticalBox);
	Widget->AddSlot()
		.AutoHeight()
		.Padding(FMargin(0.f, 0.f, 0.f, 4.f))
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(STextBlock)
				.TextStyle(FSMEditorStyle::Get(), "SMGraph.Tooltip.Title")
				.Text(FText::Format(LOCTEXT("StatePopupTitle", "{0} ({1})"), FText::FromString(StateNode->GetStateName()), FText::FromString(NodeType)))
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(0.f, -4.f, 0.f, 0.f)
			[
				SNew(SImage)
				.Image(FastPathImageBrush)
				.Visibility_Lambda([StateNode]()
				{
					return StateNode && StateNode->IsNodeFastPathEnabled() ? EVisibility::Visible : EVisibility::Collapsed;
				})
			]
		];
	
	if (UEdGraph* Graph = GetGraphToUseForTooltip())
	{
		Widget->AddSlot()
			.AutoHeight()
			[
				SNew(SGraphPreviewer, Graph)
				.ShowGraphStateOverlay(false)
			];
	}
	if (!bCanExecute && !bIsAnyState)
	{
		Widget->AddSlot()
			.AutoHeight()
			.Padding(FMargin(2.f, 4.f, 2.f, 2.f))
			[
				SNew(STextBlock)
				.TextStyle(FSMEditorStyle::Get(), "SMGraph.Tooltip.Warning")
				.Text(LOCTEXT("StateCantExecuteTooltip", "No Valid Input: State will never execute"))
			];
	}

	if (bIsEndState)
	{
		const FText EndStateTooltip = StateNode->IsEndState(true) ? LOCTEXT("EndStateTooltip", "End State: State will never exit") :
			LOCTEXT("NotEndStateTooltip", "Not an End State: An Any State node is adding transitions to this node");
		
		Widget->AddSlot()
			.AutoHeight()
			.Padding(FMargin(2.f, 4.f, 2.f, 2.f))
			[
				SNew(STextBlock)
				.TextStyle(FSMEditorStyle::Get(), "SMGraph.Tooltip.Info")
				.Text(EndStateTooltip)
			];
	}
	else if (bAnyStateImpactsThisNode)
	{
		Widget->AddSlot()
			.AutoHeight()
			.Padding(FMargin(2.f, 4.f, 2.f, 2.f))
			[
				SNew(STextBlock)
				.TextStyle(FSMEditorStyle::Get(), "SMGraph.Tooltip.Info")
				.Text(LOCTEXT("AnyStateImpactTooltip", "An Any State node is adding transitions to this node"))
			];
	}

	return Widget;
}

UEdGraph* SGraphNode_StateNode::GetGraphToUseForTooltip() const
{
	const USMGraphNode_StateNodeBase* StateNode = CastChecked<USMGraphNode_StateNodeBase>(GraphNode);
	return StateNode->GetBoundGraph();
}

void SGraphNode_StateNode::CalculateAnyStateImpact()
{
	DECLARE_SCOPE_CYCLE_COUNTER(TEXT("SGraphNode_StateNode::CalculateAnyStateImpact"), STAT_CalculateAnyStateImpact, STATGROUP_LogicDriverEditor);

	AnyStateImpactWidgets.Reset();
	
	const USMGraphNode_StateNodeBase* StateNode = CastChecked<USMGraphNode_StateNodeBase>(GraphNode);
	TArray<USMGraphNode_AnyStateNode*> AnyStates;

	const USMEditorSettings* EditorSettings = FSMBlueprintEditorUtils::GetEditorSettings();
	if (EditorSettings->MaxAnyStateIcons > 0 && FSMBlueprintEditorUtils::IsNodeImpactedFromAnyStateNode(StateNode, &AnyStates))
	{
		// Sort first so similar colors are grouped. Luminance seems to provide quickest and best results.
		AnyStates.Sort([](const USMGraphNode_AnyStateNode& AnyStateA, const USMGraphNode_AnyStateNode& AnyStateB)
		{
			return AnyStateA.GetAnyStateColor().GetLuminance() >= AnyStateB.GetAnyStateColor().GetLuminance();
		});
		
		int32 ColorsOverLimit = 1;
		for (int32 AnyStateIdx = 0; AnyStateIdx < AnyStates.Num(); ++AnyStateIdx)
		{
			const USMGraphNode_AnyStateNode* AnyState = AnyStates[AnyStateIdx];
			const bool bIsGrouped = AnyStateIdx >= EditorSettings->MaxAnyStateIcons;
			const bool bIsLastIteration = AnyStateIdx == AnyStates.Num() - 1;
			
			FLinearColor AnyStateColor = AnyState->GetAnyStateColor();
			
			if (bIsGrouped)
			{
				ColorsOverLimit++;
				if (!bIsLastIteration)
				{
					// Skip until end.
					continue;
				}
			}
			
			FText TooltipText;

			if (bIsGrouped)
			{
				// Replace the last one with the grouped widget.
				AnyStateImpactWidgets.RemoveAt(0);
				
				TooltipText = FText::FromString(FString::Printf(TEXT("An additional %s Any State nodes are adding transitions to this node."),
					*FString::FromInt(ColorsOverLimit)));

				AnyStateColor = FLinearColor::White;
			}
			else
			{
				// Display individual any state.
				TooltipText = FText::FromString(FString::Printf(TEXT("The Any State node '%s' is adding one or more transitions to this state."),
					*AnyState->GetStateName()));
			}

			AnyStateColor.A = 0.72f;
			
			const FSlateBrush* ImageBrush = FSMEditorStyle::Get()->GetBrush(TEXT("SMGraph.AnyState"));
			TSharedPtr<SWidget> Widget =
				SNew(SBorder)
				.BorderImage(FEditorStyle::GetBrush("NoBorder"))
				.Cursor(bIsGrouped ? EMouseCursor::Default : EMouseCursor::Hand)
				.Padding(0.f)
				.VAlign(VAlign_Center)
				.OnMouseDoubleClick_Lambda([AnyState, bIsGrouped](const FGeometry&, const FPointerEvent&)
				{
					if (!bIsGrouped && AnyState)
					{
						FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(AnyState);
					}
					return FReply::Handled();
				})
				[
					SNew(SOverlay)
					+ SOverlay::Slot()
					.VAlign(VAlign_Center)
					[
						SNew(SImage)
						.Image(ImageBrush)
						.ToolTipText(TooltipText)
						.ColorAndOpacity(AnyStateColor)
						.Visibility(EVisibility::Visible)
					]
					+ SOverlay::Slot()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Center)
					[
						SNew(STextBlock)
						.Visibility(bIsGrouped ? EVisibility::HitTestInvisible : EVisibility::Collapsed)
						.Text(FText::FromString(FString::FromInt(ColorsOverLimit)))
						.Font(FCoreStyle::GetDefaultFontStyle("Regular", 8))
						.ColorAndOpacity(FLinearColor::Black)
					]
				];

			AnyStateImpactWidgets.Insert(Widget, 0);
		}
	}
}

void SGraphNode_StateNode::GetNodeInfoPopups(FNodeInfoContext* Context,
                                             TArray<FGraphInformationPopupInfo>& Popups) const
{
	const USMGraphNode_StateNodeBase* Node = CastChecked<USMGraphNode_StateNodeBase>(GraphNode);
	if (const FSMNode_Base* DebugNode = Node->GetDebugNode())
	{
		// Show active time or last active time over the node.

		if (Node->IsDebugNodeActive())
		{
			const FString StateText = FString::Printf(TEXT("Active for %.2f secs"), DebugNode->TimeInState);
			new (Popups) FGraphInformationPopupInfo(nullptr, Node->GetBackgroundColor(), StateText);
		}
		else if (Node->WasDebugNodeActive())
		{
			
			const USMEditorSettings* EditorSettings = FSMBlueprintEditorUtils::GetEditorSettings();

			const float StartFade = EditorSettings->TimeToDisplayLastActiveState;
			const float TimeToFade = EditorSettings->TimeToFadeLastActiveState;
			const float DebugTime = Node->GetDebugTime();

			if (DebugTime < StartFade + TimeToFade)
			{
				const FString StateText = FString::Printf(TEXT("Was Active for %.2f secs"), DebugNode->TimeInState);

				if (DebugTime > StartFade)
				{
					FLinearColor Color = Node->GetBackgroundColor();

					const float PercentComplete = TimeToFade <= 0.f ? 0.f : FMath::Clamp(Color.A * (1.f - (DebugTime - StartFade) / TimeToFade), 0.f, Color.A);
					Color.A *= PercentComplete;

					const FLinearColor ResultColor = Color;
					new (Popups) FGraphInformationPopupInfo(nullptr, ResultColor, StateText);
				}
				else
				{
					new (Popups) FGraphInformationPopupInfo(nullptr, Node->GetBackgroundColor(), StateText);
				}
			}
		}
	}
}

void SGraphNode_StateNode::OnRefreshRequested(USMGraphNode_Base* InNode, bool bFullRefresh)
{
	CalculateAnyStateImpact();
	// Full refresh
	SGraphNode_BaseNode::OnRefreshRequested(InNode, bFullRefresh);
}

TSharedPtr<SWidget> SGraphNode_StateNode::CreateContentBox()
{
	TSharedPtr<SVerticalBox> Content;
	TSharedPtr<SNodeTitle> NodeTitle = SNew(SNodeTitle, GraphNode);
	SAssignNew(Content, SVerticalBox);

	bool bDisplayTitle = true;

	Content->AddSlot()
		.AutoHeight()
		[
			SAssignNew(InlineEditableText, SInlineEditableTextBlock)
			.Style(FEditorStyle::Get(), "Graph.StateNode.NodeTitleInlineEditableText")
			.Text(NodeTitle.Get(), &SNodeTitle::GetHeadTitle)
			.OnVerifyTextChanged(this, &SGraphNode_StateNode::OnVerifyNameTextChanged)
			.OnTextCommitted(this, &SGraphNode_StateNode::OnNameTextCommited)
			.IsReadOnly(this, &SGraphNode_StateNode::IsNameReadOnly)
			.IsSelected(this, &SGraphNode_StateNode::IsSelectedExclusively)
			.Visibility(bDisplayTitle ? EVisibility::Visible : EVisibility::Collapsed)
		];
	Content->AddSlot()
		.AutoHeight()
		[
			NodeTitle.ToSharedRef()
		];
	
	// Add custom property widgets sorted by user specification.
	USMGraphNode_StateNodeBase* StateNodeBase = CastChecked<USMGraphNode_StateNodeBase>(GraphNode);

	// Populate all used state classes, in order.
	TArray<USMNodeInstance*> NodeTemplates;

	auto IsValidClass = [](USMGraphNode_Base* Node, UClass* NodeClass) {return NodeClass && NodeClass != Node->GetDefaultNodeClass(); };

	if (IsValidClass(StateNodeBase, StateNodeBase->GetNodeClass()))
	{
		NodeTemplates.Add(StateNodeBase->GetNodeTemplate());
	}

	return Content;
}

FSlateColor SGraphNode_StateNode::GetBorderBackgroundColor() const
{
	USMGraphNode_StateNodeBase* StateNode = CastChecked<USMGraphNode_StateNodeBase>(GraphNode);
	return StateNode->GetBackgroundColor();
}

const FSlateBrush* SGraphNode_StateNode::GetNameIcon() const
{
	USMGraphNode_StateNodeBase* StateNode = CastChecked<USMGraphNode_StateNodeBase>(GraphNode);
	if (const FSlateBrush* Brush = StateNode->GetNodeIcon())
	{
		return Brush;
	}
	
	return FEditorStyle::GetBrush(TEXT("Graph.StateNode.Icon"));
}


void SGraphNode_ConduitNode::Construct(const FArguments& InArgs, USMGraphNode_ConduitNode* InNode)
{
	const USMEditorSettings* EditorSettings = FSMBlueprintEditorUtils::GetEditorSettings();

	SGraphNode_StateNode::FArguments Args;
	Args.ContentPadding(EditorSettings->StateContentPadding);
	
	SGraphNode_StateNode::Construct(Args, InNode);
}

void SGraphNode_ConduitNode::GetNodeInfoPopups(FNodeInfoContext* Context, TArray<FGraphInformationPopupInfo>& Popups) const
{
	USMGraphNode_ConduitNode* Node = CastChecked<USMGraphNode_ConduitNode>(GraphNode);
	if (const FSMConduit* DebugNode = (FSMConduit*)Node->GetDebugNode())
	{
		if (Node->ShouldEvalWithTransitions() && Node->WasEvaluating())
		{
			// Transition evaluation, don't show active information.
			return;
		}
	}
	
	SGraphNode_StateNode::GetNodeInfoPopups(Context, Popups);
}

const FSlateBrush* SGraphNode_ConduitNode::GetNameIcon() const
{
	USMGraphNode_StateNodeBase* StateNode = CastChecked<USMGraphNode_StateNodeBase>(GraphNode);
	if (const FSlateBrush* Brush = StateNode->GetNodeIcon())
	{
		return Brush;
	}
	
	return FEditorStyle::GetBrush(TEXT("Graph.ConduitNode.Icon"));
}

#undef LOCTEXT_NAMESPACE