// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryInterface.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UInventoryComponent;
/**
 * 
 */
UCLASS()
class SKYRIM_API UInventoryWidget : public UUserWidget, public IInventoryInterface
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
/* Inventory Component */
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InventoryComponent")
	UInventoryComponent* OwningComponent;
	
	UFUNCTION(BlueprintImplementableEvent)
	void AddItem(FName Key, AActor* Item);

	UFUNCTION(BlueprintImplementableEvent)
	void ToggleInventory();
	
	UFUNCTION(BlueprintImplementableEvent)
	void FavoriteItem();
	
	UFUNCTION(BlueprintImplementableEvent)
	void DropItem(FName ItemID);
};
