// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelItemInfo.generated.h"
/**
 * 
 */
USTRUCT()
struct FLevelItemInfo
{
	GENERATED_BODY()
	
	AActor* Item;
	int32 ItemCount;
	
	FLevelItemInfo() : Item(nullptr), ItemCount(0) {}
};
