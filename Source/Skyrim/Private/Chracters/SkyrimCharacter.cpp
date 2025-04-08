#include "Chracters/SkyrimCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Chracters/CharacterCamera.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Inventory/InventoryComponent.h"
#include "Items/Interfaces/ItemInteractor.h"
#include "Utility/InputHelper.h"
#include "Widgets/Pickups/Pickup.h"

ASkyrimCharacter::ASkyrimCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

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
	GetCharacterMovement()->RotationRate = FRotator(0, 240, 0);
	
	/* Init Sensitivity (마우스 감도 설정) */
	MouseSensitivityX = 0.7f;
	MouseSensitivityY = 0.7f;

	/* Init Interaction Item (상호작용 아이템 초기화) */
	ItemBoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("ItemCollisionBoxComponent"));
	ItemBoxCollisionComponent->SetupAttachment(GetRootComponent());

	/* Init Inventory (인벤토리 초기화) */
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	InventoryComponent->OnAddItem.AddUObject(this, &ASkyrimCharacter::HandleAddItem);
}

void ASkyrimCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ItemBoxCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ASkyrimCharacter::OnOverlapBeginItemBox);
	ItemBoxCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ASkyrimCharacter::OnOverlapEndItemBox);
}

void ASkyrimCharacter::BeginPlay()
{
	Super::BeginPlay(); 

	/* Apply Mapping Context */
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		UInputHelper::ApplyMappingContext(PlayerController, PlayerMappingContext);
	}

	/* Init Main Camera (메인 카메라 초기화) */
	if (GetWorld())
	{
		TPSCamera = TPSCameraActorComponent->GetChildActor();
		FPSCamera = FPSCameraActorComponent->GetChildActor();
		GetWorld()->GetFirstPlayerController()->SetViewTarget(TPSCamera);
	}
	

	/* Init Interaction Item (상호작용 초기화) */
	if (GetWorld() && PickupWidgetClass)
	{
		PickupActor = GetWorld()->SpawnActor<APickup>(APickup::StaticClass());
		if (PickupActor)
		{
			PickupActor->SetOwner(this);
			PickupActor->CreatePickupWidget(PickupWidgetClass);
		}
	}

	FWorldContext* WorldContext = GEngine->GetWorldContextFromWorld(GetWorld());
	if (WorldContext)
	{
		FString CurrentMapName = WorldContext->World()->GetMapName();
		UE_LOG(LogTemp, Log, TEXT("Current Map Name: %s"), *CurrentMapName);
	}
}

void ASkyrimCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UpdatePickupWidget();
}

#pragma region EnhancedInput
void ASkyrimCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	// Player Action
	EnhancedInput->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &ASkyrimCharacter::MoveForward);
	EnhancedInput->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ASkyrimCharacter::MoveRight);
	EnhancedInput->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ASkyrimCharacter::Turn);
	EnhancedInput->BindAction(LookUpAction, ETriggerEvent::Triggered, this, &ASkyrimCharacter::LookUp);
	EnhancedInput->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASkyrimCharacter::StartJump);
	EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASkyrimCharacter::StopJump);
	EnhancedInput->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ASkyrimCharacter::Zoom);
	EnhancedInput->BindAction(InteractionAction, ETriggerEvent::Triggered, this, &ASkyrimCharacter::ItemInteract);
	EnhancedInput->BindAction(InventoryAction, ETriggerEvent::Triggered, this, &ASkyrimCharacter::ToggleInventory);
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

void ASkyrimCharacter::ItemInteract()
{
	// 1. 상호작용할 아이템이 있는지 확인  
	if (ClosestItem == nullptr)
		return;

	if (InventoryComponent == nullptr)
		return;

	InventoryComponent->AddItem(ClosestItem);
}

void ASkyrimCharacter::ToggleInventory()
{
	if (InventoryComponent == nullptr)
		return;
	
	InventoryComponent->ToggleInventory(PlayerMappingContext);
}

#pragma endregion

void ASkyrimCharacter::UpdateCameraMode()
{
	if (CurrentCameraMode == ECameraMode::TPS && ZoomThreshold > CurrentZoom)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
		{
			bUseControllerRotationYaw = true;
			PlayerController->SetViewTarget(FPSCamera);
		}
		CurrentCameraMode = ECameraMode::FPS;
	}
	if (CurrentCameraMode == ECameraMode::FPS && ZoomThreshold < CurrentZoom)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
		{
			bUseControllerRotationYaw = false;
			PlayerController->SetViewTarget(TPSCamera);
		}
		CurrentCameraMode = ECameraMode::TPS;
	}
}

#pragma region ItemBoxCollision
void ASkyrimCharacter::OnOverlapBeginItemBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 1. 오버렙 대상이 아이템인지 확인
	IItemInteractor* Item = Cast<IItemInteractor>(OtherActor);
	if (Item == nullptr)
		return;

	if (PickupActor == nullptr)
		return;
	
	ClosestItem = OtherActor;
	OverlappingItems.Add(OtherActor);
	PickupActor->Activate(OtherActor);
}

void ASkyrimCharacter::OnOverlapEndItemBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 1. 아이템인지 확인
	if (OverlappingItems.Find(OtherActor) == nullptr)
		return;

	// 2. 오버렙 아이템 목록에서 삭제
	OverlappingItems.Remove(OtherActor);

	// 3. 더이상 오버렙된 아이템이 없을 경우 
	if (PickupActor && OverlappingItems.Num() == 0)
	{
		ClosestItem = nullptr;
		PickupActor->Deactivate();
	}
}
#pragma endregion

void ASkyrimCharacter::UpdatePickupWidget()
{
	// 1. 오버렙된 아이템 개수 확인
	if (OverlappingItems.Num() == 0)
		return;

	// 2. 오버렙된 아이템들 중 플레이어와 가까운 아이템 확인
	AActor* Item = nullptr;
	float MinDistance = INFINITY;
	for (AActor* OverlappingItem : OverlappingItems)
	{
		float ItemDistance = FVector::DistSquaredXY(GetActorLocation(), OverlappingItem->GetActorLocation());

		if (ItemDistance < MinDistance)
		{
			Item = OverlappingItem;
			MinDistance = ItemDistance;
		}
	}

	// 3. Item 유효성 확인
	if (Item == nullptr)
		return;

	// 4. Item 현재 가리키는 액터와 동일한지 확인
	if (Item == ClosestItem)
		return;

	// 5. PickupActor 활성화
	ClosestItem = Item;
	PickupActor->Activate(ClosestItem);
}

void ASkyrimCharacter::HandleAddItem(FName Key, AActor* Item)
{
	if (IItemInteractor* ItemInteractor = Cast<IItemInteractor>(Item))
	{
		ItemInteractor->PickupItem();
	}
}
