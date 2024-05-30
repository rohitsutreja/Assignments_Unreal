// Fill out your copyright notice in the Description page of Project Settings.


#include "Assignment_5/FenceMeshActor.h"
#include "Components/SplineMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"


AFenceMeshActor::AFenceMeshActor()
{
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	RootComponent = SplineComponent;
}

void AFenceMeshActor::ClearStaticFenceComponents()
{
	for (auto& MeshComponent : VerticalRailMeshComponents)
	{
        if(IsValid(MeshComponent))
			MeshComponent->DestroyComponent();
	}
	VerticalRailMeshComponents.Empty();


    for (auto& MeshComponent : HorizontalBeamSplineMeshComponents)
    {
        if (IsValid(MeshComponent))
            MeshComponent->DestroyComponent();
    }
    HorizontalBeamSplineMeshComponents.Empty();
}


void AFenceMeshActor::AddStaticFenceComponents()
{
   
    const float SplineLength = SplineComponent->GetSplineLength();

    const int32 NumberOfSplinePoints = SplineComponent->GetNumberOfSplinePoints();

    for(int32 i{}; i<NumberOfSplinePoints; ++i)
    {
		 SplineComponent->SetSplinePointType(i, ESplinePointType::Linear);
    }

    const float VerticalMeshLength = FenceProperties.Length + FenceProperties.Spacing;

    const float NumberOfVerticalFences = FMath::Floor(SplineLength / VerticalMeshLength);

    for (int32 i{}; i < NumberOfVerticalFences; ++i)
    {
    
        const FVector  Location = SplineComponent->GetLocationAtDistanceAlongSpline(i * VerticalMeshLength, ESplineCoordinateSpace::Local);
        const FRotator Rotation = SplineComponent->GetRotationAtDistanceAlongSpline(i * VerticalMeshLength, ESplineCoordinateSpace::Local);

        UStaticMeshComponent* VerticalRailMesh = NewObject<UStaticMeshComponent>(this);

        VerticalRailMesh->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);

        VerticalRailMesh->SetRelativeLocationAndRotation(Location, Rotation);

        if (RailMeshMappings)
        {
            if (RailMeshMappings->RailMeshMappingMap.Contains(FenceProperties.RailType))
            {
                VerticalRailMesh->SetStaticMesh(RailMeshMappings->RailMeshMappingMap[FenceProperties.RailType]);
            }
        }


        VerticalRailMesh->SetMaterial(0, VerticalMeshMaterial );

        VerticalRailMesh->SetWorldScale3D({ FenceProperties.Length /10, FenceProperties.Length/10, FenceProperties.Height/100 });

        VerticalRailMesh->RegisterComponent();

        VerticalRailMeshComponents.Add(VerticalRailMesh);
    }



    for (int32 i{}; i < NumberOfSplinePoints - 1; ++i)
    {
        auto StartLocation = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
        auto EndLocation = SplineComponent->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::World);

        auto Tangent = UKismetMathLibrary::GetDirectionUnitVector(StartLocation, EndLocation) * 2.5;

        UMaterialInstanceDynamic* HorizontalMaterialInstance = UMaterialInstanceDynamic::Create(HorizontalMeshMaterial, this);
        int32 TileY = FenceProperties.Length / 10;
        int32 TileX = FenceProperties.Height / 10;

        HorizontalMaterialInstance->SetScalarParameterValue(TEXT("TileX"), TileX);  
        HorizontalMaterialInstance->SetScalarParameterValue(TEXT("TileY"), TileY);


        USplineMeshComponent* SplineMeshUp = NewObject<USplineMeshComponent>(this);

        SplineMeshUp->SetStartOffset({-2.5,0});
        SplineMeshUp->SetEndOffset({-2.5,0});
        SplineMeshUp->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
        SplineMeshUp->SetStartAndEnd(StartLocation - Tangent , Tangent, EndLocation + Tangent, Tangent);
        SplineMeshUp->SetRelativeLocation({0,0,FenceProperties.Height * FenceProperties.UpperBeamOffset});
        SplineMeshUp->SetStaticMesh(SourceMeshForHorizontalBeams);
        SplineMeshUp->SetMaterial(0,HorizontalMaterialInstance);
        SplineMeshUp->RegisterComponent();
        HorizontalBeamSplineMeshComponents.Add(SplineMeshUp);


        USplineMeshComponent* SplineMeshDown = NewObject<USplineMeshComponent>(this);

        SplineMeshDown->SetStartOffset({ -2.5,0  });
        SplineMeshDown->SetEndOffset({ -2.5,0 });
        SplineMeshDown->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
        SplineMeshDown->SetStartAndEnd(StartLocation - Tangent, Tangent, EndLocation + Tangent, Tangent);
        SplineMeshDown->SetRelativeLocation({ 0,0,FenceProperties.Height * FenceProperties.LowerBeamOffset });
    	SplineMeshDown->SetStaticMesh(SourceMeshForHorizontalBeams);
        SplineMeshDown->SetMaterial(0, HorizontalMaterialInstance);
        SplineMeshDown->RegisterComponent();
        HorizontalBeamSplineMeshComponents.Add(SplineMeshDown);
    }

}

