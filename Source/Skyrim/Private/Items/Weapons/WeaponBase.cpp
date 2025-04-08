// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/WeaponBase.h"

#include "Components/CapsuleComponent.h"
#include "Items/Data/WeaponBaseInfo.h"
#include "Items/Data/ItemTag/ItemTags.h"


AWeaponBase::AWeaponBase()
{
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Block);
	CapsuleComponent->SetSimulatePhysics(true);
	CapsuleComponent->SetEnableGravity(true);
	SetRootComponent(CapsuleComponent);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	SkeletalMeshComponent->SetupAttachment(CapsuleComponent);
	
	StaticGameplayTags.AddTag(Tag_Item_Equipable_Weapon);
}

void AWeaponBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	if (RowItemInfo)
	{
		if (const FWeaponBaseInfo* WeaponInfo = reinterpret_cast<FWeaponBaseInfo*>(RowItemInfo))
		{
			Damage = WeaponInfo->Damage;
			WeaponType = WeaponInfo->WeaponType;
		}
	}
}
