// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ItemBase.h"
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	EWeaponType WeaponType;
	
	UPROPERTY(EditAnywhere, Category = "Weapon | Stat")
	float Damage;
};