void AFenceMeshActor::ReplaceStaticMeshesWithProceduralMesh()
{
    for (auto& MeshComponent : VerticalRailMeshComponents)
    {
        
       
            FVector Location = MeshComponent->GetComponentLocation();
            FRotator Rotation = MeshComponent->GetComponentRotation();

            MeshComponent->DestroyComponent();

            AVerticalRailActor* SpawnedRail{};

	        if(RailMeshMappings)
	        {
		        if(RailMeshMappings->RailClassMappingMap.Contains(FenceProperties.RailType))
		        {
	                SpawnedRail =  GetWorld()->SpawnActor<AVerticalRailActor>(RailMeshMappings->RailClassMappingMap[FenceProperties.RailType], Location, Rotation);
		        }
	            else
	            {
	                SpawnedRail = GetWorld()->SpawnActor<AVerticalRailActor>(AVerticalRailActor::StaticClass(), Location, Rotation);
	            }
	        }

            SpawnedRail->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

            SpawnedRail->Dimensions = {FenceProperties.Length, FenceProperties.Height};

            SpawnedRail->ApplyMaterial(VerticalMeshMaterial);

            SpawnedRail->CreateVerticalMesh();
      
        
    }

    VerticalRailMeshComponents.Empty();

    for (int32 i{}; i < SplineComponent->GetNumberOfSplinePoints() - 1; ++i)
    {
        auto StartLocation = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
        auto StartRotation = SplineComponent->GetRotationAtSplinePoint(i, ESplineCoordinateSpace::Local);
        auto EndLocation = SplineComponent->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);

        auto SpawnedRailUp = GetWorld()->SpawnActor<AVerticalRailActor>(AVerticalRailActor::StaticClass(), StartLocation, StartRotation);

        if(i == 0)
        {
            SpawnedRailUp->AddActorLocalOffset({ -FenceProperties.Length * 2,-2.5,FenceProperties.Height * FenceProperties.UpperBeamOffset });
            SpawnedRailUp->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
            SpawnedRailUp->GenerateNormalPillar((EndLocation - StartLocation).Length() + FenceProperties.Length * 2, 2.5, 10);
        }
        else
        {
            SpawnedRailUp->AddActorLocalOffset({ 0, -2.5, FenceProperties.Height * FenceProperties.UpperBeamOffset });
            SpawnedRailUp->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
            SpawnedRailUp->GenerateNormalPillar((EndLocation - StartLocation).Length() + 2.5, 2.5, 10);
        }


        auto SpawnedRailBottom = GetWorld()->SpawnActor<AVerticalRailActor>(AVerticalRailActor::StaticClass(), StartLocation, StartRotation);
        


        if (i == 0)
        {
            SpawnedRailBottom->AddActorLocalOffset({ -FenceProperties.Length * 2,-2.5,FenceProperties.Height * FenceProperties.LowerBeamOffset });
            SpawnedRailBottom->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
            SpawnedRailBottom->GenerateNormalPillar((EndLocation - StartLocation).Length() + FenceProperties.Length * 2, 2.5, 10);

        }
        else
        {
            SpawnedRailBottom->AddActorLocalOffset({ 0,-2.5,FenceProperties.Height * FenceProperties.LowerBeamOffset });
            SpawnedRailBottom->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
            SpawnedRailBottom->GenerateNormalPillar((EndLocation - StartLocation).Length() + 2.5, 2.5, 10);
        }

        UMaterialInstanceDynamic* HorizontalMaterialInstance = UMaterialInstanceDynamic::Create(HorizontalMeshMaterial, this);
        int32 TileY = FenceProperties.Length / 10;
        int32 TileX = FenceProperties.Height / 10;

        HorizontalMaterialInstance->SetScalarParameterValue(TEXT("TileX"), TileX);
        HorizontalMaterialInstance->SetScalarParameterValue(TEXT("TileY"), TileY);

        SpawnedRailBottom->ApplyMaterial(HorizontalMaterialInstance);
        SpawnedRailUp->ApplyMaterial (HorizontalMaterialInstance);


    }

    ClearStaticFenceComponents();

}

void AFenceMeshActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

    ClearStaticFenceComponents();
	AddStaticFenceComponents();
}


void AFenceMeshActor::BeginPlay()
{
	Super::BeginPlay();
    ReplaceStaticMeshesWithProceduralMesh();
}
