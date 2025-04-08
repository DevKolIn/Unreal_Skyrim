// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Items/ItemEnums.h"
#include "ItemBaseInfo.generated.h"

USTRUCT(BlueprintType)
struct FItemBaseInfo : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ItemBase")
	FText Description;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ItemBase")
	EItemType ItemType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ItemBase")
	float Weight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ItemBase")
	float Value;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ItemBase")
	float RenderTargetScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ItemBase")
	USoundBase* PickupSound;
};
