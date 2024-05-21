// Fill out your copyright notice in the Description page of Project Settings.


#include "OrthographicPawn.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InteractiveArchController.h"

// Sets default values
AOrthographicPawn::AOrthographicPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	RootComponent = SceneRoot;

	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetOffset.Z = 1000;
	CameraBoom->TargetArmLength = 0;
	CameraBoom->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));


}

// Called when the game starts or when spawned
void AOrthographicPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AOrthographicPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AOrthographicPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto  MoveAction = NewObject<UInputAction>(this);

	MoveAction->ValueType = EInputActionValueType::Axis2D;


	auto LookAction = NewObject<UInputAction>(this);

	LookAction->ValueType = EInputActionValueType::Axis2D;


	auto ZoomAction = NewObject<UInputAction>(this);

	LookAction->ValueType = EInputActionValueType::Axis1D;


	MappingContext = NewObject<UInputMappingContext>(this);
	if (!MappingContext)
	{
		return;
	}


	auto& KeyW = MappingContext->MapKey(MoveAction, EKeys::W);
	auto& KeyS = MappingContext->MapKey(MoveAction, EKeys::S);
	auto& KeyA = MappingContext->MapKey(MoveAction, EKeys::A);
	auto& KeyD = MappingContext->MapKey(MoveAction, EKeys::D);

	auto SwizzleAxisModifier = NewObject<UInputModifierSwizzleAxis>(this);

	SwizzleAxisModifier->Order = EInputAxisSwizzle::YXZ;


	auto NegateModifier = NewObject<UInputModifierNegate>(this);

	KeyW.Modifiers.Add(SwizzleAxisModifier);
	KeyS.Modifiers.Add(SwizzleAxisModifier);



	KeyS.Modifiers.Add(NegateModifier);
	KeyA.Modifiers.Add(NegateModifier);



	auto NegateModifierForMouse = NewObject<UInputModifierNegate>(this);

	NegateModifierForMouse->bX = false;
	NegateModifierForMouse->bY = true;
	NegateModifierForMouse->bZ = false;


	auto& MouseAxis = MappingContext->MapKey(LookAction, EKeys::Mouse2D);


	MouseAxis.Modifiers.Add(NegateModifierForMouse);


	MappingContext->MapKey(ZoomAction, EKeys::MouseWheelAxis);

	if (auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOrthographicPawn::HandleMove);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AOrthographicPawn::HandleLook);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AOrthographicPawn::HandleZoom);

		if (auto MyController = Cast<AInteractiveArchController>(GetController()))
		{
			if (auto InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(MyController->GetLocalPlayer()))
			{
				InputSubsystem->ClearAllMappings();
				InputSubsystem->AddMappingContext(MappingContext, 0);
			}
		}
	}


}

void AOrthographicPawn::HandleMove(const FInputActionValue& Value)
{
	auto Movement = Value.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(), Movement.Y);
	AddMovementInput(GetActorRightVector(), Movement.X);

}

void AOrthographicPawn::HandleLook(const FInputActionValue& Value)
{
	auto Movement = Value.Get<FVector2D>();
	AddControllerYawInput(Movement.X);
	AddControllerPitchInput(Movement.Y);

}

void AOrthographicPawn::HandleZoom(const FInputActionValue& InputActionValue)
{
	auto Value = InputActionValue.Get<float>();


	CameraBoom->TargetOffset.Z = CameraBoom->TargetOffset.Z - Value * 25;
	CameraBoom->TargetOffset.Z = FMath::Clamp(CameraBoom->TargetOffset.Z, 300, 5000);
}
