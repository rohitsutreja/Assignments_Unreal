// Fill out your copyright notice in the Description page of Project Settings.

#include "Assignment_5/VerticalRailActor.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"


AVerticalRailActor::AVerticalRailActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = Mesh;
}

void AVerticalRailActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	CreateVerticalMesh();
}

void AVerticalRailActor::CreateVerticalMesh()
{
	Mesh->ClearAllMeshSections();

	switch (TypeOfRail)
	{
	case ERailType::NormalPillar:
	{
		GenerateNormalPillar(Dimensions.Length, Dimensions.Length, Dimensions.Height);
		break;
	}
	case ERailType::WindsorTurnedCapital:
	{
		GenerateWindsorTurnedCapital(Dimensions.Length, Dimensions.Length, Dimensions.Height, Dimensions.Length / 2.f);
		break;
	}
	case ERailType::RoundTurnedCapital:
	{
		GenerateRoundTurnedCapital(Dimensions.Length, Dimensions.Length, Dimensions.Height, Dimensions.Length / 2.f);
		break;
	}
	case ERailType::AcornCapital:
	{

		GenerateAcornCapital(Dimensions.Length, Dimensions.Length, Dimensions.Height);
		break;
	}
	case ERailType::RoundedOverTop:
	{
		GenerateRoundedOverTop(Dimensions.Length, Dimensions.Length, Dimensions.Height);
		break;
	}
	case ERailType::PyramidTop:
	{
		GeneratePyramidTop(Dimensions.Length, Dimensions.Length, Dimensions.Height, Dimensions.Length / 2);
		break;
	}
	case ERailType::GothicStarTop:
	{
		GenerateGothicStarTop(Dimensions.Length / 2, Dimensions.Height);
		break;
	}
	}
}

void AVerticalRailActor::GenerateWindsorTurnedCapital(float Length, float Width, float Height, float SphereRadius)
{
	const float BellHeight = SphereRadius / 2.f;

	CreatePillar(Length, Width, Height, 0, FVector::ZeroVector);

	CreateBellShape(Length / 2, BellHeight, Length / 3, 1, 10, 30, 1, FVector(Length / 2, Width / 2, Height + BellHeight));

	CreateSphere(SphereRadius, 10, 20, 2, FVector(Length / 2, Width / 2, Height + SphereRadius + BellHeight * 2 - BellHeight / 1.8f));

	CreateBellShape(Length / 4, BellHeight, 0.5f, 1, 10, 30, 3, FVector(Length / 2, Width / 2, Height + SphereRadius * 2 + BellHeight * 2));

}

void AVerticalRailActor::GenerateRoundTurnedCapital(float Length, float Width, float Height, float SphereRadius)
{
	const float BellHeight = SphereRadius / 2.f;

	CreatePillar(Length, Width, Height, 0.f, FVector::ZeroVector);
	CreateBellShape(Length / 2.f, BellHeight, Length / 3.f, 1.f, 10, 30, 1, FVector(Length / 2.f, Width / 2.f, Height + BellHeight));
	CreateSphere(SphereRadius, 20, 30, 2, FVector(Length / 2.f, Width / 2.f, Height + SphereRadius + BellHeight * 2.f - BellHeight / 1.8f));


}

void AVerticalRailActor::GenerateAcornCapital(float Length, float Width, float Height)
{
	const float BellHeight = Length / 5.f;

	CreatePillar(Length, Width, Height, 0, FVector::ZeroVector);
	CreateBellShape(Length / 2, BellHeight, Length / 3, 1, 10, 20, 1, FVector(Length / 2, Width / 2, Height + BellHeight));
	CreateEgg(Length / 2, Length, 30, 20, 2, FVector(Length / 2, Width / 2, Height + Length));
	CreateDonut(Length / 2 + 1.0f, 1.f, 10, 50, 3, {Length/2, Width/2, Height + Length});

}

void AVerticalRailActor::GenerateNormalPillar(float Length, float Width, float Height)
{
	CreatePillar(Length, Width, Height, 0, FVector::ZeroVector);
}

