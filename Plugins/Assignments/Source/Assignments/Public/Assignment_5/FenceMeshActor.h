// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RailMeshMappings.h"
#include "VerticalRailActor.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "FenceMeshActor.generated.h"


class USplineMeshComponent;

USTRUCT(BlueprintType)
struct FFenceProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERailType RailType = ERailType::NormalPillar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Length{10.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Height{ 100.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Spacing{30.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMax = 0.9,ClampMin = 0))
	float LowerBeamOffset{ 0.25f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMax = 0.9, ClampMin = 0))
	float UpperBeamOffset{ 0.70f };

};



UCLASS()
class ASSIGNMENTS_API AFenceMeshActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AFenceMeshActor();

	UPROPERTY(VisibleDefaultsOnly)
	USplineComponent* SplineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fence")
	FFenceProperties FenceProperties;

	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh Details")
	UStaticMesh* SourceMeshForHorizontalBeams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Details")
	UMaterialInterface* VerticalMeshMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Details")
	UMaterialInterface* HorizontalMeshMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh Details")
	URailMeshMappings* RailMeshMappings;



	virtual void OnConstruction(const FTransform& Transform) override;


protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TArray<UStaticMeshComponent*> VerticalRailMeshComponents;

	UPROPERTY()
	TArray<USplineMeshComponent*> HorizontalBeamSplineMeshComponents;

	void ClearStaticFenceComponents();
	void AddStaticFenceComponents();
	void ReplaceStaticMeshesWithProceduralMesh();

};
