#pragma once

#include "SGraphNodeZ_BaseBlockNode.h"
#include "SGraphNode_StateNode.h"
#include "Graph/Nodes/SMGraphNodeZ_TextBlock.h"

class SGraphNodeZ_TextBlockNode : public SGraphNodeZ_BaseBlockNode
{
public:
	SLATE_BEGIN_ARGS(SGraphNodeZ_TextBlockNode) {}
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs, USMGraphNodeZ_TextBlock* InNode);

	virtual void UpdateGraphNode() override;
	virtual void CreatePinWidgets() override;
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
};

