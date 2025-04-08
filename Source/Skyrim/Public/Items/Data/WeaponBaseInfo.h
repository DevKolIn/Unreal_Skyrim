// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBaseInfo.h"
#include "WeaponBaseInfo.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FWeaponBaseInfo : public FItemBaseInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponBase")
	EWeaponType WeaponType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponBase")
	float Damage;
};
