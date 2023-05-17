// Fill out your copyright notice in the Description page of Project Settings.


#include "SMGraphBlockSchema.h"

#include "GraphEditorActions.h"
#include "ToolMenu.h"
#include "Commands/SMEditorCommands.h"
#include "Graph/SMGraph.h"
#include "Graph/ConnectionDrawing/SMGraphK2ConnectionDrawingPolicy.h"
#include "Graph/Nodes/SMGraphNodeZ_EndBlock.h"
#include "Graph/Nodes/SMGraphNodeZ_EnterBlock.h"
#include "Graph/Nodes/SMGraphNodeZ_TextBlock.h"
#include "Graph/Nodes/SMGraphNodeZ_OptionBlock.h"

#define LOCTEXT_NAMESPACE "SMGraphBlockSchema"

const FName USMGraphBlockSchema::PC_Option(TEXT("Option"));

void USMGraphBlockSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	// Super::CreateDefaultNodesForGraph(Graph);

	// Create the result node
	FGraphNodeCreator<USMGraphNodeZ_EnterBlock> EnterNodeCreator(Graph);
	USMGraphNodeZ_EnterBlock* EntryNode = EnterNodeCreator.CreateNode();
	EnterNodeCreator.Finalize();
	SetNodeMetaData(EntryNode, FNodeMetadata::DefaultGraphNode);

	FGraphNodeCreator<USMGraphNodeZ_EndBlock> EndNodeCreator(Graph);
	USMGraphNodeZ_EndBlock* EndNode = EndNodeCreator.CreateNode();
	EndNodeCreator.Finalize();
	EndNode->NodePosX = 500;
	SetNodeMetaData(EndNode, FNodeMetadata::DefaultGraphNode);

	// if (USMGraph* StateMachineGraph = CastChecked<USMGraph>(&Graph))
	// {
	// 	StateMachineGraph->EntryNode = EntryNode;
	// }
}



void USMGraphBlockSchema::GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	Super::GetContextMenuActions(Menu, Context);
	
	const UEdGraph* CurrentGraph = Context->Graph;
	const UEdGraphNode* InGraphNode = Context->Node;
	const UEdGraphPin* InGraphPin = Context->Pin;

	FToolMenuSection& GraphSection = Menu->AddSection("SMGraphBlockSchemaGraphActions", LOCTEXT("GraphActionsMenuHeader", "Block Actions"));
	{
		GraphSection.AddMenuEntry(FSMEditorCommands::Get().AddOption);
		GraphSection.AddMenuEntry(FSMEditorCommands::Get().RemoveOption);
	}
 }

void USMGraphBlockSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	// Super::GetGraphContextActions(ContextMenuBuilder);
	const int32 BaseGrouping = 1;
	const int32 UserGrouping = 0;
	
	FText CategoryBlock = LOCTEXT("Block", "Block");
	{
		TSharedPtr<FSMGraphSchemaAction_NewNode> NewNodeAction(new FSMGraphSchemaAction_NewNode(CategoryBlock,
			LOCTEXT("AddTextBlock", "Add Text Block..."),
			LOCTEXT("AddBlockTooltip", "A new Block which contains entry points for logic execution."),
			BaseGrouping));
		ContextMenuBuilder.AddAction(NewNodeAction);
		NewNodeAction->NodeTemplate = NewObject<USMGraphNodeZ_TextBlock>(ContextMenuBuilder.OwnerOfTemporaries);
	}

	{
		TSharedPtr<FSMGraphSchemaAction_NewNode> NewNodeAction(new FSMGraphSchemaAction_NewNode(CategoryBlock,
			LOCTEXT("AddOptionBlock", "Add Option Block..."),
			LOCTEXT("AddBlockTooltip", "A new Block which contains entry points for logic execution."),
			BaseGrouping));
		ContextMenuBuilder.AddAction(NewNodeAction);
		NewNodeAction->NodeTemplate = NewObject<USMGraphNodeZ_OptionBlock>(ContextMenuBuilder.OwnerOfTemporaries);
	}
	
	// {
	// 	TSharedPtr<FSMGraphSchemaAction_NewNode> NewNodeAction = AddNewStateNodeAction<FSMGraphSchemaAction_NewNode>(ContextMenuBuilder, CategoryBlock, LOCTEXT("AddOptionBlock", "Add Option Block..."), LOCTEXT("AddBlockTooltip", "A new Block which contains entry points for logic execution."), BaseGrouping);
	// 	NewNodeAction->NodeTemplate = NewObject<USMGraphNodeZ_OptionBlock>(ContextMenuBuilder.OwnerOfTemporaries);
	// }
}

const FPinConnectionResponse USMGraphBlockSchema::CanCreateConnection(const UEdGraphPin* PinA,
	const UEdGraphPin* PinB) const
{
	// return Super::CanCreateConnection(PinA, PinB);
	// PinA->Direction == EEdGraphPinDirection

	if (PinA->Direction == EGPD_Output && PinB->Direction == EGPD_Input)
	{
		if (PinA->PinType.PinCategory == PinB->PinType.PinCategory)
		{
			return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_A, TEXT(""));
		}
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinErrorNotStateNode", "In Out Error"));
	}
	return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinErrorNotStateNode", "In Out Error"));
}

FConnectionDrawingPolicy* USMGraphBlockSchema::CreateConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID,
                                                                             float InZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements,
                                                                             UEdGraph* InGraphObj) const
{
	return new FSMGraphK2ConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, InZoomFactor, InClippingRect, InDrawElements, InGraphObj);
}
#undef LOCTEXT_NAMESPACE