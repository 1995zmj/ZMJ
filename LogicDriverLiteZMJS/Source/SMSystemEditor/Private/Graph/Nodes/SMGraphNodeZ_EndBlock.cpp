// Fill out your copyright notice in the Description page of Project Settings.


#include "SMGraphNodeZ_EndBlock.h"

USMGraphNodeZ_EndBlock::USMGraphNodeZ_EndBlock()
{
	bCanRenameNode = false;
}

void USMGraphNodeZ_EndBlock::AllocateDefaultPins()
{
	// Super::AllocateDefaultPins();
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, TEXT(""));
}

FText USMGraphNodeZ_EndBlock::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Exit"));
}