void AVerticalRailActor::GenerateRoundedOverTop(float Length, float Width, float Height)
{
	CreatePillar(Length, Width, Height, 0, FVector::ZeroVector);
	CreatePillar(Length * 0.9f, Width * 0.9f, Height * 0.01f, 1, FVector(0.5f, 0.5f, Height));
	CreateSemiDisk(Length / 2, Width, 30, 2, FVector(Length / 2, Width / 2, Height + Height * 0.01f));
}

void AVerticalRailActor::GeneratePyramidTop(float Length, float Width, float Height, float PyramidHeight)
{
	CreatePillar(Length, Width, Height, 0, FVector::ZeroVector);
	CreatePillar(Length * 0.9f, Width * 0.9f, Height * 0.01f, 1, FVector(0.5f, 0.5f, Height));
	CreatePillar(Length, Width, PyramidHeight * 1.5f, 2, FVector(0, 0, Height + Height * 0.01f));
	CreatePyramid(Length, PyramidHeight, 3, FVector(0, 0, Height + Height * 0.01f + PyramidHeight * 1.5f));
}

void AVerticalRailActor::GenerateGothicStarTop(float Radius, float Height)
{
	CreateCylinder(Radius, Height, 20, 20, 0, FVector::ZeroVector);
	CreateCylinder(Radius * 0.8, Height/50, 2, 20, 1, FVector{0,0,Height});
	CreateCone(Radius,Height/7, 10, 50, 2, FVector{0,0,Height + Height/50});

};



void AVerticalRailActor::CreatePillar(float Length, float Width, float Height, int32 SectionIndex, FVector Offset)
{

	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	const TArray<FLinearColor> VertexColors;
	const TArray<FProcMeshTangent> Tangents;


	//Front
	Vertices.Add(FVector(0, Width, 0) + Offset);
	Vertices.Add(FVector(0, Width, Height) + Offset);
	Vertices.Add(FVector(Length, Width, Height) + Offset);
	Vertices.Add(FVector(Length, Width, 0) + Offset);

	//Back
	Vertices.Add(FVector(0, 0, 0) + Offset);
	Vertices.Add(FVector(0, 0, Height) + Offset);
	Vertices.Add(FVector(Length, 0, Height) + Offset);
	Vertices.Add(FVector(Length, 0, 0) + Offset);


	//Left
	Vertices.Add(FVector(0, 0, 0) + Offset);
	Vertices.Add(FVector(0, 0, Height) + Offset);
	Vertices.Add(FVector(0, Width, Height) + Offset);
	Vertices.Add(FVector(0, Width, 0) + Offset);

	//Right
	Vertices.Add(FVector(Length, 0, 0) + Offset);
	Vertices.Add(FVector(Length, 0, Height) + Offset);
	Vertices.Add(FVector(Length, Width, Height) + Offset);
	Vertices.Add(FVector(Length, Width, 0) + Offset);

	//Top
	Vertices.Add(FVector(0, Width, Height) + Offset);
	Vertices.Add(FVector(0, 0, Height) + Offset);
	Vertices.Add(FVector(Length, 0, Height) + Offset);
	Vertices.Add(FVector(Length, Width, Height) + Offset);

	//Bottom
	Vertices.Add(FVector(0, Width, 0) + Offset);
	Vertices.Add(FVector(0, 0, 0) + Offset);
	Vertices.Add(FVector(Length, 0, 0) + Offset);
	Vertices.Add(FVector(Length, Width, 0) + Offset);


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



	Mesh->CreateMeshSection_LinearColor(SectionIndex, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true);



}

void AVerticalRailActor::ApplyMaterial(UMaterialInterface* Material1)
{

	for (int32 i{}; i < Mesh->GetNumSections(); ++i)
	{
		Mesh->SetMaterial(i, Material1);
	}
}

