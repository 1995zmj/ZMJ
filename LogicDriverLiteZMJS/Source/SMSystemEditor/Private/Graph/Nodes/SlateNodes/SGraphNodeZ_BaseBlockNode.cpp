#include "SGraphNodeZ_BaseBlockNode.h"
#include "GraphEditorSettings.h"
#include "Configuration/SMEditorSettings.h"
#include "Utilities/SMBlueprintEditorUtils.h"

#define LOCTEXT_NAMESPACE "SGraphBaseBlockNode"

void SGraphNodeZ_BaseBlockNode::Construct(const FArguments& InArgs, USMGraphNodeZ_BaseBlock* InNode)
{
	this->GraphNode = InNode;
	this->SetCursor(EMouseCursor::CardinalCross);
	this->UpdateGraphNode();
}

void SGraphNodeZ_BaseBlockNode::GetNodeInfoPopups(FNodeInfoContext* Context, TArray<FGraphInformationPopupInfo>& Popups) const
{

}

void SGraphNodeZ_BaseBlockNode::UpdateGraphNode()
{
	SGraphNode::UpdateGraphNode();
}

void SGraphNodeZ_BaseBlockNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	SGraphNode::AddPin(PinToAdd);
}

#undef LOCTEXT_NAMESPACE
