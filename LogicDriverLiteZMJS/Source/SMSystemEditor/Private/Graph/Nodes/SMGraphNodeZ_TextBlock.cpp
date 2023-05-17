// Fill out your copyright notice in the Description page of Project Settings.


#include "SMGraphNodeZ_TextBlock.h"

#include "Configuration/SMEditorStyle.h"
#include "Graph/Schema/SMGraphBlockSchema.h"
#include "Graph/Schema/SMGraphK2Schema.h"
#include "Serialization/JsonWriter.h"

void USMGraphNodeZ_TextBlock::PostPlacedNewNode()
{
	Super::PostPlacedNewNode();
}

bool USMGraphNodeZ_TextBlock::ShowPaletteIconOnNode() const
{
	return true;
}

FSlateIcon USMGraphNodeZ_TextBlock::GetIconAndTint(FLinearColor& OutColor) const
{
	return FSlateIcon(FSMEditorStyle::GetStyleSetName(),"SMGraph.TextBlock");
}

FLinearColor USMGraphNodeZ_TextBlock::Internal_GetBackgroundColor() const
{
	return Super::Internal_GetBackgroundColor();
}

FText USMGraphNodeZ_TextBlock::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	FString temp = TEXT("文本\n") + TextContent;
	return FText::FromString(temp);
}

void USMGraphNodeZ_TextBlock::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(USMGraphNodeZ_TextBlock, TextContent))
	{
		Modify();
		const UEdGraphSchema* Schema = GetSchema();
		Schema->ReconstructNode(*this);
	}
	
}

void USMGraphNodeZ_TextBlock::WriteDataAsJSON(
	const TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>>& JsonWriter)
{
	JsonWriter->WriteObjectStart();
	// JsonWriter->WriteValue("TypeKey", TypeKey);
	// JsonWriter->WriteValue("TypeValue", TypeValue);
	// JsonWriter->WriteValue("TextContent", TextContent);
	JsonWriter->WriteValue("InBLockID",GetLinkBlockSerialNumberByPin(GetInPin()));
	JsonWriter->WriteValue("OutBLockID",GetLinkBlockSerialNumberByPin(GetOutPin()));
	JsonWriter->WriteArrayStart("Options");
		TArray<UEdGraphPin*> LocalPins;
		GetAllOptionsOutPin(LocalPins);
		for (int i = 0; i < LocalPins.Num(); ++i)
		{
			JsonWriter->WriteObjectStart();
			JsonWriter->WriteValue("OptionsName", Options[i]);
			auto LocalOtherPins = GetPinAt(GetPinIndex(LocalPins[i]) - 1);
			JsonWriter->WriteValue("OptionsDataBlockID",GetLinkBlockSerialNumberByPin(LocalPins[i]));
			JsonWriter->WriteValue("OptionsOutBlockID",GetLinkBlockSerialNumberByPin(LocalOtherPins));
			JsonWriter->WriteObjectEnd();
		}
	JsonWriter->WriteArrayEnd();
	JsonWriter->WriteObjectEnd();
}

void USMGraphNodeZ_TextBlock::AddOption()
{
	Modify();
	TArray<UEdGraphPin*> LocalPins;
	GetAllOptionsOutPin(LocalPins);
	FString Name = TEXT("Option_") + FString::FromInt(LocalPins.Num());
	CreatePin(EGPD_Input, USMGraphK2Schema::PC_StateMachine, USMGraphBlockSchema::PC_Option, *Name);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec,USMGraphBlockSchema::PC_Option, *Name);
	Options.Add(Name);
}

void USMGraphNodeZ_TextBlock::RemoveOption(UEdGraphPin* TargetPin)
{
	USMGraphNodeZ_TextBlock* OwningSeq = Cast<USMGraphNodeZ_TextBlock>( TargetPin->GetOwningNode() );
	if (OwningSeq)
	{
		int Index = -1;
		TArray<UEdGraphPin*> LocalPins;
		GetAllOptionsOutPin(LocalPins);
		Index = LocalPins.Find(TargetPin);
		if (Index >= 0 && Index < LocalPins.Num()) 
		{
			auto LocalOtherPins = GetPinAt(GetPinIndex(TargetPin) - 1);

			OwningSeq->Pins.Remove(TargetPin);
			TargetPin->MarkPendingKill();
			
			OwningSeq->Pins.Remove(LocalOtherPins);
			LocalOtherPins->MarkPendingKill();
			
			Options.RemoveAt(Index);
		}
		
	}
}

bool USMGraphNodeZ_TextBlock::CanRemoveOption(UEdGraphPin* TargetPin)
{
	USMGraphNodeZ_TextBlock* OwningSeq = Cast<USMGraphNodeZ_TextBlock>( TargetPin->GetOwningNode() );
	if (OwningSeq)
	{
		return TargetPin->PinType.PinSubCategory == USMGraphBlockSchema::PC_Option && TargetPin->Direction == EGPD_Output;
	}
	return false;
}

void USMGraphNodeZ_TextBlock::GetAllOptionsOutPin(TArray<UEdGraphPin*>& OutPins)
{
	for (auto Pin : Pins)
	{
		if (Pin->Direction == EGPD_Output && Pin->PinType.PinSubCategory == USMGraphBlockSchema::PC_Option)
		{
			OutPins.Add(Pin);
		}
	}
}

UEdGraphPin* USMGraphNodeZ_TextBlock::GetInPin()
{
	for (auto Pin : Pins)
	{
		if (Pin->Direction == EGPD_Input && Pin->PinType.PinSubCategory != USMGraphBlockSchema::PC_Option)
		{
			return Pin;
		}
	}
	return nullptr;
}

UEdGraphPin* USMGraphNodeZ_TextBlock::GetOutPin()
{
	for (auto Pin : Pins)
	{
		if (Pin->Direction == EGPD_Output && Pin->PinType.PinSubCategory != USMGraphBlockSchema::PC_Option)
		{
			return Pin;
		}
	}
	return nullptr;
}

FString USMGraphNodeZ_TextBlock::GetLinkBlockSerialNumberByPin(UEdGraphPin* Pin)
{
	if (Pin->LinkedTo.Num() > 0)
	{
		auto Block = Cast<USMGraphNodeZ_BaseBlock>(Pin->LinkedTo[0]->GetOwningNode());
		if (Block)
		{
			return Block->GetSerialNumber();
		}
	}
	return TEXT("0");
}