void AVerticalRailActor::CreateSphere(float Radius, int32 NumberOfStacks, int32 NumberOfSlices, int32 SectionIndex, FVector Offset)
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	 TArray<FProcMeshTangent> Tangents;

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

			Vertex = Vertex + Offset;

			Vertices.Add(Vertex);

			const FVector Normal = UKismetMathLibrary::GetDirectionUnitVector(Offset, Vertex);
		Normals.Add(Normal);

			const FVector2D UV = FVector2D(Theta / (2.0f * PI), Phi / PI);
			UVs.Add(UV);

			if (Lat < NumberOfStacks - 1 && Long < NumberOfSlices - 1)
			{
				const int32 Current = Lat * NumberOfSlices+Long;
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




	Mesh->CreateMeshSection_LinearColor(SectionIndex, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), Tangents, true);



}

void AVerticalRailActor::CreatePyramid(float SideLength, float Height, int32 SectionIndex, FVector Offset)
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	 TArray<FLinearColor> VertexColors;
	 TArray<FProcMeshTangent> Tangents;


	//Bottom
	Vertices.Add(FVector(0, 0, 0) + Offset);
	Vertices.Add(FVector(SideLength, 0, 0) + Offset);
	Vertices.Add(FVector(SideLength, SideLength, 0) + Offset);
	Vertices.Add(FVector(0, SideLength, 0) + Offset);

	//Front
	Vertices.Add(FVector(0, SideLength, 0) + Offset);
	Vertices.Add(FVector(SideLength / 2, SideLength / 2, Height) + Offset);
	Vertices.Add(FVector(SideLength, SideLength, 0) + Offset);

	//Back
	Vertices.Add(FVector(0, 0, 0) + Offset);
	Vertices.Add(FVector(SideLength / 2, SideLength / 2, Height) + Offset);
	Vertices.Add(FVector(SideLength, 0, 0) + Offset);

	//Left
	Vertices.Add(FVector(0, 0, 0) + Offset);
	Vertices.Add(FVector(SideLength / 2, SideLength / 2, Height) + Offset);
	Vertices.Add(FVector(0, SideLength, 0) + Offset);

	//Right
	Vertices.Add(FVector(SideLength, 0, 0) + Offset);
	Vertices.Add(FVector(SideLength / 2, SideLength / 2, Height) + Offset);
	Vertices.Add(FVector(SideLength, SideLength, 0) + Offset);

	Triangles = {
		0,1,2,0,2,3, //Base
		6,5,4, //Front
		7,8,9, //Back
		12,11,10, //Left
		15,13,14 //Right
	};



	//Base
	Normals.Add({ 0,0,-1 });
	Normals.Add({ 0,0,-1 });
	Normals.Add({ 0,0,-1 });
	Normals.Add({ 0,0,-1 });

	//Front
	for (int32 i{}; i < 3; ++i)
	{
		Normals.Add(-FVector::CrossProduct(UKismetMathLibrary::GetDirectionUnitVector(Vertices[6], Vertices[5]), UKismetMathLibrary::GetDirectionUnitVector(Vertices[5], Vertices[4])).GetSafeNormal());
	}

	//Back
	for (int32 i{}; i < 3; ++i)
	{
		Normals.Add(FVector::CrossProduct(Vertices[7] - Vertices[8], Vertices[9] - Vertices[8]).GetSafeNormal());
	}

	//Left
	for (int32 i{}; i < 3; ++i)
	{
		Normals.Add(-FVector::CrossProduct(Vertices[10] - Vertices[11], Vertices[12] - Vertices[11]).GetSafeNormal());
	}

	//Right
	for (int32 i{}; i < 3; ++i)
	{
		Normals.Add(FVector::CrossProduct(Vertices[13] - Vertices[14], Vertices[15] - Vertices[14]).GetSafeNormal());
	}



	UVs.Add(FVector2D(0, 0));
	UVs.Add(FVector2D(1, 0));
	UVs.Add(FVector2D(1, 1));
	UVs.Add(FVector2D(0, 1));

	//UV Mapping
	for (int32 i{}; i < 2; ++i)
	{
		UVs.Add(FVector2D(0, 1));
		UVs.Add(FVector2D(0.5, 0));
		UVs.Add(FVector2D(1, 1));

		UVs.Add(FVector2D(0, 1));
		UVs.Add(FVector2D(-0.5, 0));
		UVs.Add(FVector2D(-1, 1));
	}

	Mesh->CreateMeshSection_LinearColor(SectionIndex, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true);


}

