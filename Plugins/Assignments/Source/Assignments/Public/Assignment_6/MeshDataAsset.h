// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MeshDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FScatterMeshData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinimumScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaximumScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector MinimumRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector MaximumRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* Material;
};


UCLASS()
class ASSIGNMENTS_API UMeshDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FScatterMeshData> ArrayOfMeshData;

};
