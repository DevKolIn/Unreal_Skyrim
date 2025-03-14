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

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

/* Camera (카메라) */
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
	
/* Input (입력) */
protected:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;
	
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

/* Sensitivity (감도) */
protected:
	// 좌우 회전 감도
	UPROPERTY(EditAnywhere, Category = "Camera")
	float MouseSensitivityX;

	// 상하 회전 감도
	UPROPERTY(EditAnywhere, Category = "Camera")
	float MouseSensitivityY;  
};