void AVerticalRailActor::CreateCone(float Radius, float Height, int32 NumberOfStacks, int32 NumberOfSlices, int32 SectionIndex, FVector Offset)
{

	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;


	for (int32 Lat = 0; Lat < NumberOfStacks; ++Lat) {

		for (int32 Long = 0; Long < NumberOfSlices; ++Long) {

			float Phi = 2 * PI * Long;

			if (NumberOfSlices > 1) {
				Phi /= (NumberOfSlices - 1);
			}
			const float SinPhi = FMath::Sin(Phi);
			const float CosPhi = FMath::Cos(Phi);

			const float CurrentRadius = Radius * (static_cast<float>(Lat) / (NumberOfStacks - 1));

			FVector Vertex = FVector{ CurrentRadius * SinPhi,  CurrentRadius * CosPhi, Height * (1 - (static_cast<float>(Lat) / (NumberOfStacks - 1))) };

			Vertices.Add(Vertex + Offset);
			UVs.Add(FVector2D{ Phi / (2 * PI),  (1.0 - (static_cast<float>(Lat) / (NumberOfStacks - 1))) });

			if (Lat < NumberOfStacks - 1 && Long < NumberOfSlices - 1) {
				int32 Current = Lat * NumberOfSlices + Long;
				int32 Next = Current + NumberOfSlices;

				Triangles.Add(Current);
				Triangles.Add(Next);
				Triangles.Add(Next + 1);

				Triangles.Add(Next + 1);
				Triangles.Add(Current + 1);
				Triangles.Add(Current);
			}
		}
	}

	//Base
	Vertices.Add(FVector{0,0,0} + Offset);
	UVs.Add(FVector2D(0.5f, 0.5f));

	for (int32 i = 0; i < NumberOfSlices; ++i) {

		float Phi = 2 * PI * i;
		if (NumberOfSlices > 1) {
			Phi /= (NumberOfSlices - 1);
		}
		FVector Vertex{ Radius * FMath::Sin(Phi) , Radius * FMath::Cos(Phi) , 0 };
		
		Vertices.Add(Vertex + Offset);

		UVs.Add(FVector2D(Vertex.Y / Radius / 2.0f + 0.5f, Vertex.X / Radius / 2.0f + 0.5f));
	}

	//Triangles
	for (int32 i = 1; i <= NumberOfSlices; ++i) {
		if (i < NumberOfSlices) {
			Triangles.Add(NumberOfStacks * NumberOfSlices + i);
			Triangles.Add(NumberOfStacks * NumberOfSlices);
			Triangles.Add((NumberOfStacks * NumberOfSlices + i + 1));

		}
		else {
			Triangles.Add(NumberOfStacks * NumberOfSlices + i);
			Triangles.Add(NumberOfStacks * NumberOfSlices);
			Triangles.Add((NumberOfStacks * NumberOfSlices + 1));
		}
	}


	//Normals
	for (int i = 0; i < Vertices.Num(); i++) {
		Normals.Add(FVector::ZeroVector);
	}
	for (int i = 0; i < Triangles.Num(); i += 3) {

		if (i + 2 < Triangles.Num()) {

			FVector vec1 = Vertices[Triangles[i + 1]] - Vertices[Triangles[i]];
			FVector vec2 = Vertices[Triangles[i + 2]] - Vertices[Triangles[i]];

			const auto Normal = FVector::CrossProduct(vec2, vec1).GetSafeNormal();

			Normals[Triangles[i]] += Normal;
			Normals[Triangles[i + 1]] += Normal;
			Normals[Triangles[i + 2]] += Normal;

		}
	}

	for (int i = 0; i < Normals.Num(); i++) {
		Normals[i].Normalize();
	}


	Mesh->CreateMeshSection_LinearColor(SectionIndex, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>{}, TArray<FProcMeshTangent>{}, true);




}

