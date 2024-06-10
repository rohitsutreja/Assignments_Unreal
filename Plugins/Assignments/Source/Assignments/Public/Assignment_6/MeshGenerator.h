// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FAsyncMeshGenerationTask.h"
#include "MeshDataAsset.h"
#include "SelectionArea.h"
#include "GameFramework/Actor.h"

#include "MeshGenerator.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FUpdateProgressBar, float, Percent);

UCLASS()
class ASSIGNMENTS_API AMeshGenerator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMeshGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void FinishScatter();
	void ClearAllInstances();

	UPROPERTY()
	FUpdateProgressBar OnUpdateProgressBar;

	FAsyncTask<FAsyncMeshGenerationTask>* AsyncScatterTask = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* ProcMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<UStaticMesh*, UHierarchicalInstancedStaticMeshComponent*> HISMComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMeshDataAsset* MeshDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ASelectionArea* SelectionArea;

	int32 NumberOfInstances;

	int32 Current{ 0 };

	void ScatterObjects();

	void AddInstance(UStaticMesh* StaticMesh, const FTransform& Transform, UMaterialInterface* Material);
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
