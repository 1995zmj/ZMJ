// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SMGraphNode_StateNode.h"
#include "SMGraphNodeZ_BaseBlock.generated.h"

/**
 * 
 */
UCLASS()
class SMSYSTEMEDITOR_API USMGraphNodeZ_BaseBlock : public USMGraphNode_StateNode
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere)
	FString BlockTypeIdStr;
	UPROPERTY(VisibleAnywhere)
	FString BlockIdStr;
	UPROPERTY(VisibleAnywhere)
	FString SerialNumber;
	
public:
	USMGraphNodeZ_BaseBlock();
	// UObject
	virtual void PostRename(UObject* OldOuter, const FName OldName) override;
	
	virtual void ReconstructNode() override;
	virtual void AllocateDefaultPins() override;
	virtual void PostPlacedNewNode() override;
	virtual void PostPasteNode() override;
	virtual const FSlateBrush* GetNodeIcon() override;
	virtual FLinearColor Internal_GetBackgroundColor() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void OnRenameNode(const FString& NewName) override;

	// UEdGraphNode override
	virtual FLinearColor GetNodeTitleColor() const override;

	FString GetBlockTypeID();
	FString GetBlockID();
	void UpdateImportanceID();
	FString GetSerialNumber();
	
	virtual void WriteDataAsJSON(const TSharedRef< TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR> > >& JsonWriter);

};
