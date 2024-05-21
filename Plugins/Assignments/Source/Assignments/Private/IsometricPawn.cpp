// Fill out your copyright notice in the Description page of Project Settings.


#include "IsometricPawn.h"

#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include <EnhancedInputComponent.h>

#include "EnhancedInputSubsystems.h"
#include "InteractiveArchController.h"


// Sets default values
AIsometricPawn::AIsometricPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	RootComponent = SceneRoot;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 1500;
	CameraBoom->SetRelativeRotation(FRotator(-40.f, 45.f, 0.f));
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 5.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	Camera->ProjectionMode = ECameraProjectionMode::Type::Orthographic;
	Camera->OrthoWidth = 2048.0f;
}

// Called when the game starts or when spawned
void AIsometricPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AIsometricPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator CurrentRotation = CameraBoom->GetRelativeRotation();
	FRotator TargetRotation = FRotator(CurrentRotation.Pitch, TargetAngle, CurrentRotation.Roll);
	CameraBoom->SetRelativeRotation(FMath::Lerp(CurrentRotation, TargetRotation, DeltaTime));

}

// Called to bind functionality to input
void AIsometricPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto RotationClocWiseAction = NewObject<UInputAction>(this);
	RotationClocWiseAction->ValueType = EInputActionValueType::Boolean;

	auto RotationAntiClockWiseAction = NewObject<UInputAction>(this);
	RotationAntiClockWiseAction->ValueType = EInputActionValueType::Boolean;

	auto ZoomAction = NewObject<UInputAction>(this);
	ZoomAction->ValueType = EInputActionValueType::Axis1D;

	MappingContext = NewObject<UInputMappingContext>(this);



	auto& QKey = MappingContext->MapKey(RotationClocWiseAction, EKeys::Q);

	auto& EKey = MappingContext->MapKey(RotationAntiClockWiseAction, EKeys::E);

	auto& MouseWheel = MappingContext->MapKey(ZoomAction, EKeys::MouseWheelAxis);

	if (auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(RotationClocWiseAction, ETriggerEvent::Completed, this, &AIsometricPawn::HandleRotateClockWise);
		EnhancedInputComponent->BindAction(RotationAntiClockWiseAction, ETriggerEvent::Completed, this, &AIsometricPawn::HandleRotateAntiClockWise);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AIsometricPawn::HandleZoom);

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

void AIsometricPawn::HandleZoom(const FInputActionValue& Value)
{
	auto Movement = Value.Get<float>();

	//CameraBoom->TargetArmLength = CameraBoom->TargetArmLength - Movement * 25;

	Camera->OrthoWidth = Camera->OrthoWidth - Movement * 25;

	Camera->OrthoWidth = FMath::Clamp(Camera->OrthoWidth, 1400, 5000);
	
}

void AIsometricPawn::HandleRotateClockWise()
{
	TargetAngle += 45;
}

void AIsometricPawn::HandleRotateAntiClockWise()
{
	TargetAngle -= 45;
}



