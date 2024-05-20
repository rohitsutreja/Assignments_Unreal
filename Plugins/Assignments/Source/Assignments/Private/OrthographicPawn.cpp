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


// Called to bind functionality to input
void AOrthographicPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Create MoveAction and LookAction
    MoveAction = NewObject<UInputAction>(this);
    if (MoveAction)
    {
        MoveAction->ValueType = EInputActionValueType::Axis2D;
    }

    LookAction = NewObject<UInputAction>(this);
    if (LookAction)
    {
        LookAction->ValueType = EInputActionValueType::Axis2D;
    }

    ZoomAction = NewObject<UInputAction>(this);
    if (LookAction)
    {
        LookAction->ValueType = EInputActionValueType::Axis1D;
    }

    // Create MappingContext
    MappingContext = NewObject<UInputMappingContext>(this);
    if (!MappingContext)
    {
        return;
    }

    // Map keys for MoveAction
    auto& KeyW = MappingContext->MapKey(MoveAction, EKeys::W);
    auto& KeyS = MappingContext->MapKey(MoveAction, EKeys::S);
    auto& KeyA = MappingContext->MapKey(MoveAction, EKeys::A);
    auto& KeyD = MappingContext->MapKey(MoveAction, EKeys::D);


    // Create and configure modifiers
    auto SwizzleAxisModifier = NewObject<UInputModifierSwizzleAxis>(this);
    if (SwizzleAxisModifier)
    {
        SwizzleAxisModifier->Order = EInputAxisSwizzle::YXZ;
    }

    auto NegateModifier = NewObject<UInputModifierNegate>(this);

    // Apply modifiers to keys for movement in the correct directions
    if (SwizzleAxisModifier)
    {
        KeyW.Modifiers.Add(SwizzleAxisModifier);
        KeyS.Modifiers.Add(SwizzleAxisModifier);
    }

    if (NegateModifier)
    {
        KeyS.Modifiers.Add(NegateModifier);
        KeyA.Modifiers.Add(NegateModifier);
    }


    // Create and configure mouse look negation modifier
    auto NegateModifierForMouse = NewObject<UInputModifierNegate>(this);
    if (NegateModifierForMouse)
    {
        NegateModifierForMouse->bX = false;
        NegateModifierForMouse->bY = true;
        NegateModifierForMouse->bZ = false;
    }

    // Map and configure mouse axis for LookAction
    auto& MouseAxis = MappingContext->MapKey(LookAction, EKeys::Mouse2D);

    if (NegateModifierForMouse)
    {
        MouseAxis.Modifiers.Add(NegateModifierForMouse);
    }

    MappingContext->MapKey(ZoomAction, EKeys::MouseWheelAxis);


    // Bind actions if the EnhancedInputComponent is available
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
    CameraBoom->TargetOffset.Z = CameraBoom->TargetOffset.Z - Value* 25; 
}
