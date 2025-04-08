// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ItemBase.h"
#include "Items/ItemEnums.h"
#include "PotionBase.generated.h"

/**
 * 
 */
UCLASS()
class SKYRIM_API APotionBase : public AItemBase
{
	GENERATED_BODY()

public:
	APotionBase();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Potion")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	virtual void PostInitializeComponents() override;

/* Primary Value */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Potion | PrimaryValue")
	EPotionType PotionType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Potion | PrimaryValue")
	float Amount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Potion | PrimaryValue")
	float CurrentCount;
};
