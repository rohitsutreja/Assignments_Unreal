// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


AMyPlayerController::AMyPlayerController()
{
	PawnMappingTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/DataTables/PawnMapingTable.PawnMapingTable"));
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (auto EIC = Cast<UEnhancedInputComponent>(InputComponent)) {
		if (ToggleInput) {
			EIC->BindAction(ToggleInput, ETriggerEvent::Completed, this, &AMyPlayerController::SpawnAndPossessPawn);
		}
	}

	if (auto InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
		InputSubsystem->AddMappingContext(MappingContext, 0);
	}
}


void AMyPlayerController::SpawnAndPossessPawn() {
	if (PawnMappingTable)
	{
		TArray<FName> RowNames = PawnMappingTable->GetRowNames();
		FName NextRow = RowNames[idx++ % RowNames.Num()];

		auto Row = PawnMappingTable->FindRow<FPawnMapping>(NextRow, TEXT(""));

		if (Row) {
			UWorld* World = GetWorld();
			if (World)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

				FVector NewSpawnLocation{ .0f,.0f,.0f };
				FRotator NewSpawnRotation{ .0f,.0f,.0f };
				if (GetPawn()) {
					NewSpawnLocation = GetPawn()->GetActorLocation();
				}

				auto NewPawn = World->SpawnActor<APawn>(Row->PawnClass, NewSpawnLocation, NewSpawnRotation, SpawnParams);

				if (NewPawn)
				{
					auto oldPawn = GetPawn();
					Possess(NewPawn);
					if (oldPawn) {
						oldPawn->Destroy();
					}

					if (Row->TypeOfPawn == EPawnType::TopDown) {
						SetShowMouseCursor(true);
					}
					else {
						SetShowMouseCursor(false);
					}
				}
			}

		}
	}
}

void AMyPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	if (auto InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
		InputSubsystem->AddMappingContext(MappingContext, 0);
	}
	ShowAttributes(Cast<ABaseCharacter>(aPawn)->PawnAttributes);
}

void AMyPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "PRESS 'P' On your keyboard to change pawns.");
	SpawnAndPossessPawn();
}
