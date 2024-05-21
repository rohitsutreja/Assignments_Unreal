// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveArchController.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include <EnhancedInputSubsystems.h>
#include "ArchMeshActor.h"
#include "Engine/StaticMeshActor.h"
#include <WallSpline.h>
#include "IsometricPawn.h"
#include "OrthographicPawn.h"
#include "PerspectivePawn.h"


void AInteractiveArchController::BeginPlay()
{
	Super::BeginPlay();

	// To have starting wall actor by default.
	CurrentSplineActor = NewObject<AWallSpline>(this);
	SplineArray.Add(CurrentSplineActor);


	if (SelectionWidgetClass)
	{
		SelectionWidget = CreateWidget<USelectionWidget>(this, SelectionWidgetClass);
	}

	if (WallSplineWidgetClass)
	{
		WallSplineWidget = CreateWidget<UUserWidget>(this, WallSplineWidgetClass);
	}

	if (SwitchWidgetClass)
	{
		SwitchWidget = CreateWidget<USwitchWidget>(this, SwitchWidgetClass);
		SwitchWidget->ViewComboBox->OnSelectionChanged.AddDynamic(this, &AInteractiveArchController::OnSelectionChanged);

	}

	SelectionWidget->MeshSelectionScrollBox->OnMeshThumbnailSelectedEvent.BindUObject(this, &AInteractiveArchController::HandleMeshThumbnailSelected);
	SelectionWidget->TextureSelectionScrollBox->OnTextureThumbnailSelectedEvent.BindUObject(this, &AInteractiveArchController::HandleTextureThumbnailSelected);
	SelectionWidget->MaterialSelectionScrollBox->OnMaterialThumbnailSelectedEvent.BindUObject(this, &AInteractiveArchController::HandleMaterialThumbnailSelected);



	if (!SelectionWidget->IsInViewport()) SelectionWidget->AddToViewport();

	if (!SwitchWidget->IsInViewport()) SwitchWidget->AddToViewport(7);

	SelectionWidget->SetVisibility(ESlateVisibility::Collapsed);


	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;


	if (auto SpawnedActor = GetWorld()->SpawnActor<APerspectivePawn>(APerspectivePawn::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params)) {
		SpawnedActor->SetActorLocation({ 0,0,100 });
		Possess(SpawnedActor);
	}

	bShowMouseCursor = true;

}

//This will add spline point to spline points array of current spline actor(Current Wall)
void AInteractiveArchController::HandleLeftClickForWall(const FInputActionValue& value) {
	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_WorldDynamic, true, HitResult);

	if (HitResult.bBlockingHit)
	{
		if (CurrentSplineActor) {
			CurrentSplineActor->AddSplinePoint(HitResult.Location);
			CurrentSplineActor->GenerateSpline();
		}
	}
}

//This will start a new wall by creating new spline actor with its own spline point array. It will only start new wall if there is at least one wall segment(Two Spline Points) in current wall(Spline Actor)
void AInteractiveArchController::SaveCurrentWall() {
	if (CurrentSplineActor) {
		if (CurrentSplineActor->HasAnySegment()) {

			//Will broadcast the message so UI widget BP can display notification.
			SendMessageToUIDelegate.Broadcast("Wall Construction Completed");
			CurrentSplineActor = NewObject<AWallSpline>(this);
			SplineArray.Add(CurrentSplineActor);
		}
	}

}

//This will delete current spline wall(Spline Actor) and move control back to previous spline wall, so we can continue drawing in previous wall where we left.
void AInteractiveArchController::HandleDelete(const FInputActionValue& value) {
	if (CurrentSplineActor) {

		CurrentSplineActor->DestroyWall();
		CurrentSplineActor->RemoveAllSplinePoints();

		//Remove current one from array and destroy it, then set previous one to current spline actor, except its the last one, becuase we want one default spline actor.
		if (!SplineArray.IsEmpty()) {
			if (SplineArray.Num() > 1)
			{
				SplineArray.Pop();
				CurrentSplineActor->Destroy();
				CurrentSplineActor = SplineArray[SplineArray.Num() - 1];

				//Will broadcast the message so UI widget BP can display notification.
				SendMessageToUIDelegate.Broadcast("Current Wall Deleted");
			}
		}
	}
}

