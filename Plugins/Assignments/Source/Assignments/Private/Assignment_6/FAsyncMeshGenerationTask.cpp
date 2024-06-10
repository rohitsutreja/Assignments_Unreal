#include "Assignment_6/FAsyncMeshGenerationTask.h"
#include "Assignment_6/MeshGenerator.h"


FAsyncMeshGenerationTask::FAsyncMeshGenerationTask(AMeshGenerator* InScatterActor)
{
    ScatterActor = InScatterActor;
}

FVector FAsyncMeshGenerationTask::GetRandomPointInSphere(const FVector& Origin, float Radius)
{
    float Theta = FMath::RandRange(0.0f, 2 * PI);
    float Phi = FMath::RandRange(0.0f, PI);

    float U = FMath::RandRange(0.0f, 1.0f);
    float r = Radius * FMath::Pow(U, 1.0f / 3.0f);

    float x = r * FMath::Sin(Phi) * FMath::Cos(Theta);
    float y = r * FMath::Sin(Phi) * FMath::Sin(Theta);
    float z = r * FMath::Cos(Phi);

    FVector RandomPoint = Origin + FVector(x, y, z);

    return RandomPoint;
}

void FAsyncMeshGenerationTask::DoWork()
{


    int32 InstanceGenerated = 0;
    if (ScatterActor.IsValid())
    {

        FVector SelectAreaLocation = ScatterActor->SelectionArea->GetActorLocation();

        FVector SelectAreaBoxDimensions = ScatterActor->SelectionArea->GetBoxDimensions();

        float SelectAreaRadius = ScatterActor->SelectionArea->GetSphereRadius();



        if (ScatterActor->MeshDataAsset)
        {
            TArray<FScatterMeshData> StaticMeshes = ScatterActor->MeshDataAsset->ArrayOfMeshData;

            for (int32 i = 0; i < ScatterActor->NumberOfInstances; i++)
            {
                int32 RandomIndex = FMath::RandRange(0, StaticMeshes.Num() - 1);

                UStaticMesh* CurrentMesh = StaticMeshes[RandomIndex].StaticMesh;
                UMaterialInterface* CurrentMaterial = StaticMeshes[RandomIndex].Material;


                FVector MinRotation = StaticMeshes[RandomIndex].MinimumRotation;
                FVector MaxRotation = StaticMeshes[RandomIndex].MaximumRotation;

                float MinScale = StaticMeshes[RandomIndex].MinimumScale;
                float MaxScale = StaticMeshes[RandomIndex].MaximumScale;


                FVector Position;

                FRotator Rotation;
                Rotation.Roll = FMath::RandRange(MinRotation.X, MaxRotation.X);
                Rotation.Pitch = FMath::RandRange(MinRotation.Y, MaxRotation.Y);
                Rotation.Yaw = FMath::RandRange(MinRotation.Z, MaxRotation.Z);

                FVector Scale(FMath::RandRange(MinScale, MaxScale));

                if (ScatterActor->SelectionArea->CurrentTypeOfArea == EAreaType::Box)
                {

                    FVector Origin = SelectAreaLocation + SelectAreaBoxDimensions / 2.0f;

                    FBox BoundingBox = FBox(Origin - SelectAreaBoxDimensions / 2, Origin + SelectAreaBoxDimensions / 2);

                    Position = FMath::RandPointInBox(BoundingBox);

                }
                else
                {
                    Position = GetRandomPointInSphere(SelectAreaLocation, SelectAreaRadius);
                }


                FTransform Transform;

                Transform.SetTranslation(Position);
                Transform.SetRotation(Rotation.Quaternion());
                Transform.SetScale3D(Scale);



                // Add the instance on the game thread
                AsyncTask(ENamedThreads::GameThread, [CurrentMesh, CurrentMaterial, Transform, this]()
                    {
                        ScatterActor->AddInstance(CurrentMesh, Transform, CurrentMaterial);
                    });


                FPlatformProcess::Sleep(0.002);
            }

            AsyncTask(ENamedThreads::GameThread, [this]()
                {
                    ScatterActor->OnUpdateProgressBar.ExecuteIfBound(0);

                });
        }
    }
}
