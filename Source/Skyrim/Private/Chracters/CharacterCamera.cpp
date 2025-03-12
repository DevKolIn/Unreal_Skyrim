// Fill out your copyright notice in the Description page of Project Settings.


#include "Chracters/CharacterCamera.h"
#include "Camera/CameraComponent.h"

ACharacterCamera::ACharacterCamera()
{
	PrimaryActorTick.bCanEverTick = false;
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
}
