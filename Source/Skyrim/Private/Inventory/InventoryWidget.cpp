// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryWidget.h"

#include "GameFramework/Character.h"
#include "Inventory/InventoryComponent.h"

void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (ACharacter* Character = Cast<ACharacter>(GetOwningPlayerPawn()))
	{
		if (UInventoryComponent* InventoryComponent = Character->FindComponentByClass<UInventoryComponent>())
		{
			OwningComponent = InventoryComponent;
			InventoryComponent->OnAddItem.AddUObject(this, &UInventoryWidget::AddItem);
			InventoryComponent->OnToggleInventory.BindDynamic(this, &UInventoryWidget::UInventoryWidget::ToggleInventory);
			InventoryComponent->OnFavoriteItem.BindDynamic(this, &UInventoryWidget::UInventoryWidget::FavoriteItem);
			InventoryComponent->OnDropItem.BindDynamic(this, &UInventoryWidget::UInventoryWidget::DropItem);
		}
	}
}
