// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Potions/PotionBase.h"

#include "Items/Data/PotionBaseInfo.h"
#include "Items/Data/ItemTag/ItemTags.h"

APotionBase::APotionBase()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetupAttachment(GetRootComponent());

	StaticGameplayTags.AddTag(Tag_Item_Consumable_Potion);
}

void APotionBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (RowItemInfo)
	{
		if (const FPotionBaseInfo* PotionInfo = reinterpret_cast<FPotionBaseInfo*>(RowItemInfo))
		{
			Amount = PotionInfo->Amount;
			PotionType = PotionInfo->PotionType;
			CurrentCount = 1;
		}
	}
}
