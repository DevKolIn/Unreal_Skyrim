// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CharacterCamera.generated.h"

UCLASS()
class SKYRIM_API ACharacterCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	ACharacterCamera();

protected:
	UPROPERTY()
	TObjectPtr<class UCameraComponent> CameraComponent;
};
