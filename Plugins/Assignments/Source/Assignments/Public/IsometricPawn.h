// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/Pawn.h"
#include "IsometricPawn.generated.h"

class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
class UFloatingPawnMovement;

UCLASS()
class ASSIGNMENTS_API AIsometricPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AIsometricPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void HandleZoom(const FInputActionValue& Value);
	void HandleRotateClockWise();
	void HandleRotateAntiClockWise();


	UPROPERTY()
	UInputAction* RotationClocWiseAction;

	UPROPERTY()
	UInputAction* RotationAntiClockWiseAction;

	UPROPERTY()
	UInputAction* ZoomAction;

	UPROPERTY()
	UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* SceneRoot;



};
