// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableText.h"
#include "Components/HorizontalBox.h"
#include "Components/ProgressBar.h"
#include "Components/SpinBox.h"
#include "MeshGenerationUI.generated.h"

/**
 *
 */

UCLASS()
class ASSIGNMENTS_API UMeshGenerationUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* AreaSelectionBox;



	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* BoxDimensionsBox;

	UPROPERTY(meta = (BindWidget))
	USpinBox* X;

	UPROPERTY(meta = (BindWidget))
	USpinBox* Y;

	UPROPERTY(meta = (BindWidget))
	USpinBox* Z;



	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* SphereRadiusBox;

	UPROPERTY(meta = (BindWidget))
	USpinBox* Radius;




	UPROPERTY(meta = (BindWidget))
	UBorder* MeshNumberBox;


	UPROPERTY(meta = (BindWidget))
	USpinBox* MeshNumber;




	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar;

	UPROPERTY(meta = (BindWidget))
	UButton* Button;

};
