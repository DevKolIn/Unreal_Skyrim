// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Potions/PotionBase.h"

APotionBase::APotionBase()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetupAttachment(GetRootComponent());
}
