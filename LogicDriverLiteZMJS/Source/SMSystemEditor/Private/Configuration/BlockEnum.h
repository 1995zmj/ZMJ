// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlockEnum.generated.h"

UENUM(BlueprintType)
enum class EBlockType: uint8
{
	None = 0,
	EnterBlock = 1 UMETA(DisplayName = "EnterBlock"),
	EndBlock = 2 UMETA(DisplayName = "EndBlock"),
	TextBlock = 10 UMETA(DisplayName = "TextBlock"),
	OptionBlock = 11 UMETA(DisplayName = "OptionBlock"),
};


UENUM(BlueprintType)
enum class ETextCharacterType: uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Self UMETA(DisplayName = "Self"),
	TeamMember UMETA(DisplayName = "TeamMember"),
	Model UMETA(DisplayName = "Model"),
	Custom UMETA(DisplayName = "Custom"),
};

UENUM(BlueprintType)
enum class ETeamMemberIndex: uint8
{
	Random = 0 UMETA(DisplayName = "Random"),
	One UMETA(DisplayName = "1"),
	Two UMETA(DisplayName = "2"),
	Three UMETA(DisplayName = "3"),
	Four UMETA(DisplayName = "4"),
};



