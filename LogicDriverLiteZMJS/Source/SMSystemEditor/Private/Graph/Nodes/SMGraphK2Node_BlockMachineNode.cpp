// Fill out your copyright notice in the Description page of Project Settings.


#include "SMGraphK2Node_BlockMachineNode.h"

#include "Graph/SMGraph.h"
#include "Graph/Schema/SMGraphBlockSchema.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/Kismet2NameValidators.h"


#define LOCTEXT_NAMESPACE "SMGraphK2BlockMachineNode"
void USMGraphK2Node_BlockMachineNode::PostPlacedNewNode()
{
	// Super::PostPlacedNewNode();
	// Create a new state machine graph
	check(BoundGraph == NULL);
	BoundGraph = Cast<USMGraph>(FBlueprintEditorUtils::CreateNewGraph(
		this,
		NAME_None,
		USMGraph::StaticClass(),
		USMGraphBlockSchema::StaticClass()));
	check(BoundGraph);

	// Find an interesting name
	TSharedPtr<INameValidatorInterface> NameValidator = FNameValidatorFactory::MakeValidator(this);
	FBlueprintEditorUtils::RenameGraphWithSuggestion(BoundGraph, NameValidator, TEXT("Block Machine"));

	// Initialize the state machine graph
	const UEdGraphSchema* Schema = BoundGraph->GetSchema();
	Schema->CreateDefaultNodesForGraph(*BoundGraph);

	// Add the new graph as a child of our parent graph
	UEdGraph* ParentGraph = GetGraph();

	if (ParentGraph->SubGraphs.Find(BoundGraph) == INDEX_NONE)
	{
		ParentGraph->Modify();
		ParentGraph->SubGraphs.Add(BoundGraph);
	}
}

FText USMGraphK2Node_BlockMachineNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if ((TitleType == ENodeTitleType::MenuTitle || TitleType == ENodeTitleType::ListView) && (BoundGraph == nullptr))
	{
		return LOCTEXT("AddNewBlockMachine", "Add New Block Machine...");
	}
	else if (BoundGraph == nullptr)
	{
		if (TitleType == ENodeTitleType::FullTitle)
		{
			return LOCTEXT("NullBlockMachineFullTitle", "Error: No Graph\nBlock Machine");
		}
		else
		{
			return LOCTEXT("ErrorNoGraph", "Error: No Graph");
		}
	}
	else if (TitleType == ENodeTitleType::FullTitle)
	{
		if (CachedFullTitle.IsOutOfDate(this))
		{
			FFormatNamedArguments Args;
			Args.Add(TEXT("Title"), FText::FromName(BoundGraph->GetFName()));
			// FText::Format() is slow, so we cache this to save on performance
			CachedFullTitle.SetCachedText(FText::Format(LOCTEXT("BlockMachineFullTitle", "{Title}\nBlock Machine"), Args), this);
		}
		return CachedFullTitle;
	}

	return FText::FromName(BoundGraph->GetFName());
}
#undef LOCTEXT_NAMESPACE
