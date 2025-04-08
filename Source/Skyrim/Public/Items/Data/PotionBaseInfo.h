// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBaseInfo.h"
#include "PotionBaseInfo.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FPotionBaseInfo : public FItemBaseInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PotionBase")
	EPotionType PotionType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PotionBase")
	float Amount;
};
