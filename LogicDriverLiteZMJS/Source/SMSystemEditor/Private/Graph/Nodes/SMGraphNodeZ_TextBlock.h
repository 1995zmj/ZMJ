// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SMGraphNodeZ_BaseBlock.h"
#include "Configuration/BlockEnum.h"
#include "Configuration/BlockStruct.h"
#include "SMGraphNodeZ_TextBlock.generated.h"

/**
 * 
 */
UCLASS()
class SMSYSTEMEDITOR_API USMGraphNodeZ_TextBlock : public USMGraphNodeZ_BaseBlock
{
	GENERATED_BODY()
public:
	virtual void PostPlacedNewNode() override;
	virtual bool ShowPaletteIconOnNode() const override;
	virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
	virtual FLinearColor Internal_GetBackgroundColor() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void WriteDataAsJSON(const TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>>& JsonWriter) override;

	void AddOption();
	void RemoveOption(UEdGraphPin* TargetPin);
	bool CanRemoveOption(UEdGraphPin* TargetPin);

	void GetAllOptionsOutPin(TArray<UEdGraphPin*>& OutPins);
	UEdGraphPin* GetInPin();
	UEdGraphPin* GetOutPin();
	FString GetLinkBlockSerialNumberByPin(UEdGraphPin* Pin);

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ETextCharacterType TextCharacterType;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (EditCondition = "TextCharacterType == ETextCharacterType::TeamMember",EditConditionHides))
	ETeamMemberIndex CharacterTypeTeamMemberIndex;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (EditCondition = "TextCharacterType == ETextCharacterType::Custom",EditConditionHides))
	FString CharacterTypeCustomValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "TextCharacterType == ETextCharacterType::Model", EditConditionHides))
	FString CharacterTypeModelValue;

	// UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString TypeKey;
	// UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString TypeValue;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString TextContent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TArray<FString> Options;
};

