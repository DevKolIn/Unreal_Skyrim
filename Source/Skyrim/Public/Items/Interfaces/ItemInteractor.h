// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInteractor.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemInteractor : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SKYRIM_API IItemInteractor
{
	GENERATED_BODY()

public:
	virtual void PickupItem() = 0;
};
