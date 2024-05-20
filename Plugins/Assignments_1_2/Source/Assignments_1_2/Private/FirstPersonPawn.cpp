#include "FirstPersonPawn.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"

class UCharacterMovementComponent;
AFirstPersonPawn::AFirstPersonPawn() {
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;
}


void AFirstPersonPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	MappingContext = NewObject<UInputMappingContext>(this);

	ActionMove = NewObject<UInputAction>(this);
	ActionMove->ValueType = EInputActionValueType::Axis2D;



	UInputModifierNegate* NegateModifire = NewObject<UInputModifierNegate>(this);
	UInputModifierSwizzleAxis* SwizzleAxisModifire = NewObject<UInputModifierSwizzleAxis>(this);

	auto& WKeyMapping = MappingContext->MapKey(ActionMove, EKeys::W);
	WKeyMapping.Modifiers.Add(SwizzleAxisModifire);

	auto& SKeyMapping = MappingContext->MapKey(ActionMove, EKeys::S);
	SKeyMapping.Modifiers.Add(SwizzleAxisModifire);
	SKeyMapping.Modifiers.Add(NegateModifire);

	auto& DKeyMapping = MappingContext->MapKey(ActionMove, EKeys::D);

	auto& AKeyMapping = MappingContext->MapKey(ActionMove, EKeys::A);
	AKeyMapping.Modifiers.Add(NegateModifire);


	ActionLook = NewObject<UInputAction>(this);
	ActionLook->ValueType = EInputActionValueType::Axis2D;

	UInputModifierNegate* NegateModifireForMouse = NewObject<UInputModifierNegate>(this);
	NegateModifireForMouse->bX = false;
	NegateModifireForMouse->bY = true;
	NegateModifireForMouse->bZ = false;

	auto& MouseMapping = MappingContext->MapKey(ActionLook, EKeys::Mouse2D);
	MouseMapping.Modifiers.Add(NegateModifireForMouse);

	ActionJump = NewObject<UInputAction>(this);
	ActionJump->ValueType = EInputActionValueType::Boolean;

	MappingContext->MapKey(ActionJump, EKeys::SpaceBar);


	if (auto EIC = Cast<UEnhancedInputComponent>(InputComponent)) {
		if (ActionMove) {
			EIC->BindAction(ActionMove, ETriggerEvent::Triggered, this, &AFirstPersonPawn::HandleMove);
		}

		if (ActionLook) {
			EIC->BindAction(ActionLook, ETriggerEvent::Triggered, this, &AFirstPersonPawn::HandleLook);
		}

		if (ActionJump) {
			EIC->BindAction(ActionJump, ETriggerEvent::Triggered, this, &AFirstPersonPawn::HandleJump);
		}

		if (auto MPC = Cast<APlayerController>(GetController())) {
			if (auto InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(MPC->GetLocalPlayer())) {
				InputSubsystem->ClearAllMappings();
				InputSubsystem->AddMappingContext(MappingContext, 0);
			}
		}
	}
}

void AFirstPersonPawn::HandleMove(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation{ 0.f, Rotation.Yaw, 0.f };

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(GetActorForwardVector(), MovementVector.Y);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(GetActorRightVector(), MovementVector.X);
}

void AFirstPersonPawn::HandleLook(const FInputActionValue& value) {
	FVector2D LookAxisVector = value.Get<FVector2D>();
	if (GetController()) {
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}

}

void AFirstPersonPawn::HandleJump()
{
	Jump();
}

