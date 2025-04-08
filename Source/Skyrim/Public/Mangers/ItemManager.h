// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryItemInfo.h"
#include "Items/Data/LevelItemInfo.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ItemManager.generated.h"

/*
UENUM(BlueprintType)
enum class EItemCategory : uint8
{
	Inventory,
	End UMETA(Hidden)
};
*/

UCLASS(BlueprintType)
class SKYRIM_API UItemManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UItemManager();
	
	UFUNCTION(BlueprintCallable)
	void DeleteItem(AActor* Item);

public:
	FName AddInventoryItem(uint8 InventoryKey, AActor* Item, int32 ItemCount = 1);
	void AddLevelItem(const FLevelItemInfo& LevelItemInfo);
	
	TMap<FName, FInventoryItemInfo>* GetInventoryItems(uint8 LevelKey);
	TMap<uint8, FLevelItemInfo>* GetLevelItems(FName ItemKey);

	FName GetInventoryItemKey(AActor* Item) const;

private:
	TMap<FName, TMap<uint8, FLevelItemInfo>> LevelItems;
	TMap<uint8, TMap<FName, FInventoryItemInfo>> InventoryItems;
	FName GetCurrentLevelKey();
};