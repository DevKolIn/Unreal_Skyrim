// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SkyrimCharacter.generated.h"

UENUM(BlueprintType)
enum class ECameraMode : uint8
{
	TPS,
	FPS
};

class UUserWidget;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UChildActorComponent;
struct FInputActionValue;

UCLASS()
class SKYRIM_API ASkyrimCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASkyrimCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

/* Camera (카메라) */
#pragma region Camera
protected:
	UPROPERTY(EditDefaultsOnly, Category="Camera")
	TObjectPtr<USpringArmComponent> TPSSpringArmComponent;

	UPROPERTY(EditDefaultsOnly, Category="Camera")
	TObjectPtr<UChildActorComponent> TPSCameraActorComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Camera")
	TObjectPtr<USpringArmComponent> FPSSpringArmComponent;

	UPROPERTY(EditDefaultsOnly, Category="Camera")
	TObjectPtr<UChildActorComponent> FPSCameraActorComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Camera")
	ECameraMode CurrentCameraMode;

	UPROPERTY(EditDefaultsOnly, Category="Camera")
	float FPSCameraDistance;

	UPROPERTY(EditDefaultsOnly, Category="Camera")
	FVector CameraOffset;
	
	AActor* TPSCamera;
	AActor* FPSCamera;
	
	/* Zoom */
	float CurrentZoom;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float ZoomSpeed;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float MinZoom;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float MaxZoom;

	// 이 값보다 작아지면 FPS 모드 전환
	UPROPERTY(EditAnywhere, Category = "Camera")
	float ZoomThreshold;

	void UpdateCameraMode();
#pragma endregion
	
/* Enhanced Input (입력) */
#pragma region EnhancedInput
protected:
// Common Input
	void ApplyMappingContext(class UInputMappingContext* Context);
	
// Player Input
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> PlayerMappingContext;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveForwardAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveRightAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> TurnAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookUpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ZoomAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> InteractionAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> InventoryAction;
	
	UFUNCTION()
	void MoveForward(const FInputActionValue& Value);

	UFUNCTION()
	void MoveRight(const FInputActionValue& Value);

	UFUNCTION()
	void Turn(const FInputActionValue& Value);

	UFUNCTION()
	void LookUp(const FInputActionValue& Value);

	UFUNCTION()
	void StartJump();

	UFUNCTION()
	void StopJump();
	
	UFUNCTION()
	void Zoom(const FInputActionValue& Value);

	UFUNCTION()
	void ItemInteract();

	UFUNCTION()
	void ToggleInventory();
	
#pragma endregion
	
/* Sensitivity (감도) */
#pragma region Sensitivty
protected:
	// 좌우 회전 감도
	UPROPERTY(EditAnywhere, Category = "Camera")
	float MouseSensitivityX;

	// 상하 회전 감도
	UPROPERTY(EditAnywhere, Category = "Camera")
	float MouseSensitivityY;
#pragma endregion
	
/*  Inventory (인벤토리) */
#pragma region Inventory
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	TObjectPtr<class UInventoryComponent> InventoryComponent;

	UFUNCTION()
	void HandleAddItem(FName Key, AActor* Item);
#pragma endregion
	
/* Item Interaction (아이템 상호작용) */
#pragma region ItemInteraction
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interaction|Item")
	TObjectPtr<class UBoxComponent> ItemBoxCollisionComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interaction|Item")
	TSubclassOf<AActor> RenderTargetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interaction|Item")
	TObjectPtr<AActor> RenderTarget;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interaction|Item")
	TSubclassOf<UUserWidget> PickupWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interaction|Item")
	TObjectPtr<class APickup> PickupActor;

	AActor* ClosestItem;

	UPROPERTY()
	TSet<AActor*> OverlappingItems;
	
	UFUNCTION()
	void OnOverlapBeginItemBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEndItemBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void UpdatePickupWidget();
#pragma endregion
};