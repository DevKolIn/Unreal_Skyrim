// Fill out your copyright notice in the Description page of Project Settings.

#include "Mangers/ItemManager.h"

#include "GameplayTagAssetInterface.h"
#include "Kismet/GameplayStatics.h"

UItemManager::UItemManager()
{
}

void UItemManager::DeleteItem(AActor* Item)
{
	
}

FName UItemManager::AddInventoryItem(uint8 InventoryKey, AActor* Item, int32 ItemCount)
{
	if (Item == nullptr)
		return NAME_None;
	
	FName InventoryItemKey = GetInventoryItemKey(Item);
	if (InventoryItemKey == NAME_None)
		return NAME_None;

	TMap<FName, FInventoryItemInfo>& Inventory = InventoryItems.FindOrAdd(InventoryKey);
	if (FInventoryItemInfo* InventoryItemInfo = Inventory.Find(InventoryItemKey))
	{
		InventoryItemInfo->ItemCount++;
		return InventoryItemKey;
	}
	else
	{
		FInventoryItemInfo NewInventoryItemInfo;
		NewInventoryItemInfo.Item = Item;
		NewInventoryItemInfo.ItemCount = ItemCount;
		Inventory.Emplace(InventoryItemKey, NewInventoryItemInfo);
	}
	
	return InventoryItemKey;
}

void UItemManager::AddLevelItem(const FLevelItemInfo& LevelItemInfo)
{
	FName CurrentLevelKey = GetCurrentLevelKey();
	if (CurrentLevelKey == NAME_None)
		return;

	AActor* Item = LevelItemInfo.Item;
	if (Item == nullptr)
		return;

	uint8 ItemID = Item->GetUniqueID();
	if (auto* LevelItemInfos = LevelItems.Find(CurrentLevelKey))
	{
		LevelItemInfos->Add(ItemID, LevelItemInfo);
	}
	else
	{
		LevelItems.Emplace(CurrentLevelKey, TMap<uint8, FLevelItemInfo>());
		LevelItemInfos = &LevelItems[CurrentLevelKey];
		LevelItemInfos->Add(ItemID, LevelItemInfo);
	}
}

TMap<FName, FInventoryItemInfo>* UItemManager::GetInventoryItems(uint8 ItemKey)
{
	if (auto* Element = InventoryItems.Find(ItemKey))
	{
		return Element;
	}

	return &InventoryItems.Emplace(ItemKey, TMap<FName, FInventoryItemInfo>());
}

TMap<uint8, FLevelItemInfo>* UItemManager::GetLevelItems(FName LevelKey)
{
	if (auto* Element = LevelItems.Find(LevelKey))
	{
		return Element;
	}

	return &LevelItems.Emplace(LevelKey, TMap<uint8, FLevelItemInfo>());
}

FName UItemManager::GetCurrentLevelKey()
{
	if (GetWorld())
	{
		return GetWorld()->GetPackage()->GetFName();
	}

	return NAME_None;
}

FName UItemManager::GetInventoryItemKey(AActor* Item) const
{
	IGameplayTagAssetInterface* GameplayTagInterface = Cast<IGameplayTagAssetInterface>(Item);
	if (GameplayTagInterface == nullptr)
		return NAME_None;

	FName InventoryItemKey = NAME_None;
	
	FGameplayTagContainer GameplayTags;
	GameplayTagInterface->GetOwnedGameplayTags(GameplayTags);
	
	for (FGameplayTag Tag : GameplayTags.GetGameplayTagArray())
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Item.Equipable"))))
		{
			InventoryItemKey = Item->GetFName();
		}
		else if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Item.Consumable"))))
		{
			InventoryItemKey = Item->GetClass()->GetFName();
		}
	}

	return InventoryItemKey;
}
