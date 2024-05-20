// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchMeshActor.h"
#include "SelectionWidget.h"
#include "SwitchWidget.h"
#include "GameFramework/PlayerController.h"
#include "InteractiveArchController.generated.h"

struct FInputActionValue;
class AWallSpline;
class UInputMappingContext;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSendMessageToUI, FString, Data);


UENUM()
enum class EViewType: uint8 {
	Perspective,
	Orthographic,
	Isometric
};


UCLASS()
class ASSIGNMENTS_API AInteractiveArchController : public APlayerController
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FSendMessageToUI SendMessageToUIDelegate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UMeshAssetManager* MeshAssetManager;

	UPROPERTY()
	AArchMeshActor* CurrentlySelectedActor;

	UPROPERTY()
	UInputMappingContext* MappingContextForScrollBar;

	UPROPERTY()
	UInputMappingContext* MappingContextForWall;

	void BeginPlay() override;
	void SetupInputComponent() override;

	void SetUpInputForWallSpline();
	void SetUpInputForScrollBar();

	UFUNCTION()
	void HandleMeshThumbnailSelected(const FMeshData& MeshData);

	UFUNCTION()
	void HandleMaterialThumbnailSelected(const FMaterialData& MaterialData);

	UFUNCTION()
	void HandleTextureThumbnailSelected(const FTextureData& TextureData);

	void HandleLeftClick();
	void HandleTab();

	void SpawnActorWithMesh(UStaticMesh* Mesh);

	FVector HitLocation;
	FVector LastHitLocation;



	void HandleLeftClickForWall(const FInputActionValue& value);
	void HandleBackSpace(const FInputActionValue& value);
	void HandleDelete(const FInputActionValue& value);

	UFUNCTION(BlueprintCallable)
	void SaveCurrentWall();

	UFUNCTION(BlueprintCallable)
	void DeleteAllWalls();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> WallSplineWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> SwitchWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<USelectionWidget> SelectionWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	USelectionWidget* SelectionWidget;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* WallSplineWidget;

	UPROPERTY(BlueprintReadWrite)
	USwitchWidget* SwitchWidget;




	UPROPERTY()
	TArray<AWallSpline*> SplineArray;

	UPROPERTY()
	TArray<AArchMeshActor*> ArchMeshActorsArray;

	UPROPERTY(EditDefaultsOnly)
	AWallSpline* CurrentSplineActor;



	UFUNCTION(BlueprintCallable)
	void HandleSwitch();

	int CurrentMode = 0;


	UPROPERTY(BlueprintReadWrite)
	EViewType ViewType = EViewType::Perspective;

	UFUNCTION(BlueprintCallable)
	void RefreshView();

	UFUNCTION()
	void OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
};
