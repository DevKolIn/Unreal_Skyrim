// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBaseInfo.h"
#include "ArmorBaseInfo.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FArmorBaseInfo : public FItemBaseInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ArmorBase")
	EArmorType WeaponType;
};
