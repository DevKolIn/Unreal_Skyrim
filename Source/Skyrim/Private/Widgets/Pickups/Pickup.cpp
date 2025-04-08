// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Pickups/Pickup.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);
	
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(SceneComponent);
}

void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateRotationToPlayer();
}

void APickup::CreatePickupWidget(const TSubclassOf<UUserWidget> WidgetClass)
{
	if (WidgetComponent == nullptr || WidgetClass == nullptr)
		return;
	
	if (UUserWidget* NewPickupWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass))
	{
		// 기존의 PickupWidget 삭제
		if (PickupWidget)
			PickupWidget->RemoveFromRoot();

		// 새로운 위젯 할당
		WidgetComponent->SetWidget(NewPickupWidget);
		PickupWidget = NewPickupWidget;
		PickupWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void APickup::Activate(const AActor* Target)
{
	if (PickupWidget == nullptr)
		return;
	
	if (Target == nullptr)
		return;

	SetActorLocation(Target->GetActorLocation());
	UpdateRotationToPlayer();
	SetActorTickEnabled(true);
	PickupWidget->SetVisibility(ESlateVisibility::Visible);
}

void APickup::Deactivate()
{
	if (PickupWidget == nullptr)
		return;

	PickupWidget->SetVisibility(ESlateVisibility::Hidden);
	SetActorTickEnabled(false);
}

void APickup::UpdateRotationToPlayer()
{
	if (APlayerCameraManager* CameraManager =  UGameplayStatics::GetPlayerCameraManager(this, 0))
	{
		FVector ActorLocation = GetActorLocation();
		FVector CameraLocation = CameraManager->GetCameraLocation();
		FRotator LookAtRotation  = UKismetMathLibrary::FindLookAtRotation(ActorLocation, CameraLocation);
		SetActorRotation({0.f, LookAtRotation.Yaw, 0.f});
	}
}