//This will destroy all the spline actors, and set current actor(Spline wall) to default new.
void AInteractiveArchController::DeleteAllWalls()
{


	if (!SplineArray.IsEmpty()) {

		for (int i{}; i < SplineArray.Num(); i++) {
			if (SplineArray[i]) {
				SplineArray[i]->DestroyWall();
				SplineArray[i]->Destroy();
				SplineArray[i] = nullptr;
			}
		}

		SplineArray.Empty();


		for (int i{}; i < ArchMeshActorsArray.Num(); i++) {
			if (ArchMeshActorsArray[i]) {
				ArchMeshActorsArray[i]->Destroy();
				ArchMeshActorsArray[i] = nullptr;
			}
		}

		ArchMeshActorsArray.Empty();

		//Will broadcast the message so UI widget BP can display notification.
		SendMessageToUIDelegate.Broadcast("Display Cleared");

		CurrentSplineActor = nullptr;

		//To create default new current spline actor, because we deleted all.
		CurrentSplineActor = NewObject<AWallSpline>(this);

		SplineArray.Add(CurrentSplineActor);
	}
}

//This will delete last spline point and hence delete Last wall ssegment created in current spline wall(Spline Actor). It Works as an undo for current wall.
void AInteractiveArchController::HandleBackSpace(const FInputActionValue& value) {
	CurrentSplineActor->RemoveLastSplinePoint();
	CurrentSplineActor->GenerateSpline();
}


void AInteractiveArchController::SetupInputComponent()
{
	Super::SetupInputComponent();

	SetUpInputForScrollBar();
}

//Attach input mapping for Wall spline mode
void AInteractiveArchController::SetUpInputForWallSpline()
{
	auto LeftClickActionForWall = NewObject<UInputAction>(this);
	LeftClickActionForWall->ValueType = EInputActionValueType::Boolean;

	auto RightClickActionForWall = NewObject<UInputAction>(this);
	RightClickActionForWall->ValueType = EInputActionValueType::Boolean;

	auto DeleteWallSegmentAction = NewObject<UInputAction>(this);
	DeleteWallSegmentAction->ValueType = EInputActionValueType::Boolean;

	auto DeleteWallAction = NewObject<UInputAction>(this);
	DeleteWallAction->ValueType = EInputActionValueType::Boolean;


	MappingContextForWall = NewObject<UInputMappingContext>(this);

	MappingContextForWall->MapKey(LeftClickActionForWall, EKeys::LeftMouseButton);
	MappingContextForWall->MapKey(RightClickActionForWall, EKeys::RightMouseButton);
	MappingContextForWall->MapKey(DeleteWallAction, EKeys::Delete);
	MappingContextForWall->MapKey(DeleteWallSegmentAction, EKeys::BackSpace);


	if (auto EIC = Cast<UEnhancedInputComponent>(InputComponent)) {
		if (LeftClickActionForWall) {
			EIC->BindAction(LeftClickActionForWall, ETriggerEvent::Completed, this, &AInteractiveArchController::HandleLeftClickForWall);
		}

		if (DeleteWallSegmentAction) {
			EIC->BindAction(DeleteWallSegmentAction, ETriggerEvent::Completed, this, &AInteractiveArchController::HandleBackSpace);
		}

		if (DeleteWallAction) {
			EIC->BindAction(DeleteWallAction, ETriggerEvent::Completed, this, &AInteractiveArchController::HandleDelete);
		}

	}

	if (auto InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
		InputSubsystem->AddMappingContext(MappingContextForWall, 0);
	}
}

