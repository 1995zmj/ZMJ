// Fill out your copyright notice in the Description page of Project Settings.


#include "SMGraphNodeZ_OptionBlock.h"
#include "Configuration/SMEditorStyle.h"
#include "Graph/Schema/SMGraphBlockSchema.h"
#include "Graph/Schema/SMGraphK2Schema.h"
#include "Serialization/JsonWriter.h"

void USMGraphNodeZ_OptionBlock::PostPlacedNewNode()
{
	Super::PostPlacedNewNode();
	TextContent = TEXT("你好！");
}

void USMGraphNodeZ_OptionBlock::AllocateDefaultPins()
{
	// Super::AllocateDefaultPins();
	CreatePin(EGPD_Output, USMGraphK2Schema::PC_StateMachine, USMGraphBlockSchema::PC_Option);
}

const FSlateBrush* USMGraphNodeZ_OptionBlock::GetNodeIcon()
{
	return FSMEditorStyle::Get()->GetBrush(TEXT("SMGraph.OptionBlock"));
}

FLinearColor USMGraphNodeZ_OptionBlock::Internal_GetBackgroundColor() const
{
	return Super::Internal_GetBackgroundColor();
}

FText USMGraphNodeZ_OptionBlock::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TextContent);
}

void USMGraphNodeZ_OptionBlock::WriteDataAsJSON(
	const TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>>& JsonWriter)
{
	JsonWriter->WriteObjectStart();
	JsonWriter->WriteValue("TypeKey", TypeKey);
	JsonWriter->WriteValue("TypeValue", TypeValue);
	JsonWriter->WriteValue("TextContent", TextContent);
	JsonWriter->WriteObjectEnd();
}