void AVerticalRailActor::CreateBellShape(float BaseRadius, float Height, float RimRadius, float CurvatureFactor, int32 NumberOfStacks, int32 NumberOfSlices, int32 SectionIndex, FVector Offset)
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	 TArray<FProcMeshTangent> Tangents;
	 TArray<FLinearColor> Colors;


	const float DeltaTheta = 2.0f * PI / (NumberOfSlices - 1);
	const float DeltaPhi = PI / (NumberOfStacks - 1);

	for (int Lat = 0; Lat < NumberOfStacks; Lat++)
	{
		const float Phi = Lat * DeltaPhi;
		const float t = Phi / (PI / 2.0f);
		const float CurrentRadius = FMath::Lerp(BaseRadius, RimRadius, FMath::Sin(t * CurvatureFactor));

		for (int Long = 0; Long < NumberOfSlices; Long++)
		{
			const float Theta = Long * DeltaTheta;

			const float X = CurrentRadius * FMath::Sin(Theta);
			const float Y = CurrentRadius * FMath::Cos(Theta);
			const float Z = Height * (t - 1.0f); // Z decreases with height for bell shape



			//Vertices
			Vertices.Add(FVector(X, Y, Z) + Offset);

			//UVs
			UVs.Add(FVector2D(static_cast<float>(Long) / (NumberOfSlices - 1), - static_cast<float>(Lat) / (NumberOfStacks - 1)));



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


	//Normals
	for (int i = 0; i < Vertices.Num(); i++) {
		Normals.Add(FVector::ZeroVector);
	}
	for (int i = 0; i < Triangles.Num(); i += 3) {

		if (i + 2 < Triangles.Num()) {

			FVector vec1 = Vertices[Triangles[i + 1]] - Vertices[Triangles[i]];
			FVector vec2 = Vertices[Triangles[i + 2]] - Vertices[Triangles[i]];

			const auto Normal = FVector::CrossProduct(vec2, vec1).GetSafeNormal();

			Normals[Triangles[i]] += Normal;
			Normals[Triangles[i + 1]] += Normal;
			Normals[Triangles[i + 2]] += Normal;

		}
	}

	for (int i = 0; i < Normals.Num(); i++) {
		Normals[i].Normalize();
	}

	Mesh->CreateMeshSection_LinearColor(SectionIndex++, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);



}

void AVerticalRailActor::CreateSemiDisk(float Radius, float Width, int32 NumberOfSlices, int32 SectionIndex, FVector Offset)
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	 TArray<FProcMeshTangent> Tangents;
	TArray<FLinearColor> VertexColors;

	const float DeltaTheta = (PI) / (NumberOfSlices - 1);

	Vertices.Add(FVector{ 0,  Width / 2,0 } + Offset); // Center of front face
	Vertices.Add(FVector{ 0, -Width / 2,0 } + Offset); // Center of Back Face

	UVs.Add(FVector2D(0.5f, 0.f));	// Center of front face
	UVs.Add(FVector2D(0.5f, 0.f));	// Center of Back Face


	// Top vertices of the semicircles
	for (int32 i = 0; i < NumberOfSlices; i++)
	{
		const float Theta = i * DeltaTheta;

		const float X = Radius * FMath::Cos(Theta);
		const float Z = Radius * FMath::Sin(Theta);

		//Front Face
		Vertices.Add(FVector(X, Width / 2, Z) + Offset);

		//Back Face
		Vertices.Add(FVector(X, -Width / 2, Z) + Offset);


		UVs.Add({ X / Radius + 0.5f ,- Z/ Radius }); //Front
		UVs.Add({- X / Radius + 0.5f ,- Z/ Radius  }); //Front

	}


	// triangles for Front Faces
	{
		int32 CurrentVertexIndex = 2;

		for (int32 i = 0; i < NumberOfSlices - 1; i++)
		{
			Triangles.Add(0);
			Triangles.Add(CurrentVertexIndex);
			Triangles.Add(CurrentVertexIndex + 2);
			CurrentVertexIndex += 2;
		}
	}


	// triangles for Back Face
	{
		int32 CurrentVertexIndex = 3;

		for (int32 i = 0; i < NumberOfSlices - 1; i++)
		{
			Triangles.Add(1);
			Triangles.Add(CurrentVertexIndex + 2);
			Triangles.Add(CurrentVertexIndex);
			CurrentVertexIndex += 2;
		}
	}


	//Duplicate Vertices for Top Curve and UVs
	const int32 OldTotalVertices = Vertices.Num();

	for (int32 i = 2; i < OldTotalVertices; ++i)
	{
		auto Vertex = Vertices[i];
		Vertices.Add(Vertex);

		if (i % 2 == 0)
		{
			UVs.Add({ (i - 2)/ static_cast<float>(OldTotalVertices - 3), 0 });
		}
		else
		{
			UVs.Add({ (i - 2)/ static_cast<float>(OldTotalVertices - 3), 1 });
		}


	}


	//Triangles for top curve
	{
		int32 CurrentVertexIndex = OldTotalVertices;

		for (int32 i = 0; i < NumberOfSlices - 1; i++)
		{
		
				Triangles.Add(CurrentVertexIndex);
				Triangles.Add(CurrentVertexIndex + 1);
				Triangles.Add(CurrentVertexIndex + 2);

				Triangles.Add(CurrentVertexIndex + 2);
				Triangles.Add(CurrentVertexIndex + 1);
				Triangles.Add(CurrentVertexIndex + 3);

			
			CurrentVertexIndex += 2;
		}
	}


	//Normals
		for (int i = 0; i < Vertices.Num(); i++) {
			Normals.Add(FVector::ZeroVector);
		}

		for (int i = 0; i < Triangles.Num() - 2; i += 3) {

	
				FVector Normal;

				FVector vec1 = Vertices[Triangles[i]] - Vertices[Triangles[i + 1]];
				FVector vec2 = Vertices[Triangles[i + 2]] - Vertices[Triangles[i + 1]];

				Normal = FVector::CrossProduct(vec1, vec2);

				Normal.Normalize();

				Normals[Triangles[i]] += Normal;
				Normals[Triangles[i + 1]] += Normal;
				Normals[Triangles[i + 2]] += Normal;
			
		
		}


		for (int i = 0; i < Normals.Num(); i++) {
			Normals[i].Normalize();
		}
	

	Mesh->CreateMeshSection_LinearColor(SectionIndex, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), Tangents, true);



}

