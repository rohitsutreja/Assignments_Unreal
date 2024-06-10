// Fill out your copyright notice in the Description page of Project Settings.


#include "Assignment_6/SelectionArea.h"
#include "ProceduralMeshComponent.h"


// Sets default values
ASelectionArea::ASelectionArea()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProcMeshComponent"));
	RootComponent = ProceduralMeshComponent;

}

// Called when the game starts or when spawned
void ASelectionArea::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void ASelectionArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}


void ASelectionArea::GenerateBox(const FVector& Dimensions)
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FLinearColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;


	//Front
	Vertices.Add(FVector(0, Dimensions.Y, 0));
	Vertices.Add(FVector(0, Dimensions.Y, Dimensions.Z));
	Vertices.Add(FVector(Dimensions.X, Dimensions.Y, Dimensions.Z));
	Vertices.Add(FVector(Dimensions.X, Dimensions.Y, 0));

	//Back
	Vertices.Add(FVector(0, 0, 0));
	Vertices.Add(FVector(0, 0, Dimensions.Z));
	Vertices.Add(FVector(Dimensions.X, 0, Dimensions.Z));
	Vertices.Add(FVector(Dimensions.X, 0, 0));


	//Left
	Vertices.Add(FVector(0, 0, 0));
	Vertices.Add(FVector(0, 0, Dimensions.Z));
	Vertices.Add(FVector(0, Dimensions.Y, Dimensions.Z));
	Vertices.Add(FVector(0, Dimensions.Y, 0));

	//Right
	Vertices.Add(FVector(Dimensions.X, 0, 0));
	Vertices.Add(FVector(Dimensions.X, 0, Dimensions.Z));
	Vertices.Add(FVector(Dimensions.X, Dimensions.Y, Dimensions.Z));
	Vertices.Add(FVector(Dimensions.X, Dimensions.Y, 0));

	//Top
	Vertices.Add(FVector(0, Dimensions.Y, Dimensions.Z));
	Vertices.Add(FVector(0, 0, Dimensions.Z));
	Vertices.Add(FVector(Dimensions.X, 0, Dimensions.Z));
	Vertices.Add(FVector(Dimensions.X, Dimensions.Y, Dimensions.Z));

	//Bottom
	Vertices.Add(FVector(0, Dimensions.Y, 0));
	Vertices.Add(FVector(0, 0, 0));
	Vertices.Add(FVector(Dimensions.X, 0, 0));
	Vertices.Add(FVector(Dimensions.X, Dimensions.Y, 0));


	// triangles (two triangles per face)
	Triangles.Append({
		0, 3, 1, 1, 3, 2,
		4, 5, 7, 5, 6, 7,
		8, 11, 9, 9, 11, 10,
		12, 13, 15, 13, 14, 15,
		16, 19, 17, 17, 19, 18,
		20, 21, 23, 21, 22, 23
		});

	// normals (perpendicular to each face)

	for (int32 i{}; i < 4; i++)
	{
		Normals.Add(FVector(0, 1, 0));
	}
	for (int32 i{}; i < 4; i++)
	{
		Normals.Add(FVector(0, -1, 0));
	}
	for (int32 i{}; i < 4; i++)
	{
		Normals.Add(FVector(-1, 0, 0));
	}
	for (int32 i{}; i < 4; i++)
	{
		Normals.Add(FVector(1, 0, 0));
	}
	for (int32 i{}; i < 4; i++)
	{
		Normals.Add(FVector(0, 0, 1));
	}

	for (int32 i{}; i < 4; i++)
	{
		Normals.Add(FVector(0, 0, -1));
	}


	//UV Mapping
	for (int32 i{}; i < 3; ++i)
	{
		UVs.Add(FVector2D(0, 1));
		UVs.Add(FVector2D(0, 0));
		UVs.Add(FVector2D(1, 0));
		UVs.Add(FVector2D(1, 1));

		UVs.Add(FVector2D(0, 1));
		UVs.Add(FVector2D(0, 0));
		UVs.Add(FVector2D(-1, 0));
		UVs.Add(FVector2D(-1, 1));
	}



	ProceduralMeshComponent->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true);


}

void ASelectionArea::GenerateSphere(float Radius)
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;


	int32 NumberOfStacks = 30;
	int32 NumberOfSlices = 100;

	for (int32 Lat = 0; Lat < NumberOfStacks; ++Lat)
	{
		const float Phi = Lat * PI / (NumberOfStacks - 1);
		const float SinTheta = FMath::Sin(Phi);
		const float CosTheta = FMath::Cos(Phi);

		for (int32 Long = 0; Long < NumberOfSlices; ++Long)
		{
			const float Theta = Long * 2 * PI / (NumberOfSlices - 1);
			const float SinPhi = FMath::Sin(Theta);
			const float CosPhi = FMath::Cos(Theta);

			FVector Vertex = FVector(CosPhi * SinTheta, SinPhi * SinTheta, CosTheta) * Radius;

			Vertex = Vertex;

			Vertices.Add(Vertex);

			const FVector Normal = Vertex.GetSafeNormal();
			Normals.Add(Normal);

			const FVector2D UV = FVector2D(Theta / (2.0f * PI), Phi / PI);
			UVs.Add(UV);

			if (Lat < NumberOfStacks - 1 && Long < NumberOfSlices - 1)
			{
				const int32 Current = Lat * NumberOfSlices + Long;
				const int32 Next = Current + NumberOfSlices;

				Triangles.Add(Current);
				Triangles.Add(Current + 1);
				Triangles.Add(Next);

				Triangles.Add(Current + 1);
				Triangles.Add(Next + 1);
				Triangles.Add(Next);
			}
		}
	}




	ProceduralMeshComponent->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), Tangents, true);

}



void ASelectionArea::RefreshArea()
{
	if (CurrentTypeOfArea == EAreaType::Box)
	{
		GenerateBox(BoxDimensions);
	}
	else
	{
		GenerateSphere(SphereRadius);

	}

	ProceduralMeshComponent->SetMaterial(0, Material);

}

void ASelectionArea::MoveToHitLocation()
{

	if (FVector StartLocation, WorldDirection; GetWorld()->GetFirstPlayerController()->DeprojectMousePositionToWorld(StartLocation, WorldDirection))
	{
		FVector EndLocation = StartLocation + WorldDirection * 10000;

		FCollisionQueryParams Params;
		Params.bTraceComplex = true;
		Params.AddIgnoredActor(this);

		if (FHitResult HitResult; GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation,
			ECC_Visibility, Params))
		{

			FVector NewLocation = HitResult.Location;

			if (CurrentTypeOfArea == EAreaType::Sphere)
			{
				NewLocation.Z += SphereRadius;
			}

			SetActorLocation(NewLocation);
		}
	}



}






void ASelectionArea::SetSphereRadius(float Radius)
{
	SphereRadius = Radius;
}

void ASelectionArea::SetBoxDimensions(const FVector& Dimensions)
{
	BoxDimensions = Dimensions;
}

float ASelectionArea::GetSphereRadius()
{
	return SphereRadius;
}

FVector ASelectionArea::GetBoxDimensions()
{
	return BoxDimensions;
}


