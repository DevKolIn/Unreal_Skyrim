// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayTagContainer.h"
#include "ItemEnums.h"
#include "Interfaces/ItemInteractor.h"

#include "ItemBase.generated.h"

struct FItemBaseInfo;

UCLASS()
class SKYRIM_API AItemBase : public AActor, public IGameplayTagAssetInterface, public IItemInteractor
{
	GENERATED_BODY()

public:	
	AItemBase();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ItemBase")
	TObjectPtr<USceneComponent> SceneComponent;

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

/* GameplayTags */
public:
	//~IGameplayTagAssetInterface
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	//~End of IGameplayTagAssetInterface
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayTags")
	FGameplayTagContainer StaticGameplayTags;

/* Item Interaction*/
public:
	//~IItemInteractorInterface
	virtual void PickupItem() override;
	//~End of IItemInteractorInterface
	
/* Data Table */
public:
	UFUNCTION(BlueprintCallable)
	const UDataTable* GetDataTable() { return ItemDataTable; }
	
	UFUNCTION(BlueprintCallable)
	FName GetItemName() const { return ItemName;}
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ItemBase", Meta = (RowType = "/Script/Skyrim.ItemBaseInfo"))
	FDataTableRowHandle ItemDataTableRow;

	UPROPERTY(BlueprintReadOnly, Category = "ItemBase")
	TObjectPtr<const UDataTable> ItemDataTable;

	UPROPERTY(BlueprintReadOnly, Category = "ItemBase")
	FName ItemName;
	
	uint8* RowItemInfo;

/* Primary Value */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemBase | PrimaryValue")
	EItemType ItemType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemBase | PrimaryValue")
	float Weight;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemBase | PrimaryValue")
	float Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemBase  |PrimaryValue")
	float RenderTargetScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemBase  |PrimaryValue")
	USoundBase* PickupSound;
	
	UFUNCTION(BlueprintCallable)
	EItemType GetItemType() const { return ItemType; }
	
	UFUNCTION(BlueprintCallable)
	float GetWeight() const { return Weight; }

	UFUNCTION(BlueprintCallable)
	float GetValue() const { return Value;}

	UFUNCTION(BlueprintCallable)
	float GetRenderTargetScale() const { return RenderTargetScale; }
};