void AVerticalRailActor::CreateEgg(float Radius, float Height, int32 NumberOfStacks, int32 NumberOfSlices, int32 SectionIndex, FVector Offset)
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	 TArray<FColor> VertexColors;
	 TArray<FProcMeshTangent> Tangents;

	for (int32 Lat = 0; Lat < NumberOfStacks; Lat++)
	{
		const float Theta = Lat * PI / (NumberOfStacks-1);
		const float SinTheta = FMath::Sin(Theta);
		const float CosTheta = FMath::Cos(Theta);

		for (int32 Long = 0; Long < NumberOfSlices; Long++)
		{
			const float Phi = Long * 2 * PI / (NumberOfSlices-1);
			const float SinPhi = FMath::Sin(Phi);
			const float CosPhi = FMath::Cos(Phi);

			const float X = Radius * SinTheta * CosPhi;
			const float Y = Radius * SinTheta * SinPhi;
			const float Z = Height * CosTheta;

			Vertices.Add(FVector(X, Y, Z) + Offset);
			UVs.Add(FVector2D(static_cast<float>(Long) / (NumberOfSlices-1), static_cast<float>(Lat) / (NumberOfStacks-1)));



			if (Lat < NumberOfStacks - 1 && Long < NumberOfSlices - 1)
			{
				int32 Current = Lat * NumberOfSlices + Long;
				int32 Next = Current + NumberOfSlices;

				Triangles.Add(Current);
				Triangles.Add(Current + 1);
				Triangles.Add(Next);

				Triangles.Add(Next);
				Triangles.Add(Current + 1);
				Triangles.Add(Next + 1);

			}
		}
	}



	//Normals
	for (int i = 0; i < Vertices.Num(); i++) {
		Normals.Add(FVector::ZeroVector);
	}
	for (int i = 0; i < Triangles.Num(); i += 3) {

		if (i + 2 < Triangles.Num()) {

			FVector vec1 = Vertices[Triangles[i + 1]] - Vertices[Triangles[i]];
			FVector vec2 = Vertices[Triangles[i + 2]] - Vertices[Triangles[i]];

			const auto Normal = FVector::CrossProduct(vec2, vec1).GetSafeNormal();

			Normals[Triangles[i]] += Normal;
			Normals[Triangles[i + 1]] += Normal;
			Normals[Triangles[i + 2]] += Normal;

		}
	}

	for (int i = 0; i < Normals.Num(); i++) {
		Normals[i].Normalize();
	}




	Mesh->CreateMeshSection(SectionIndex, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true);




}

