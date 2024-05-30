// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "VerticalRailActor.generated.h"


UENUM(BlueprintType)
enum class ERailType : uint8
{
	NormalPillar,
	RoundTurnedCapital,
	WindsorTurnedCapital,
	RoundedOverTop,
	PyramidTop,
	AcornCapital,
	GothicStarTop
};


USTRUCT(BlueprintType)
struct FRailDimensions
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Length{10.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Height{100.f};
};

UCLASS()
class ASSIGNMENTS_API AVerticalRailActor : public AActor
{
	GENERATED_BODY()

public:	

	AVerticalRailActor();

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Details")
	ERailType TypeOfRail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Details")
	FRailDimensions Dimensions{ 10.f,100.f };


	void CreateVerticalMesh();

	void CreateCone(float Radius, float Height, int32 NumberOfStacks, int32 NumberOfSlices, int32 SectionIndex, FVector Offset);
	void CreateCylinder(float Radius, float Height, int32 NumberOfStacks, int32 NumberOfSlices, int32 SectionIndex, FVector Offset);
	void CreateDonut(float OuterRadius, float DistanceFromOuterRadius, int NumberOfStacks, int NumberOfSlices,
	                   int32 SectionIndex, FVector Offset);

	void GenerateRoundTurnedCapital(float Length, float Width, float Height, float SphereRadius);
	void GenerateWindsorTurnedCapital(float Length, float Width, float Height, float SphereRadius);
	void GenerateRoundedOverTop(float Length, float Width, float Height);
	void GeneratePyramidTop(float Length, float Width, float Height, float PyramidHeight);
	void GenerateGothicStarTop(float Radius, float Height);
	void GenerateAcornCapital(float Length, float Width, float Height);
	void GenerateNormalPillar(float Length, float Width, float Height);

	void ApplyMaterial(UMaterialInterface* Material);

private:

	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* Mesh;

	void CreatePillar(float Length, float Width, float Height, int32 SectionIndex, FVector Offset);
	void CreateSphere(float Radius, int32 NumberOfStacks, int32 NumberOfSlices, int32 SectionIndex, FVector Offset);
	void CreatePyramid(float SideLength, float Height, int32 SectionIndex, FVector Offset);
	
	void CreateBellShape(float BaseRadius, float Height, float RimRadius, float CurvatureFactor, int32 NumberOfStacks, int32 NumberOfSlices, int32 SectionIndex, FVector Offset);
	void CreateSemiDisk(float Radius, float Width, int32 NumberOfSlices, int32 SectionIndex, FVector Offset);
	void CreateEgg(float Radius, float Height, int32 NumberOfStacks, int32 NumberOfSlices, int32 SectionIndex, FVector Offset);
};
