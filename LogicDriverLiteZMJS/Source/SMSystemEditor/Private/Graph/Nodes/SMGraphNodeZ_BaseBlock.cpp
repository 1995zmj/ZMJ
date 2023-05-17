// Fill out your copyright notice in the Description page of Project Settings.


#include "SMGraphNodeZ_BaseBlock.h"

#include "Configuration/BlockEnum.h"
#include "Configuration/SMEditorStyle.h"
#include "Serialization/JsonWriter.h"

USMGraphNodeZ_BaseBlock::USMGraphNodeZ_BaseBlock()
{
	bCanRenameNode = false;
}

void USMGraphNodeZ_BaseBlock::PostRename(UObject* OldOuter, const FName OldName)
{
	Super::PostRename(OldOuter, OldName);
	UpdateImportanceID();
}

void USMGraphNodeZ_BaseBlock::ReconstructNode()
{
	Super::ReconstructNode();
}

void USMGraphNodeZ_BaseBlock::AllocateDefaultPins()
{
	// Super::AllocateDefaultPins();
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, TEXT("In"));
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, TEXT("Out"));
}

void USMGraphNodeZ_BaseBlock::PostPlacedNewNode()
{
	Super::PostPlacedNewNode();
}

void USMGraphNodeZ_BaseBlock::PostPasteNode()
{
	Super::PostPasteNode();
}

const FSlateBrush* USMGraphNodeZ_BaseBlock::GetNodeIcon()
{
	return FSMEditorStyle::Get()->GetBrush(TEXT("SMGraph.BaseBlock"));
}

FLinearColor USMGraphNodeZ_BaseBlock::Internal_GetBackgroundColor() const
{
	// return Super::Internal_GetBackgroundColor();
	return FLinearColor::Blue;
}

FText USMGraphNodeZ_BaseBlock::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Base Block"));
}

void USMGraphNodeZ_BaseBlock::OnRenameNode(const FString& NewName)
{
	Super::OnRenameNode(NewName);
}

FLinearColor USMGraphNodeZ_BaseBlock::GetNodeTitleColor() const
{
	// return Super::GetNodeTitleColor();
	return FLinearColor::Blue;
}

FString USMGraphNodeZ_BaseBlock::GetBlockTypeID()
{
	auto ClassName = GetClass()->GetName();
	TArray<FString> StrArray;  
	ClassName.ParseIntoArray(StrArray, TEXT("_"), false);
	auto Id = StaticEnum<EBlockType>()->GetValueByName(*StrArray[1]);
	return FString::FromInt(Id);
}

FString USMGraphNodeZ_BaseBlock::GetBlockID()
{
	auto Name = GetName();
	TArray<FString> StrArray;  
	Name.ParseIntoArray(StrArray, TEXT("_"), false);
	if (StrArray.Num() > 0)
	{
		return StrArray[StrArray.Num() - 1];
	}
	else
	{
		return TEXT("0");
	}
}

void USMGraphNodeZ_BaseBlock::UpdateImportanceID()
{
	BlockIdStr = GetBlockID();
	BlockTypeIdStr = GetBlockTypeID();
	SerialNumber = BlockTypeIdStr + TEXT("_") + BlockIdStr;
}

FString USMGraphNodeZ_BaseBlock::GetSerialNumber()
{
	return GetBlockTypeID() + TEXT("_") +  GetBlockID();
}

void USMGraphNodeZ_BaseBlock::WriteDataAsJSON(
	const TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>>& JsonWriter)
{
	JsonWriter->WriteObjectStart();
	JsonWriter->WriteObjectEnd();
}