void AVerticalRailActor::CreateCylinder(float Radius, float Height, int32 NumberOfStacks, int32 NumberOfSlices, int32 SectionIndex, FVector Offset)
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	 TArray<FColor> VertexColors;
	 TArray<FProcMeshTangent> Tangents;


	// Bottom Center
	Vertices.Add(FVector(0, 0, 0) + Offset);
	UVs.Add(FVector2D(0.5f, 0.5f));


	//Vertices of Bottom Face
	for (int i = 0; i <= NumberOfSlices; i++)
	{
		const float Theta = i * 2 * PI / (NumberOfSlices - 1);
		const float SinTheta = FMath::Sin(Theta);
		const float CosTheta = FMath::Cos(Theta);

		Vertices.Add(FVector(CosTheta, SinTheta, 0) * Radius + Offset);
		UVs.Add(FVector2D(CosTheta+0.5f, -SinTheta+0.5f));
	}


	//Triangles of Bottom Face
	for (int i = 1; i < NumberOfSlices; i++) {
		Triangles.Add(0);
		Triangles.Add(i);
		Triangles.Add(i + 1);
	}


	//Top Center
	Vertices.Add(FVector(0, 0, Height) + Offset);
	UVs.Add(FVector2D(0.5f, 0.5f));


	//Vertices of Top Face
	for (int i = 0; i <= NumberOfSlices; i++)
	{
		const float Theta = i * 2 * PI / (NumberOfSlices - 1);
		const float SinTheta = FMath::Sin(Theta);
		const float CosTheta = FMath::Cos(Theta);

		Vertices.Add(FVector(CosTheta * Radius, SinTheta * Radius, Height) + Offset);
		UVs.Add(FVector2D(-CosTheta + 0.5f, -SinTheta+0.5f));
	}

	//Triangles of Top Face
	for (int i = 0; i < NumberOfSlices; i++) {
		Triangles.Add(NumberOfSlices + 2);
		Triangles.Add(NumberOfSlices + 2 + i + 1);
		Triangles.Add(NumberOfSlices + 2 + i);
	}

	for (int Lat = 0; Lat < NumberOfStacks; Lat++) {
		for (int Long = 0; Long < NumberOfSlices; Long++) {
			const float Theta = Long * PI * 2 / (NumberOfSlices - 1);
			const float SinTheta = FMath::Sin(Theta);
			const float CosTheta = FMath::Cos(Theta);


			const float X = CosTheta * Radius;
			const float Y = SinTheta * Radius;
			const float Z = Height * (1 - static_cast<float>(Lat) / (NumberOfStacks - 1));


			FVector Vertex{X,Y,Z};

			Vertices.Add(Vertex + Offset);
			UVs.Add(FVector2D(-Theta / (2 * PI), static_cast<float>(Lat) / (NumberOfStacks - 1)));

			if (Lat < NumberOfStacks - 1 && Long < NumberOfSlices - 1) {

				int Current = 2 * NumberOfSlices + 4 + (Lat * NumberOfSlices + Long);
				int Next = Current + NumberOfSlices;

				Triangles.Add(Current);
				Triangles.Add(Current + 1);
				Triangles.Add(Next);

				Triangles.Add(Next);
				Triangles.Add(Current + 1);
				Triangles.Add(Next + 1);
			}
		}
	}

	for (int i = 0; i < Vertices.Num(); i++) {
		Normals.Add(FVector::ZeroVector);
	}
	for (int i = 0; i < Triangles.Num(); i += 3) {

		if (i + 2 < Triangles.Num()) {

			FVector vec1 = Vertices[Triangles[i + 1]] - Vertices[Triangles[i]];
			FVector vec2 = Vertices[Triangles[i + 2]] - Vertices[Triangles[i]];

			const auto Normal = FVector::CrossProduct(vec2, vec1).GetSafeNormal();

			Normals[Triangles[i]] += Normal;
			Normals[Triangles[i + 1]] += Normal;
			Normals[Triangles[i + 2]] += Normal;

		}
	}


	for (int i = 0; i < Normals.Num(); i++) {
		Normals[i].Normalize();
	}

	Mesh->CreateMeshSection(SectionIndex, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true);

}


