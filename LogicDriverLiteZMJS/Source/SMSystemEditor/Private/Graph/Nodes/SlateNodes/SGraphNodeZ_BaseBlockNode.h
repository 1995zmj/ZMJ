#pragma once

#include "K2Node_AddPinInterface.h"
#include "SGraphNode.h"
#include "Graph/Nodes/SMGraphNodeZ_BaseBlock.h"

class SGraphNodeZ_BaseBlockNode : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SGraphNodeZ_BaseBlockNode) {}
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs, USMGraphNodeZ_BaseBlock* InNode);
	// SNodePanel::SNode
	virtual void GetNodeInfoPopups(FNodeInfoContext* Context, TArray<FGraphInformationPopupInfo>& Popups) const override;
	// ~SNodePanel::SNode
	
	virtual bool ShowPaletteIconOnNode() const { return true; }

	virtual void UpdateGraphNode() override;
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
};
