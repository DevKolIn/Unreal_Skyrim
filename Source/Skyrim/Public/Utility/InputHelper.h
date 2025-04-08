// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InputHelper.generated.h"

/**
 * 
 */
UCLASS()
class SKYRIM_API UInputHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Input")
	static void ApplyMappingContext(APlayerController* PlayerController, class UInputMappingContext* MappingContext);
};
