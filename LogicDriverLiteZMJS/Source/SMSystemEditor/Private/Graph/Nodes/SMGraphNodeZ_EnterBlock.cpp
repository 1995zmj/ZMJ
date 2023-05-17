// Fill out your copyright notice in the Description page of Project Settings.


#include "SMGraphNodeZ_EnterBlock.h"

USMGraphNodeZ_EnterBlock::USMGraphNodeZ_EnterBlock()
{
	bCanRenameNode = false;
}

void USMGraphNodeZ_EnterBlock::AllocateDefaultPins()
{
	// Super::AllocateDefaultPins();
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, TEXT(""));
}

FText USMGraphNodeZ_EnterBlock::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Entry"));
}
