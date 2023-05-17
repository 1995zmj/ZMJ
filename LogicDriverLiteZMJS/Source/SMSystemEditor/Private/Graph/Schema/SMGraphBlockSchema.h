// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SMGraphSchema.h"
#include "SMGraphBlockSchema.generated.h"

/**
 * 
 */
UCLASS()
class SMSYSTEMEDITOR_API USMGraphBlockSchema : public USMGraphSchema
{
	GENERATED_BODY()
public:
	static const FName PC_Option;
	
	virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;
	virtual void GetContextMenuActions(class UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* PinA, const UEdGraphPin* PinB) const override;
	virtual FConnectionDrawingPolicy* CreateConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements, UEdGraph* InGraphObj) const override;
	
};