//Attach input mappings for shape spawns mode
void AInteractiveArchController::SetUpInputForScrollBar()
{
	const auto IA_LeftClick = NewObject<UInputAction>(this);
	IA_LeftClick->ValueType = EInputActionValueType::Boolean;

	const auto IA_Tab = NewObject<UInputAction>(this);
	IA_LeftClick->ValueType = EInputActionValueType::Boolean;

	MappingContextForScrollBar = NewObject<UInputMappingContext>(this);

	MappingContextForScrollBar->MapKey(IA_LeftClick, EKeys::LeftMouseButton);
	MappingContextForScrollBar->MapKey(IA_Tab, EKeys::Tab);

	if (auto const EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
		EnhancedInputComponent->BindAction(IA_LeftClick, ETriggerEvent::Completed, this, &AInteractiveArchController::HandleLeftClick);

		EnhancedInputComponent->BindAction(IA_Tab, ETriggerEvent::Completed, this, &AInteractiveArchController::HandleTab);
	}

	if (auto const InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
		InputSubsystem->AddMappingContext(MappingContextForScrollBar, 0);
	}
}

//HAndle left click input for shape spawn mode
void AInteractiveArchController::HandleLeftClick()
{
	if (FVector StartLocation, WorldDirection; DeprojectMousePositionToWorld(StartLocation, WorldDirection))
	{
		auto EndLocation = StartLocation + WorldDirection * 10000;

		FCollisionQueryParams Params;
		Params.bTraceComplex = true;

		if (FHitResult HitResult; GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params))
		{

			//check if we are hitting already spawned mesh or floor
			if (auto HitActor = Cast<AArchMeshActor>(HitResult.GetActor()))
			{
				CurrentlySelectedActor = HitActor;
				SelectionWidget->SetVisibility(ESlateVisibility::Visible);
				SelectionWidget->MaterialSelectionBox->SetVisibility(ESlateVisibility::Visible);
				SelectionWidget->TextureSelectionBox->SetVisibility(ESlateVisibility::Visible);
				
				if(ViewType == EViewType::Isometric)
				{
					GetPawn()->SetActorLocation(CurrentlySelectedActor->GetActorLocation() + FVector(0, 0, 300));
				}

				HitLocation = CurrentlySelectedActor->GetActorLocation();
			}
			else
			{
				HitLocation = HitResult.Location;
				SelectionWidget->SetVisibility(ESlateVisibility::Visible);
				SelectionWidget->MaterialSelectionBox->SetVisibility(ESlateVisibility::Collapsed);
				SelectionWidget->TextureSelectionBox->SetVisibility(ESlateVisibility::Collapsed);

			}
		}

	}
}

//hide selection widget on tab press
void AInteractiveArchController::HandleTab()
{
	SelectionWidget->SetVisibility(ESlateVisibility::Collapsed);
}

//SPawns actor with given mesh at last hit location of floor, only called when we hit empty floor before clicking on mesh in scrollbar
void AInteractiveArchController::SpawnActorWithMesh(UStaticMesh* Mesh)
{

	if (Mesh)
	{
		FVector const Min = Mesh->GetBoundingBox().Min;

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		HitLocation.Z = 0;

		if (AArchMeshActor* SpawnedActor = GetWorld()->SpawnActor<AArchMeshActor>(AArchMeshActor::StaticClass(), HitLocation + FVector(0, 0, -Min.Z), FRotator::ZeroRotator, Params)) {

			SpawnedActor->SetMobility(EComponentMobility::Movable);
			SpawnedActor->GetStaticMeshComponent()->SetStaticMesh(Mesh);
			SpawnedActor->SetActorRelativeScale3D({ 2,2,2 });

			CurrentlySelectedActor = SpawnedActor;
			ArchMeshActorsArray.Add(CurrentlySelectedActor);


			SelectionWidget->MaterialSelectionBox->SetVisibility(ESlateVisibility::Visible);
			SelectionWidget->TextureSelectionBox->SetVisibility(ESlateVisibility::Visible);
		}

		LastHitLocation = HitLocation;
	}

}


