// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemEnums.h"
#include "ItemBase.generated.h"

UCLASS()
class SKYRIM_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AItemBase();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ItemBase")
	TObjectPtr<USceneComponent> SceneComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ItemBase")
	FName Name;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ItemBase")
	EItemType ItemType;
};
