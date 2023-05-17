// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SMGraphNodeZ_BaseBlock.h"
#include "SMGraphNodeZ_EndBlock.generated.h"

/**
 * 
 */
UCLASS()
class SMSYSTEMEDITOR_API USMGraphNodeZ_EndBlock : public USMGraphNodeZ_BaseBlock
{
	GENERATED_BODY()
public:
	USMGraphNodeZ_EndBlock();
	virtual void AllocateDefaultPins() override;
	virtual bool CanUserDeleteNode() const override { return false; }
    virtual bool CanDuplicateNode() const override { return false; }
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
};
