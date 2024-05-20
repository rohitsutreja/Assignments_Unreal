// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine/DataTable.h"
#include "DataTableEditor/Private/SRowEditor.h"
#include "InputAction.h"
#include "FirstPersonPawn.h"
#include "MyPlayerController.generated.h"

/**
 *
 */

UENUM(BlueprintType)
enum class EPawnType : uint8 {
	FirstPerson,
	ThirdPerson,
	TopDown,
	None
};

USTRUCT(BlueprintType)
struct FPawnMapping : public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EPawnType TypeOfPawn;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APawn> PawnClass;
};

UCLASS()
class ASSIGNMENTS_1_2_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AMyPlayerController();


	UPROPERTY(EditDefaultsOnly, Category = "Pawn Table")
	UDataTable* PawnMappingTable;

	UPROPERTY(EditDefaultsOnly, Category = "Player Input")
	UInputAction* ToggleInput;

	UPROPERTY(EditDefaultsOnly, Category = "Player Input")
	UInputMappingContext* MappingContext;

protected:
	void SetupInputComponent() override;

	void SpawnAndPossessPawn();

	void OnPossess(APawn* aPawn) override;

	void OnUnPossess() override;


	UFUNCTION(BlueprintImplementableEvent)
	void ShowAttributes(UPawnAttributeAsset* Attributes);


	void BeginPlay() override;

private:
	int idx{};
};
