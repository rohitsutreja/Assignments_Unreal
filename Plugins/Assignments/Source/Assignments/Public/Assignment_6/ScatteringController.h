// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Assignment_6/MeshGenerationUi.h"
#include  "Assignment_6/SelectionArea.h"
#include  "Assignment_6/MeshGenerator.h"

#include "GameFramework/PlayerController.h"
#include  "ScatteringController.generated.h"

class AMeshGenerator;
/**
 *
 */
UCLASS()
class ASSIGNMENTS_API AScatteringController : public APlayerController
{
	GENERATED_BODY()

public:

	AScatteringController();

	virtual void SetupInputComponent() override;

	void HandleLeftClick();

	UFUNCTION()
	void UpdateDimensions(float InValue);

	UFUNCTION()
	void UpdateRadius(float InValue);

	UFUNCTION()
	void UpdateProgressBar(float InValue);

	UFUNCTION()
	void HandleButtonClick();

	UFUNCTION()
	void UpdateNumberOfMeshInstances(float InValue);

	void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;



	UPROPERTY()
	UMeshGenerationUI* UI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UMeshGenerationUI> UIClass;



	UPROPERTY()
	ASelectionArea* SelectionArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ASelectionArea> SelectionAreaClass;


	UPROPERTY()
	AMeshGenerator* MeshGenerator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AMeshGenerator> MeshGeneratorClass;


	bool bIsSelectionActorMoving = true;


	UFUNCTION()
	void HandleAreaTypeSelection(FString SelectedItem, ESelectInfo::Type SelectionType);
};
