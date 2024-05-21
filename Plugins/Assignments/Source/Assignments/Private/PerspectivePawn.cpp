// Fill out your copyright notice in the Description page of Project Settings.


#include "PerspectivePawn.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InteractiveArchController.h"
#include "Components/CapsuleComponent.h"
#include "Evaluation/PreAnimatedState/MovieScenePreAnimatedCaptureSources.h"


// Sets default values
APerspectivePawn::APerspectivePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    Capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Capsule->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	Capsule->SetCollisionResponseToAllChannels(ECR_Block);

	
	Capsule->SetCapsuleHalfHeight(1); 
	Capsule->SetCapsuleRadius(1);

	RootComponent = Capsule;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 0;
	CameraBoom->TargetOffset.X = -500;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));

    bUseControllerRotationYaw = true;
    bUseControllerRotationPitch = true;
}


void APerspectivePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);


	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	check(EIC);

	MappingContext = NewObject<UInputMappingContext>(this);

	auto MoveAction = NewObject<UInputAction>(this);
	 MoveAction->ValueType = EInputActionValueType::Axis3D;
	 auto LookAction = NewObject<UInputAction>(this);
	LookAction->ValueType = EInputActionValueType::Axis2D;

	UInputModifierNegate* MoveActionModifierNegate = NewObject<UInputModifierNegate>();

	UInputModifierSwizzleAxis* SwizzleAxisModifier = NewObject<UInputModifierSwizzleAxis>();
	SwizzleAxisModifier->Order = EInputAxisSwizzle::YXZ;
	UInputModifierSwizzleAxis* SwizzleAxisModifierQE = NewObject<UInputModifierSwizzleAxis>();
	SwizzleAxisModifierQE->Order = EInputAxisSwizzle::ZYX;

	FEnhancedActionKeyMapping& MovementForwardKeyMapping = MappingContext->MapKey(MoveAction, EKeys::W);

	FEnhancedActionKeyMapping& MovementBackwardKeyMapping = MappingContext->MapKey(MoveAction, EKeys::S);
	MovementBackwardKeyMapping.Modifiers.Add(MoveActionModifierNegate);

	FEnhancedActionKeyMapping& MovementRightKeyMapping = MappingContext->MapKey(MoveAction, EKeys::D);
	MovementRightKeyMapping.Modifiers.Add(SwizzleAxisModifier);

	FEnhancedActionKeyMapping& MovementLeftKeyMapping = MappingContext->MapKey(MoveAction, EKeys::A);
	MovementLeftKeyMapping.Modifiers.Add(SwizzleAxisModifier);
	MovementLeftKeyMapping.Modifiers.Add(MoveActionModifierNegate);

	FEnhancedActionKeyMapping& MovementUpKeyMapping = MappingContext->MapKey(MoveAction, EKeys::E);
	MovementUpKeyMapping.Modifiers.Add(SwizzleAxisModifierQE);

	FEnhancedActionKeyMapping& MovementDownKeyMapping = MappingContext->MapKey(MoveAction, EKeys::Q);
	MovementDownKeyMapping.Modifiers.Add(SwizzleAxisModifierQE);
	MovementDownKeyMapping.Modifiers.Add(MoveActionModifierNegate);

	UInputModifierNegate* LookActionMouseModifierNegate = NewObject<UInputModifierNegate>();
	LookActionMouseModifierNegate->bX = false;
	LookActionMouseModifierNegate->bY = true;
	LookActionMouseModifierNegate->bZ = false;

	FEnhancedActionKeyMapping& LookActionMouseMapping = MappingContext->MapKey(LookAction, EKeys::Mouse2D);
	LookActionMouseMapping.Modifiers.Add(LookActionMouseModifierNegate);

	EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APerspectivePawn::HandleMove);
	EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &APerspectivePawn::HandleLook);

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	check(PlayerController)
		ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	Subsystem->AddMappingContext(MappingContext, 0);
}


// Called when the game starts or when spawned
void APerspectivePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void APerspectivePawn::HandleMove(const FInputActionValue& Value)
{
	auto Movement = Value.Get<FVector>();
	AddMovementInput(GetActorForwardVector(), Movement.X);
	AddMovementInput(GetActorRightVector(), Movement.Y);
	AddMovementInput(FVector::ZAxisVector, Movement.Z);

}

void APerspectivePawn::HandleLook(const FInputActionValue& Value)
{
	auto Movement = Value.Get<FVector2D>();
	AddControllerYawInput(Movement.X);
	AddControllerPitchInput(Movement.Y);
}

