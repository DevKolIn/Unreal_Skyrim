// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ItemBase.h"
#include "WeaponBase.generated.h"

/**
 * 
 */
UCLASS()
class SKYRIM_API AWeaponBase : public AItemBase
{
	GENERATED_BODY()
public:
	AWeaponBase();

protected:
	virtual void PostInitializeComponents() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<class UCapsuleComponent> CapsuleComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

/* Primary Value */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | PrimaryValue")
	float Damage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon | PrimaryValue")
	EWeaponType WeaponType;
};
