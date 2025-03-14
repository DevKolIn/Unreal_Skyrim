// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Armors/ArmorBase.h"

AArmorBase::AArmorBase()
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	SkeletalMeshComponent->SetupAttachment(GetRootComponent());
}
