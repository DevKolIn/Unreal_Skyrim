// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemBase.h"

#include "Items/Data/ItemBaseInfo.h"
#include "Kismet/GameplayStatics.h"

AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = false;
	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);
}

void AItemBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ItemDataTable = ItemDataTableRow.DataTable;
	ItemName = ItemDataTableRow.RowName;

	if (ItemDataTable)
	{
		RowItemInfo = ItemDataTable->FindRowUnchecked(ItemName);
	}

	if (RowItemInfo)
	{
		if (const FItemBaseInfo* ItemInfo = reinterpret_cast<FItemBaseInfo*>(RowItemInfo))
		{
			ItemType = ItemInfo->ItemType;
			Weight = ItemInfo->Weight;
			Value = ItemInfo->Value;
			RenderTargetScale = ItemInfo->RenderTargetScale;
			PickupSound = ItemInfo->PickupSound;
		}
	}
}

void AItemBase::BeginPlay()
{
	Super::BeginPlay();
}

void AItemBase::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(StaticGameplayTags);
}

void AItemBase::PickupItem()
{
	if (PickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickupSound, GetActorLocation());
	}

	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
}
