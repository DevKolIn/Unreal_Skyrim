// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Armors/ArmorBase.h"
#include "Items/Data/ItemTag/ItemTags.h"

AArmorBase::AArmorBase()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetupAttachment(GetRootComponent());

	StaticGameplayTags.AddTag(Tag_Item_Equipable_Armor);
}
