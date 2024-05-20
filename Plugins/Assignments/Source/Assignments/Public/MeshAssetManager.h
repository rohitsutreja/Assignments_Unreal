// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MeshAssetManager.generated.h"


USTRUCT(BlueprintType)
struct FMeshData 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* Mesh;
};

USTRUCT(BlueprintType)
struct FMaterialData
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* Material;
};


USTRUCT(BlueprintType)
struct FTextureData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture* Texture;
};



UCLASS()
class ASSIGNMENTS_API UMeshAssetManager : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMeshData> MeshDataArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMaterialData> MaterialDataArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTextureData> TextureDataArray;
};
