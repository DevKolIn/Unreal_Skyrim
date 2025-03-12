// Fill out your copyright notice in the Description page of Project Settings.


#include "Chracters/SkyrimCharacter.h"

// Sets default values
ASkyrimCharacter::ASkyrimCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASkyrimCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASkyrimCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASkyrimCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

