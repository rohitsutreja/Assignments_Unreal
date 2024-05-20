// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "OrthographicPawn.generated.h"

struct FInputActionValue;

UCLASS()
class ASSIGNMENTS_API AOrthographicPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AOrthographicPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void HandleZoom(const FInputActionValue& InputActionValue);
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY()
	UInputAction* MoveAction;

	UPROPERTY()
	UInputAction* LookAction;

	UPROPERTY()
	UInputAction* ZoomAction;

	UPROPERTY()
	UInputMappingContext* MappingContext;

	void HandleMove(const FInputActionValue& Value);
	void HandleLook(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY()
	UFloatingPawnMovement* MovementComponent;

	UPROPERTY()
	USceneComponent* SceneRoot;

};
