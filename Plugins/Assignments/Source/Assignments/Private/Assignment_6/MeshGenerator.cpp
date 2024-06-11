#include "Assignment_6/MeshGenerator.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Async/Async.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

// Sets default values
AMeshGenerator::AMeshGenerator()
{
    PrimaryActorTick.bCanEverTick = true;
    AsyncScatterTask = nullptr;
}

// Called when the game starts or when spawned
void AMeshGenerator::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AMeshGenerator::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMeshGenerator::FinishScatter()
{
    Current = 0;

    if (AsyncScatterTask)
    {
        if (!AsyncScatterTask->IsDone())
        {
            AsyncScatterTask->EnsureCompletion();
        }

        delete AsyncScatterTask;
        AsyncScatterTask = nullptr;
    }
}

void AMeshGenerator::ClearAllInstances()
{
    for (auto& Pair : HISMComponents)
    {
        if (auto HISM = Pair.Value)
        {
            HISM->ClearInstances();
        }
    }
}

void AMeshGenerator::ScatterObjects()
{
    FinishScatter();
    ClearAllInstances();

    AsyncScatterTask = new FAsyncTask<FAsyncMeshGenerationTask>(this);
    AsyncScatterTask->StartBackgroundTask();
}

void AMeshGenerator::AddInstance(UStaticMesh* StaticMesh, const FTransform& Transform, UMaterialInterface* Material)
{
    if (!IsValid(StaticMesh) || !IsValid(Material))
    {
        return;
    }

    UHierarchicalInstancedStaticMeshComponent** HISMCPtr = HISMComponents.Find(StaticMesh);

    if (HISMCPtr && IsValid(*HISMCPtr) && (*HISMCPtr)->IsValidLowLevel())
    {
        (*HISMCPtr)->AddInstance(Transform);
        (*HISMCPtr)->SetMaterial(0, Material);
    }
    else
    {
        UHierarchicalInstancedStaticMeshComponent* NewHISMCPtr = NewObject<UHierarchicalInstancedStaticMeshComponent>(this);

        if (IsValid(NewHISMCPtr))
        {
            NewHISMCPtr->SetStaticMesh(StaticMesh);
            NewHISMCPtr->SetMaterial(0, Material);
            HISMComponents.Add(StaticMesh, NewHISMCPtr);

            NewHISMCPtr->RegisterComponentWithWorld(GetWorld());
            NewHISMCPtr->AddInstance(Transform);
        }
        else
        {
            return;
        }
    }

    if (NumberOfInstances == 1)
    {
        OnUpdateProgressBar.ExecuteIfBound(1);
    }
    else
    {
        OnUpdateProgressBar.ExecuteIfBound((Current) / (float)(NumberOfInstances - 1));
    }

    Current++;
}

void AMeshGenerator::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
 
   // NumberOfInstances = 0;

   // FinishScatter();

    Super::EndPlay(EndPlayReason);
}
