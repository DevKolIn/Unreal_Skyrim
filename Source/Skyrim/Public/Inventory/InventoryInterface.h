// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

class SKYRIM_API IInventoryInterface
{
	GENERATED_BODY()
	
public:
	/*UFUNCTION(BlueprintImplementableEvent, Category = "InventoryInterface")
	void ToggleInventory();
	
	UFUNCTION(BlueprintNativeEvent, Category = "InventoryInterface")
	void AddItem(AActor* Item);

	UFUNCTION(BlueprintImplementableEvent, Category = "InventoryInterface")
	void SetFavorite();

	UFUNCTION(BlueprintImplementableEvent, Category = "InventoryInterface")
	void DropItem();

	UFUNCTION(BlueprintImplementableEvent, Category = "InventoryInterface")
	bool IsInventoryOpen() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "InventoryInterface")
	bool IsOverEncumbered() const;*/

	UFUNCTION(BlueprintImplementableEvent, Category = "InventoryInterface")
	FName GetCurrentSelectedItemID() const;
};
