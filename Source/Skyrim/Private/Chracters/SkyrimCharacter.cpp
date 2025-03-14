#include "Chracters/SkyrimCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Chracters/CharacterCamera.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

ASkyrimCharacter::ASkyrimCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// Init Zoom (줌 설정)
	ZoomSpeed = 45.f;
	MinZoom = 100.f;
	MaxZoom = 300.f;
	ZoomThreshold = 150.f;
	CurrentZoom = MaxZoom;
	
	/* init Camera (카메라 설정) */
	CurrentCameraMode = ECameraMode::TPS;
	CameraOffset = FVector(0, 100, 0);
	
	TPSSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("TPSSpringArmComponent"));
	TPSSpringArmComponent->SetupAttachment(GetRootComponent());
	TPSSpringArmComponent->TargetArmLength = MaxZoom;
	TPSSpringArmComponent->bUsePawnControlRotation = true;
	TPSSpringArmComponent->SocketOffset = CameraOffset;
	
	TPSCameraActorComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("TPSCameraActorComponent"));
	TPSCameraActorComponent->SetChildActorClass(ACharacterCamera::StaticClass());
	TPSCameraActorComponent->SetupAttachment(TPSSpringArmComponent, USpringArmComponent::SocketName);
	
	FPSCameraDistance = 100.f;
	FPSSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("FPSSpringArmComponent"));
	FPSSpringArmComponent->SetupAttachment(GetRootComponent());
	FPSSpringArmComponent->TargetArmLength = 0;
	FPSSpringArmComponent->bUsePawnControlRotation = true;
	
	FPSCameraActorComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("FPSCameraActorComponent"));
	FPSCameraActorComponent->SetChildActorClass(ACharacterCamera::StaticClass());
	FPSCameraActorComponent->SetupAttachment(FPSSpringArmComponent, USpringArmComponent::SocketName);
	
	/* Init Character Rotation (캐릭터 회전 설정) */
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	/* Init Sensitivity (마우스 감도 설정) */
	MouseSensitivityX = 0.7f;
	MouseSensitivityY = 0.7f;
}

void ASkyrimCharacter::BeginPlay()
{
	Super::BeginPlay(); 
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		// 입력 맵핑 컨텍스트 설정
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}

		// 메인 카메라 설정
		TPSCamera = TPSCameraActorComponent->GetChildActor();
		FPSCamera = FPSCameraActorComponent->GetChildActor();
		PlayerController->SetViewTarget(TPSCamera);
	}
}

void ASkyrimCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASkyrimCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput	= CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	EnhancedInput->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &ASkyrimCharacter::MoveForward);
	EnhancedInput->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ASkyrimCharacter::MoveRight);
	EnhancedInput->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ASkyrimCharacter::Turn);
	EnhancedInput->BindAction(LookUpAction, ETriggerEvent::Triggered, this, &ASkyrimCharacter::LookUp);
	EnhancedInput->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASkyrimCharacter::StartJump);
	EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASkyrimCharacter::StopJump);
	EnhancedInput->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ASkyrimCharacter::Zoom);
}

void ASkyrimCharacter::MoveForward(const FInputActionValue& Value)
{
	float AxisY = Value.Get<float>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, AxisY);
}

void ASkyrimCharacter::MoveRight(const FInputActionValue& Value)
{
	float AxisX = Value.Get<float>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, AxisX);
}

void ASkyrimCharacter::Turn(const FInputActionValue& Value)
{
	float MouseX = Value.Get<float>();
	AddControllerYawInput(MouseX * MouseSensitivityX);
}

void ASkyrimCharacter::LookUp(const FInputActionValue& Value)
{
	float MouseY = Value.Get<float>();
	AddControllerPitchInput(-MouseY * MouseSensitivityY);
}

void ASkyrimCharacter::StartJump()
{
	Jump();
}

void ASkyrimCharacter::StopJump()
{
	StopJumping();
}

void ASkyrimCharacter::Zoom(const FInputActionValue& Value)
{
	float ZoomInput = Value.Get<float>();

	CurrentZoom -= ZoomInput * ZoomSpeed;
	CurrentZoom = FMath::Clamp(CurrentZoom, MinZoom, MaxZoom);
	TPSSpringArmComponent->TargetArmLength = CurrentZoom;

	FVector CurrentCameraOffset = FVector(0, CameraOffset.Y * (CurrentZoom / MaxZoom),0);
	TPSSpringArmComponent->SocketOffset	= CurrentCameraOffset;
	UpdateCameraMode();
}

void ASkyrimCharacter::UpdateCameraMode()
{
	if (CurrentCameraMode == ECameraMode::TPS && ZoomThreshold > CurrentZoom)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
		{
			PlayerController->SetViewTarget(FPSCamera);
		}
		CurrentCameraMode = ECameraMode::FPS;
	}
	if (CurrentCameraMode == ECameraMode::FPS && ZoomThreshold < CurrentZoom)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
		{
			PlayerController->SetViewTarget(TPSCamera);
		}
		CurrentCameraMode = ECameraMode::TPS;
	}
}
