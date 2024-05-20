// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "PerspectivePawn.generated.h"

UCLASS()
class ASSIGNMENTS_API APerspectivePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APerspectivePawn();

protected:
	UPROPERTY()
	UInputAction* MoveAction;


	UPROPERTY()
	UInputAction* MoveInZAction;

	UPROPERTY()
	UInputAction* LookAction;

	UPROPERTY()
	UInputMappingContext* MappingContext;


	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void HandleMove(const FInputActionValue& Value);
	void HandleLook(const FInputActionValue& Value);


	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* SceneRoot;

	UPROPERTY()
	UFloatingPawnMovement* MovementComponent;


};
