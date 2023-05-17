// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SMGraphNodeZ_BaseBlock.h"
#include "SMGraphNodeZ_OptionBlock.generated.h"

/**
 * 
 */
UCLASS()
class SMSYSTEMEDITOR_API USMGraphNodeZ_OptionBlock : public USMGraphNodeZ_BaseBlock
{
	GENERATED_BODY()

public:
	virtual void PostPlacedNewNode() override;
	virtual void AllocateDefaultPins() override;
	virtual const FSlateBrush* GetNodeIcon() override;
	
	virtual FLinearColor Internal_GetBackgroundColor() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

	virtual void WriteDataAsJSON(const TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>>& JsonWriter) override;

	FString TypeKey;
	FString TypeValue;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString TextContent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bSHowOtherInfo;
};
