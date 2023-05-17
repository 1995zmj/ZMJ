#include "SGraphNodeZ_TextBlockNode.h"

#include "GraphEditorSettings.h"
#include "SCommentBubble.h"
#include "Graph/Pins/SGraphPin_StatePin.h"
#include "Utilities/SMBlueprintEditorUtils.h"

void SGraphNodeZ_TextBlockNode::Construct(const FArguments& InArgs, USMGraphNodeZ_TextBlock* InNode)
{
	// SGraphNode_StateNode::Construct(InArgs, InNode);
}

void SGraphNodeZ_TextBlockNode::UpdateGraphNode()
{
	// InputPins.Empty();
	// OutputPins.Empty();
	//
	// // Reset variables that are going to be exposed, in case we are refreshing an already setup node.
	// RightNodeBox.Reset();
	// LeftNodeBox.Reset();
	//
	// const FLinearColor TitleShadowColor(0.6f, 0.6f, 0.6f);
	//
	// const float PinPadding = FSMBlueprintEditorUtils::GetEditorSettings()->StateConnectionSize;
	//
	// SetupErrorReporting();
	// TSharedPtr<SErrorText> ErrorText;
	// const TSharedPtr<SWidget> ContentBox = CreateContentBox();
	// const TAttribute<const FSlateBrush*> SelectedBrush = TAttribute<const FSlateBrush*>::Create(TAttribute<const FSlateBrush*>::FGetter::CreateSP(this, &SGraphNode_StateNode::GetNameIcon));
	//
	// this->ContentScale.Bind(this, &SGraphNode::GetContentScale);
	// this->GetOrAddSlot(ENodeZone::Center)
	// 	.HAlign(HAlign_Center)
	// 	.VAlign(VAlign_Center)
	// 	[
	// 		SNew(SBorder)
	// 		.BorderImage(FEditorStyle::GetBrush("Graph.StateNode.Body"))
	// 		.Padding(0)
	// 		.BorderBackgroundColor(this, &SGraphNodeZ_TextBlockNode::GetBorderBackgroundColor)
	// 		[
	// 			SNew(SOverlay)
	//
	// 			// PIN AREA
	// 			+ SOverlay::Slot()
	// 			.HAlign(HAlign_Fill)
	// 			.VAlign(VAlign_Fill)
	// 			[
	// 				// SAssignNew(RightNodeBox, SVerticalBox)
	// 				SAssignNew(LeftNodeBox,SVerticalBox)
	// 			]
	//
	// 			// STATE NAME AREA
	// 			+ SOverlay::Slot()
	// 			.HAlign(HAlign_Center)
	// 			.VAlign(VAlign_Center)
	// 			.Padding(PinPadding)
	// 			[
	// 				SNew(SBorder)
	// 				.BorderImage(FEditorStyle::GetBrush("Graph.StateNode.ColorSpill"))
	// 				.BorderBackgroundColor(TitleShadowColor)
	// 				.HAlign(HAlign_Center)
	// 				.VAlign(VAlign_Center)
	// 				.Visibility(EVisibility::SelfHitTestInvisible)
	// 				[
	// 					SNew(SVerticalBox)
	// 					+ SVerticalBox::Slot()
	// 					.AutoHeight()
	// 					[
	// 						SNew(SHorizontalBox)
	// 						+ SHorizontalBox::Slot()
	// 						.AutoWidth()
	// 						[
	// 							// POPUP ERROR MESSAGE
	// 							SAssignNew(ErrorText, SErrorText)
	// 							.BackgroundColor(this, &SGraphNodeZ_TextBlockNode::GetErrorColor)
	// 							.ToolTipText(this, &SGraphNodeZ_TextBlockNode::GetErrorMsgToolTip)
	// 						]
	// 						+ SHorizontalBox::Slot()
	//
	// 							.AutoWidth()
	// 							.VAlign(VAlign_Center)
	// 							[
	// 								SNew(SImage)
	// 								.Image(SelectedBrush)
	// 							]
	// 						+ SHorizontalBox::Slot()
	// 						.Padding(ContentPadding)
	// 						[
	// 							ContentBox.ToSharedRef()
	// 						]
	// 					]
	// 					+ SVerticalBox::Slot()
	// 					.Padding(ContentPadding)
	// 					[
	// 						SAssignNew(RightNodeBox,SVerticalBox)
	// 					]
	// 				]
	// 			]
	// 		]
	// 	];
	//
	// // Create comment bubble
	// TSharedPtr<SCommentBubble> CommentBubble;
	// const FSlateColor CommentColor = GetDefault<UGraphEditorSettings>()->DefaultCommentNodeTitleColor;
	//
	// SAssignNew(CommentBubble, SCommentBubble)
	// 	.GraphNode(GraphNode)
	// 	.Text(this, &SGraphNode::GetNodeComment)
	// 	.OnTextCommitted(this, &SGraphNode::OnCommentTextCommitted)
	// 	.ColorAndOpacity(CommentColor)
	// 	.AllowPinning(true)
	// 	.EnableTitleBarBubble(true)
	// 	.EnableBubbleCtrls(true)
	// 	.GraphLOD(this, &SGraphNode::GetCurrentLOD)
	// 	.IsGraphNodeHovered(this, &SGraphNode::IsHovered);
	//
	// GetOrAddSlot(ENodeZone::TopCenter)
	// 	.SlotOffset(TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetOffset))
	// 	.SlotSize(TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetSize))
	// 	.AllowScaling(TAttribute<bool>(CommentBubble.Get(), &SCommentBubble::IsScalingAllowed))
	// 	.VAlign(VAlign_Top)
	// 	[
	// 		CommentBubble.ToSharedRef()
	// 	];
	//
	// ErrorReporting = ErrorText;
	// ErrorReporting->SetError(ErrorMsg);
	// CreatePinWidgets();
	//
	// CalculateAnyStateImpact();
}

void SGraphNodeZ_TextBlockNode::CreatePinWidgets()
{
	// SGraphNode_StateNode::CreatePinWidgets();

	USMGraphNodeZ_TextBlock* StateNode = CastChecked<USMGraphNodeZ_TextBlock>(GraphNode);

	UEdGraphPin* CurPin = StateNode->GetInputPin();
	if (!CurPin->bHidden)
	{
		TSharedPtr<SGraphPin> NewPin = SNew(SSMGraphPin_StatePin, CurPin);

		this->AddPin(NewPin.ToSharedRef());
	}
}

void SGraphNodeZ_TextBlockNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));
	if (PinToAdd->GetDirection() == EEdGraphPinDirection::EGPD_Input)
	{
		LeftNodeBox->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.FillHeight(1.0f)
			.Padding(Settings->GetInputPinPadding())
		[
			PinToAdd
		];
		InputPins.Add(PinToAdd);
	}
	else // Direction == EEdGraphPinDirection::EGPD_Output
	{
		RightNodeBox->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Padding(Settings->GetOutputPinPadding())
		[
			PinToAdd
		];
		OutputPins.Add(PinToAdd);
	}
}