void AVerticalRailActor::CreateDonut(float OuterRadius, float DistanceFromOuterRadius, int NumberOfStacks, int NumberOfSlices, int32 SectionIndex, FVector Offset)
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;

	// Vertices
	for (int32 Lat = 0; Lat < NumberOfStacks; Lat++)
	{
		float Theta = 2.0f * PI * Lat / (NumberOfStacks-1);

		float CosTheta = FMath::Cos(Theta);
		float SinTheta = FMath::Sin(Theta);

		for (int32 Long = 0; Long < NumberOfSlices; Long++)
		{
			float Phi = 2.0f * PI * Long / (NumberOfSlices-1);

			float CosPhi = FMath::Cos(Phi);
			float SinPhi = FMath::Sin(Phi);

			FVector Vertex = FVector((OuterRadius + DistanceFromOuterRadius * CosTheta) * CosPhi, (OuterRadius + DistanceFromOuterRadius * CosTheta) * SinPhi, DistanceFromOuterRadius * SinTheta);

			Vertices.Add(Vertex + Offset);

			FVector2D UV = FVector2D(static_cast<float>(Lat) / (NumberOfStacks - 1), static_cast<float>(Long) / (NumberOfSlices - 1));
			UVs.Add(UV);


			//  triangles

			if(Lat < NumberOfStacks - 1 && Long < NumberOfSlices -1)
			{
				const int32 Current = Lat * NumberOfSlices + Long;
				const int32 Next = Current + NumberOfSlices;

				Triangles.Add(Current);
				Triangles.Add(Next);
				Triangles.Add(Current + 1);

				Triangles.Add(Current + 1);
				Triangles.Add(Next);
				Triangles.Add(Next + 1);
			}


		}
	}




	//Normals
	for (int i = 0; i < Vertices.Num(); i++) {
		Normals.Add(FVector::ZeroVector);
	}
	for (int i = 0; i < Triangles.Num(); i += 3) {

		if (i + 2 < Triangles.Num()) {

			FVector vec1 = Vertices[Triangles[i + 1]] - Vertices[Triangles[i]];
			FVector vec2 = Vertices[Triangles[i + 2]] - Vertices[Triangles[i]];

			const auto Normal = FVector::CrossProduct(vec2, vec1).GetSafeNormal();

			Normals[Triangles[i]] += Normal;
			Normals[Triangles[i + 1]] += Normal;
			Normals[Triangles[i + 2]] += Normal;

		}
	}


	for (int i = 0; i < Normals.Num(); i++) {
		Normals[i].Normalize();
	}
	Mesh->CreateMeshSection(SectionIndex, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true);
}