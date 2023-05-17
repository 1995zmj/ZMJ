// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlockStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FBlockServer : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString Content;

	UPROPERTY(EditAnywhere)
	TMap<FString,FString> DataMap;
};
USTRUCT(BlueprintType)
struct FTextBlockData
{
	// GENERATED_USTRUCT_BODY()

	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "TextBlockData")
	FString Type;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "TextBlockData")
	FString TextContent;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "TextBlockData")
	TSoftObjectPtr<UDataTable> Table;
};


// #pragma once
// #include "CoreMinimal.h"
// #include "AnimInstanceEnum_ZMJ.generated.h"
//
// UENUM(BlueprintType)
// enum class EMovementDirection_ZMJ: uint8
// {
// 	Forward UMETA(DisplayName = "Forward"),
// 	Right UMETA(DisplayName = "Right"),
// 	Left UMETA(DisplayName = "Left"),
// 	Backward UMETA(DisplayName = "Backward"),
// };
//
// UENUM(BlueprintType)
// enum class EHipsDirection_ZMJ: uint8
// {
// 	F UMETA(DisplayName = "F"),
// 	B UMETA(DisplayName = "B"),
// 	RF UMETA(DisplayName = "RF"),
// 	RB UMETA(DisplayName = "RB"),
// 	LF UMETA(DisplayName = "LF"),
// 	LB UMETA(DisplayName = "LB"),
// };

