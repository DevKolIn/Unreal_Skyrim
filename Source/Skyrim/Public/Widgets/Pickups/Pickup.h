// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class SKYRIM_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	APickup();
	
public:	
	virtual void Tick(float DeltaTime) override;

public:
	void CreatePickupWidget(const TSubclassOf<UUserWidget> WidgetClass);
	void Activate(const AActor* Target);
	void Deactivate();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pickup")
	TObjectPtr<USceneComponent> SceneComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pickup")
	TObjectPtr<class UWidgetComponent> WidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pickup")
	TObjectPtr<UUserWidget> PickupWidget;

private:
	FORCEINLINE void UpdateRotationToPlayer(); 
};
