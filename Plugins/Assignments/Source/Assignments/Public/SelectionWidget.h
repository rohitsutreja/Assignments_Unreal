// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeshSelectionScrollBox.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "SelectionWidget.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENTS_API USelectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly,VisibleDefaultsOnly, meta=(BindWidget))
	UCanvasPanel* RootCanvas;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, meta = (BindWidget))
	UMeshSelectionScrollBox* MeshSelectionScrollBox;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, meta = (BindWidget))
	UMeshSelectionScrollBox* TextureSelectionScrollBox;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, meta = (BindWidget))
	UMeshSelectionScrollBox* MaterialSelectionScrollBox;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, meta = (BindWidget))
	UBorder* MeshSelectionBox;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, meta = (BindWidget))
	UBorder* TextureSelectionBox;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, meta = (BindWidget))
	UBorder* MaterialSelectionBox;

	UPROPERTY()
	UMeshAssetManager* MeshAssetManager;



	virtual void NativeConstruct() override;
	
};
