// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ItemBase.h"
#include "Items/ItemEnums.h"
#include "ArmorBase.generated.h"

/**
 * 
 */
UCLASS()
class SKYRIM_API AArmorBase : public AItemBase
{
	GENERATED_BODY()

public:
	AArmorBase();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Armor")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Armor")
	EArmorType ArmorType;
	
	UPROPERTY(EditAnywhere, Category = "Weapon | Stat")
	float Damage;
};
