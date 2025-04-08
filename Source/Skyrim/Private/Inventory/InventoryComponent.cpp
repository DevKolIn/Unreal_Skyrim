// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/InventoryComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "Inventory/InventoryInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Mangers/ItemManager.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	DropOffset = 100.f;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	/* Bind Inventory Input Action (인벤토리 입력 액션 바인딩) */
	if (ACharacter* Character = Cast<ACharacter>(GetOuter()))
	{
		if (UEnhancedInputComponent* InputComponent = Cast<UEnhancedInputComponent>(Character->InputComponent))
		{
			CharacterInputComponent = InputComponent;
			InputComponent->BindAction(FavoriteAction, ETriggerEvent::Started, this, &UInventoryComponent::Favorite);
			InputComponent->BindAction(DropAction, ETriggerEvent::Started, this, &UInventoryComponent::DropItem);
			InputComponent->BindAction(MouseClickedAction, ETriggerEvent::Started, this, &UInventoryComponent::MouseClick);
			InputComponent->BindAction(MouseClickedAction, ETriggerEvent::Canceled, this, &UInventoryComponent::MouseClick);
			InputComponent->BindAction(MouseClickedAction, ETriggerEvent::Completed, this, &UInventoryComponent::MouseClick);
		}
	}
	
	/* Create Inventory Widget (인벤토리 위젯 생성) */
	if (GetWorld() && InventoryWidgetClass)
	{
		InventoryWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), InventoryWidgetClass);
	}
	
	if (OverencumberedWidgetClass)
		OverencumberedWidget = CreateWidget<UUserWidget>(GetWorld(), OverencumberedWidgetClass);

	/* Get InventoryItems From Subsystem (인벤토리 아이템 불러오기) */
	if (UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this))
	{
		ItemManager = GameInstance->GetSubsystem<UItemManager>();
		if (ItemManager)
		{
			InventoryItems = ItemManager->GetInventoryItems(GetUniqueID());
		}
	}
} 

void UInventoryComponent::ToggleInventory(UInputMappingContext* PlayerMappingContext)
{
	// 1. 인벤토리 위젯 유효한지 확인한다.
	if (InventoryWidget == nullptr)
		return;

	// 2. 인벤토리 토글을 알린다.
	OnToggleInventory.ExecuteIfBound();
	
	// 3. 입력 맵핑 컨텍스트를 변경한다.
	if (bIsOpenInventory == false)
	{
		ApplyMappingContext({InventoryMappingContext, PlayerMappingContext});
		bIsOpenInventory = true;
	}
	else
	{
		ApplyMappingContext(PlayerMappingContext);
		bIsOpenInventory = false;
	}
}


void UInventoryComponent::AddItem(AActor* Item)
{
	AddToInventory(Item);
	NotifyOverCumbered();
}

bool UInventoryComponent::IsOverEncumbered() const
{
	return false;
}

#pragma region EnhancedInput
void UInventoryComponent::Favorite()
{
	if (InventoryItems == nullptr)
		return;
	
	if (InventoryWidget.GetObject() == nullptr)
		return;
	
	FName ItemID = IInventoryInterface::Execute_GetCurrentSelectedItemID(InventoryWidget.GetObject());
	if (ItemID == NAME_None)
		return;

	if (FInventoryItemInfo* InventoryItemInfo = InventoryItems->Find(ItemID))
	{
		InventoryItemInfo->bIsFavorite ^= true;
		NotifyFavoriteItem();
	}
}

void UInventoryComponent::DropItem()
{
	if (InventoryWidget.GetObject() == nullptr)
		return;

	if (InventoryItems == nullptr)
		return;

	if (ItemManager == nullptr)
		return;

	FName ItemID = IInventoryInterface::Execute_GetCurrentSelectedItemID(InventoryWidget.GetObject());
	if (ItemID == NAME_None)
		return;
	
	// 1. 인벤토리에서 아이템을 꺼내온다.
	FInventoryItemInfo InventoryItemInfo;
	InventoryItems->RemoveAndCopyValue(ItemID, InventoryItemInfo);

	if (InventoryItemInfo.Item == nullptr)
		return;

	// 2. 아이템을 LevelItems에 넣는다.
	FLevelItemInfo LevelItemInfo;
	LevelItemInfo.Item = InventoryItemInfo.Item;
	LevelItemInfo.ItemCount = InventoryItemInfo.ItemCount;
	ItemManager->AddLevelItem(LevelItemInfo);

	// 3. 레벨에 아이템을 배치한다.
	FVector OwnerLocation = GetOwner()->GetActorLocation();
	FVector OwnerForwardDirection = GetOwner()->GetActorForwardVector();
	FVector DropPosition = (OwnerForwardDirection * DropOffset) + OwnerLocation;

	InventoryItemInfo.Item->SetActorLocation(DropPosition);
	InventoryItemInfo.Item->SetActorEnableCollision(true);
	InventoryItemInfo.Item->SetActorHiddenInGame(false);
	
	// 4. 위젯에게 드랍되었음을 알린다. (인벤토리 새로고침? OO)
	NotifyDropItem(ItemID);
}

void UInventoryComponent::MouseClick(const FInputActionInstance& Value)
{
	ETriggerEvent TriggerEvent = Value.GetTriggerEvent();

	switch (TriggerEvent)
	{
		case ETriggerEvent::Started:
			bIsMouseClicked = true;
			break;
		case ETriggerEvent::Canceled: // falls through
		case ETriggerEvent::Completed:
			bIsMouseClicked = false;
			break;
		default:
			break;
	}
}

void UInventoryComponent::ApplyMappingContext(const UInputMappingContext* MappingContext) const
{
	if (ACharacter* Character = Cast<ACharacter>(GetOuter()))
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->ClearAllMappings();
				Subsystem->AddMappingContext(MappingContext, 0);
			}
		}
	}
}

void UInventoryComponent::ApplyMappingContext(const TArray<UInputMappingContext*> MappingContexts) const
{
	if (ACharacter* Character = Cast<ACharacter>(GetOuter()))
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->ClearAllMappings();
				for (int index = 0; index < MappingContexts.Num(); ++index)
				{
					Subsystem->AddMappingContext(MappingContexts[index], MappingContexts.Num() - index);
				}
			}
		}
	}
}
#pragma endregion

void UInventoryComponent::AddToInventory(AActor* Item)
{
	if (Item == nullptr)
		return;
	
	if (ItemManager == nullptr)
		return;
	
	FName key = ItemManager->AddInventoryItem(GetUniqueID(), Item);
	NotifyAddItem(key, Item);
}

void UInventoryComponent::NotifyAddItem(FName ItemID, AActor* Item)
{
	OnAddItem.Broadcast(ItemID, Item);
}

void UInventoryComponent::NotifyOverCumbered()
{
	// 과적 상태가 되었다면 알림
	if (bWasOverCumbered == false && IsOverEncumbered())
	{
		bWasOverCumbered = true;
		OnOverCumberedSignature.ExecuteIfBound(true);
		return;
	}

	// 과적 상태에서 풀렸다면 알림
	if (bWasOverCumbered == true && IsOverEncumbered() == false)
	{
		bWasOverCumbered = false;
		OnOverCumberedSignature.ExecuteIfBound(false);
		return;
	}
}

void UInventoryComponent::NotifyDropItem(FName ItemID)
{
	OnDropItem.ExecuteIfBound(ItemID);
}

void UInventoryComponent::NotifyFavoriteItem()
{
	OnFavoriteItem.ExecuteIfBound();
}
