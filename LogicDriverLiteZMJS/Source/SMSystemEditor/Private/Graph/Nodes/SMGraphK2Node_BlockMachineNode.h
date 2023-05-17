// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SMGraphK2Node_StateMachineNode.h"
#include "SMGraphK2Node_BlockMachineNode.generated.h"

/**
 * 
 */
UCLASS()
class SMSYSTEMEDITOR_API USMGraphK2Node_BlockMachineNode : public USMGraphK2Node_StateMachineNode
{
	GENERATED_BODY()

public:
	virtual void PostPlacedNewNode() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
};
