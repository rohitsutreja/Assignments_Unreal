// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SelectionArea.generated.h"


class UMeshGenerationUi;

UENUM(BlueprintType)
enum class EAreaType
{
	Sphere,
	Box
};


class UProceduralMeshComponent;

UCLASS()
class ASSIGNMENTS_API ASelectionArea : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASelectionArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	UProceduralMeshComponent* ProceduralMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* Material;

	UPROPERTY()
	EAreaType CurrentTypeOfArea = EAreaType::Box;

	void RefreshArea();
	void MoveToHitLocation();

	FVector GetBoxDimensions();
	float GetSphereRadius();
	void SetBoxDimensions(const FVector& Dimensions);
	void SetSphereRadius(float Radius);

private:
	FVector BoxDimensions;
	float SphereRadius;

	void GenerateBox(const FVector& Dimensions);
	void GenerateSphere(float Radius);
};
