// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "GameFramework/SpringArmComponent.h"
#include "FirstPersonPawn.generated.h"


/**
 * 
 */
UCLASS()
class ASSIGNMENTS_1_2_API AFirstPersonPawn : public ABaseCharacter
{
	GENERATED_BODY()


public:
	AFirstPersonPawn();

	UPROPERTY(BlueprintReadOnly)
	UInputAction* ActionMove{};

	UPROPERTY(BlueprintReadOnly)
	UInputAction* ActionLook{};

	UPROPERTY(BlueprintReadOnly)
	UInputAction* ActionJump{};

	UPROPERTY(BlueprintReadOnly)
	UInputMappingContext* MappingContext{};

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void HandleMove(const FInputActionValue& FInputActionValue);

	void HandleLook(const FInputActionValue& value);

	void HandleJump();

public:
	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* SpringArm;
	
};