//Based on last hit, we either change the mesh of the selected actor or spawn new actor if empty floor is hit.
void AInteractiveArchController::HandleMeshThumbnailSelected(const FMeshData& MeshData)
{
	if (MeshData.Mesh)
	{
		if (LastHitLocation == HitLocation)
		{
			if (CurrentlySelectedActor)
			{
				CurrentlySelectedActor->ChangeMesh(MeshData.Mesh);

				//We adjust the z axis so that actor stays on top of the ground
				auto MinBoundOfNewMesh = MeshData.Mesh->GetBoundingBox().Min;
				FVector OldLoc = CurrentlySelectedActor->GetActorLocation();
				OldLoc.Z =  -MinBoundOfNewMesh.Z;

				CurrentlySelectedActor->SetActorLocation(OldLoc);
			}

		}
		else
		{
			SpawnActorWithMesh(MeshData.Mesh);
		}
	}

}

void AInteractiveArchController::HandleMaterialThumbnailSelected(const FMaterialData& MaterialData)
{
	if (CurrentlySelectedActor)
	{
		if (MaterialData.Material)
		{
			CurrentlySelectedActor->ChangeMaterial(MaterialData.Material);
		}

	}

}

void AInteractiveArchController::HandleTextureThumbnailSelected(const FTextureData& TextureData)
{
	if (CurrentlySelectedActor)
	{
		if (TextureData.Texture)
		{
			CurrentlySelectedActor->ChangeTexture(TextureData.Texture);
		}
	}

}


//Switch the modes between wall spline and shape spawn
void AInteractiveArchController::HandleSwitch()
{
	if (CurrentMode == 0)
	{
		SelectionWidget->RemoveFromParent();
		if (WallSplineWidget)
		{
			WallSplineWidget->AddToViewport();
		}
		if (auto const InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
			InputSubsystem->RemoveMappingContext(MappingContextForScrollBar);
		}
		SetUpInputForWallSpline();
		CurrentMode = 1;


	}
	else
	{

		WallSplineWidget->RemoveFromParent();
		if (SelectionWidget)
		{
			SelectionWidget->AddToViewport();
		}
		if (auto const InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
			InputSubsystem->RemoveMappingContext(MappingContextForWall);
		}
		SetUpInputForScrollBar();
		CurrentMode = 0;
	}
}

//Refresh view based on value of current view type
void AInteractiveArchController::RefreshView()
{
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	switch (ViewType)
	{
	case EViewType::Perspective:
	{
		if (auto SpawnedActor = GetWorld()->SpawnActor<APerspectivePawn>(APerspectivePawn::StaticClass(), HitLocation + FVector{ 0,0,300 }, FRotator::ZeroRotator, Params)) {
			auto Old = GetPawn();
			Possess(SpawnedActor);
			Old->Destroy();
		}
		break;
	}
	case EViewType::Orthographic:
	{
	
		if (auto SpawnedActor = GetWorld()->SpawnActor<AOrthographicPawn>(AOrthographicPawn::StaticClass(), HitLocation, FRotator::ZeroRotator, Params)) {
			auto Old = GetPawn();
			Possess(SpawnedActor);
			Old->Destroy();
		}
		break;
	}
	case EViewType::Isometric:
	{
		if (auto SpawnedActor = GetWorld()->SpawnActor<AIsometricPawn>(AIsometricPawn::StaticClass(), HitLocation + FVector{0,0,300}, FRotator::ZeroRotator, Params)) {
			auto Old = GetPawn();
			Possess(SpawnedActor);
			Old->Destroy();
		}
		break;
	}
	}
}

//Based on selection cof combo box, change the current view and refresh it.
void AInteractiveArchController::OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (SelectedItem == "Perspective")
	{
		ViewType = EViewType::Perspective;
	}
	else if (SelectedItem == "Orthographic")
	{
		ViewType = EViewType::Orthographic;
	}
	else if (SelectedItem == "Isometric")
	{
		ViewType = EViewType::Isometric;
	}


	RefreshView();


	//We have to add player controllers input mappings based on current mode, because refresh view will have cleared all mappings.
	if (CurrentMode == 0)
	{
		SetUpInputForScrollBar();
	}else
	{
		SetUpInputForWallSpline();
	}
}

