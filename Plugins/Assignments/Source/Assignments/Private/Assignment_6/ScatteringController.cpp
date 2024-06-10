// Fill out your copyright notice in the Description page of Project Settings.

#include "Assignment_6/ScatteringController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Assignment_6/MeshGenerator.h"


AScatteringController::AScatteringController()
{
	bShowMouseCursor = true;
}


void AScatteringController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsSelectionActorMoving)
	{
		SelectionArea->MoveToHitLocation();
	}
}


void AScatteringController::BeginPlay()
{
	Super::BeginPlay();

	UI = CreateWidget<UMeshGenerationUI>(this, UIClass);

	check(UI);

	UI->AddToViewport(0);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	SelectionArea = GetWorld()->SpawnActor<ASelectionArea>(SelectionAreaClass, FVector{ 0, 0, 0 },
		FRotator{ 0, 0, 0 },
		SpawnParams);


	FActorSpawnParameters SpawnParams2;
	SpawnParams2.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	MeshGenerator = GetWorld()->SpawnActor<AMeshGenerator>(MeshGeneratorClass, FVector{ 0, 0, 0 },
		FRotator{ 0, 0, 0 },
		SpawnParams2);


	check(SelectionArea);
	check(MeshGenerator);

	SelectionArea->SetBoxDimensions({ UI->X->GetValue(),UI->Y->GetValue(),UI->Z->GetValue() });
	SelectionArea->SetSphereRadius(UI->Radius->GetValue());

	MeshGenerator->NumberOfInstances = UI->MeshNumber->GetValue();
	MeshGenerator->SelectionArea = SelectionArea;


	UI->AreaSelectionBox->OnSelectionChanged.AddDynamic(this, &AScatteringController::HandleAreaTypeSelection);

	UI->X->OnValueChanged.AddDynamic(this, &AScatteringController::UpdateDimensions);
	UI->Y->OnValueChanged.AddDynamic(this, &AScatteringController::UpdateDimensions);
	UI->Z->OnValueChanged.AddDynamic(this, &AScatteringController::UpdateDimensions);

	UI->Radius->OnValueChanged.AddDynamic(this, &AScatteringController::UpdateRadius);

	UI->MeshNumber->OnValueChanged.AddDynamic(this, &AScatteringController::UpdateNumberOfMeshInstances);

	UI->Button->OnClicked.AddDynamic(this, &AScatteringController::HandleButtonClick);

	MeshGenerator->OnUpdateProgressBar.BindUFunction(this, FName("UpdateProgressBar"));




	SelectionArea->RefreshArea();
}

void AScatteringController::HandleAreaTypeSelection(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (SelectedItem == "Box")
	{
		SelectionArea->CurrentTypeOfArea = EAreaType::Box;

		UI->SphereRadiusBox->SetVisibility(ESlateVisibility::Collapsed);
		UI->BoxDimensionsBox->SetVisibility(ESlateVisibility::Visible);

	}
	else if (SelectedItem == "Sphere")
	{
		SelectionArea->CurrentTypeOfArea = EAreaType::Sphere;

		UI->BoxDimensionsBox->SetVisibility(ESlateVisibility::Collapsed);
		UI->SphereRadiusBox->SetVisibility(ESlateVisibility::Visible);
	}

	bIsSelectionActorMoving = true;
	SelectionArea->RefreshArea();
}


void AScatteringController::SetupInputComponent()
{
	Super::SetupInputComponent();

	auto LeftClickAction = NewObject<UInputAction>(this);

	auto InputMapping = NewObject<UInputMappingContext>(this);

	InputMapping->MapKey(LeftClickAction, EKeys::LeftMouseButton);

	if (auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{

		EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &AScatteringController::HandleLeftClick);

		if (auto InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			InputSubsystem->AddMappingContext(InputMapping, 0);
		};
	}

}

void AScatteringController::HandleLeftClick()
{
	bIsSelectionActorMoving = !bIsSelectionActorMoving;
}

void AScatteringController::UpdateDimensions(float InValue)
{
	FVector NewDimensions;

	NewDimensions.X = UI->X->GetValue();
	NewDimensions.Y = UI->Y->GetValue();
	NewDimensions.Z = UI->Z->GetValue();

	SelectionArea->SetBoxDimensions(NewDimensions);

	SelectionArea->RefreshArea();

}

void AScatteringController::UpdateRadius(float InValue)
{
	SelectionArea->SetSphereRadius(InValue);

	SelectionArea->RefreshArea();

}

void AScatteringController::UpdateProgressBar(float Percent)
{
	UI->ProgressBar->SetPercent(Percent);

	if (Percent == 0)
	{
		UI->ProgressBar->SetVisibility(ESlateVisibility::Collapsed);
		UI->Button->SetVisibility(ESlateVisibility::Visible);

	}
	else
	{
		UI->Button->SetVisibility(ESlateVisibility::Collapsed);
		UI->ProgressBar->SetVisibility(ESlateVisibility::Visible);


	}
}

void AScatteringController::HandleButtonClick()
{
	MeshGenerator->ScatterObjects();
}

void AScatteringController::UpdateNumberOfMeshInstances(float InValue)
{
	if (InValue)
	{
		MeshGenerator->NumberOfInstances = InValue;
	}
}