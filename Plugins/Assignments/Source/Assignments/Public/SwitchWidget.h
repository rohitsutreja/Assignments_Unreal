// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Components/CanvasPanel.h>

#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "Components/ComboBoxString.h"
#include "SwitchWidget.generated.h"


class UComboBoxKey;
/**
 * 
 */
UCLASS()
class ASSIGNMENTS_API USwitchWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, meta = (BindWidget))
	UCanvasPanel* RootCanvas;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, meta = (BindWidget))
	UButton* SwitchButton;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, meta = (BindWidget))
	UComboBoxString* ViewComboBox;
};
