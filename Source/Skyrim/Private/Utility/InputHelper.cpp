// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/InputHelper.h"
#include "EnhancedInputSubsystems.h"

void UInputHelper::ApplyMappingContext(APlayerController* PlayerController, UInputMappingContext* MappingContext)
{
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
}
