// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/InventoryItemInfo.h"
#include "InventoryComponent.generated.h"

class UItemManager;
class IInventoryInterface;
class UInputAction;
class UInputMappingContext;
class UUserWidget;
struct FInputActionInstance;

DECLARE_DELEGATE_OneParam(FOnOverCumberedSignature, bool);
DECLARE_DYNAMIC_DELEGATE(FOnToggleInventorySignature);
DECLARE_DYNAMIC_DELEGATE(FOnFavoriteItemSignature);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnDropItemSignature, FName, Key);
DECLARE_MULTICAST_DELEGATE_TwoParams(FHandleAddItem, FName Key, AActor* Item);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKYRIM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

/* External Use */
public:
	FOnOverCumberedSignature OnOverCumberedSignature;

	void ToggleInventory(UInputMappingContext* InPlayerMappingContext);
	void AddItem(AActor* Item);
	bool IsOverEncumbered() const;

/* Inventory Input */
protected:
	UPROPERTY()
	UInputComponent* CharacterInputComponent;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> InventoryMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> FavoriteAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> DropAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MouseClickedAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> InventoryAction;
	
	UFUNCTION()
	void Favorite();

	UFUNCTION()
	void DropItem();
	
	UFUNCTION()
	void MouseClick(const FInputActionInstance& Value);
	
	void ApplyMappingContext(const UInputMappingContext* MappingContext) const;
	void ApplyMappingContext(const TArray<UInputMappingContext*> MappingContexts) const;
	
/* Inventory Widget */
public:
	FHandleAddItem OnAddItem;

	UPROPERTY()
	FOnToggleInventorySignature OnToggleInventory;

	UPROPERTY()
	FOnFavoriteItemSignature OnFavoriteItem;

	UPROPERTY()
	FOnDropItemSignature OnDropItem;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory|Widget")
	TSubclassOf<UUserWidget> InventoryWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory|Widget")
	TScriptInterface<IInventoryInterface> InventoryWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory|Widget")
	TSubclassOf<UUserWidget> OverencumberedWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory|Widget")
	TObjectPtr<UUserWidget> OverencumberedWidget;
	
/* UItemManager */
protected:
	void AddToInventory(AActor* Item);
	UItemManager* ItemManager;

/* DropItem */
private:
	float DropOffset;
	
/* */
private:
	TMap<FName, FInventoryItemInfo>* InventoryItems;

	bool bIsOpenInventory;
	bool bWasOverCumbered;
	bool bIsMouseClicked;

	void NotifyAddItem(FName ItemID, AActor* Item);
	void NotifyOverCumbered();
	void NotifyDropItem(FName ItemID);
	void NotifyFavoriteItem();
};