#include "StairCaseActor.h"
#include "math.h"

AStairCaseActor::AStairCaseActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	NumberOfSteps = 10;

	StepScale = { 0.8,2,0.3 };

	TypeOfStairs = EStairTypes::ClosedStair;

	distance = 35;

	DepthLengthRatio = StepScale.Depth / StepScale.Length;

	StepMaterial = nullptr;
}

void AStairCaseActor::GenerateStair() {

	//This will help us in setting location based on dimensions of Steps. Default is 100 which is approx dimensions of most meshes.
	double StepLength{ StepScale.Length * 100.0 };
	double StepWidth{ StepScale.Width * 100.0 };
	double StepDepth{ StepScale.Depth * 100.0 };

	//Destroy Steps
	for (int i{}; i < ArrayOfStepComponent.Num(); ++i) {
		ArrayOfStepComponent[i].Destroy();
	}

	//Destroy two main rail pipes.
	for (int i{}; i < Rails.Num(); ++i) {
		if (Rails[i]) {
			Rails[i]->DestroyComponent();
		}
	}

	ArrayOfStepComponent.Empty();
	Rails.Empty();



	//setting DepthLength Ratio to find angle of stair from ground, in case of open case we have to consider distance between steps too.
	if (TypeOfStairs == EStairTypes::OpenStair) {
		DepthLengthRatio = (StepDepth + distance) / StepLength;
	}
	else {
		DepthLengthRatio = StepDepth / StepLength;
	}


	//Creating Steps along with left and right connector pillars as their children.
	for (int i{}; i < NumberOfSteps; ++i) {
		FStepComponent s;


		//We are multiplying scale with 100(which is approx size of the cube mesh) everywhere to convert scale into absolute values
		auto StairMeshComponent = NewObject<UStaticMeshComponent>(this, FName("Stair" + FString::FromInt(i)));
		StairMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		StairMeshComponent->RegisterComponent();


		//Each step will be higher as the Depth of step and forward as length of each step, multiplying by i because each steps are relative to root.
		if (TypeOfStairs == EStairTypes::ClosedStair) {
			StairMeshComponent->SetRelativeLocation(FVector(StepLength * i, 0, StepDepth * i));
			StairMeshComponent->SetRelativeScale3D(FVector(StepScale.Length, StepScale.Width, StepScale.Depth));
		}
		// Same as  Closed, but each step will be extra higher as much as the distance between two.
		else if (TypeOfStairs == EStairTypes::OpenStair) {
			StairMeshComponent->SetRelativeLocation(FVector(StepLength * i, 0, (StepDepth + distance) * i));
			StairMeshComponent->SetRelativeScale3D(FVector(StepScale.Length, StepScale.Width, StepScale.Depth));
		}

		//each steps size will increase by Depth of a single step, e.g. 1->2->3->4->..., So Below space will be filled.
		else if (TypeOfStairs == EStairTypes::BoxStair) {
			StairMeshComponent->SetRelativeLocation(FVector(StepLength * i, 0, (StepDepth * (i + 1) / 2)));
			StairMeshComponent->SetRelativeScale3D(FVector(StepScale.Length, StepScale.Width, StepScale.Depth * (i + 1)));
		}


		StairMeshComponent->SetMaterial(0, StepMaterial);
		StairMeshComponent->SetStaticMesh(StairMesh);

		s.BaseComponent = StairMeshComponent;


		//If railing box is checked, we will add two pillars on both side of each step and attach them as child components to step.
		if (ShowRailings) {
			auto RailingConnectorMeshComponentLeft = NewObject<UStaticMeshComponent>(this, FName("RailingConnectorLeft" + FString::FromInt(i)));
			RailingConnectorMeshComponentLeft->AttachToComponent(StairMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
			RailingConnectorMeshComponentLeft->RegisterComponent();

			auto RailingConnectorMeshComponentRight = NewObject<UStaticMeshComponent>(this, FName("RailingConnectorRight" + FString::FromInt(i)));
			RailingConnectorMeshComponentRight->AttachToComponent(StairMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
			RailingConnectorMeshComponentRight->RegisterComponent();

			//Pillar's Height will be 6 times of the Depth of each step.
			//Pillars will be 42% (Hence 8% from the corner) far from the center of the step.
			if (TypeOfStairs == EStairTypes::BoxStair) {
				RailingConnectorMeshComponentLeft->SetWorldLocation(FVector(GetActorLocation().X + i * StepLength, GetActorLocation().Y + 0.42 * (StepWidth), GetActorLocation().Z + StepDepth * i + StepDepth * 3));
				RailingConnectorMeshComponentLeft->SetWorldScale3D(FVector(0.3, 0.05, (StepScale.Depth * 6)));


				RailingConnectorMeshComponentRight->SetWorldLocation(FVector(GetActorLocation().X + i * StepLength, GetActorLocation().Y - 0.42 * (StepWidth), GetActorLocation().Z + StepDepth * i + StepDepth * 3));
				RailingConnectorMeshComponentRight->SetWorldScale3D(FVector(0.3, 0.05, StepScale.Depth * 6));

			}
			else {
				RailingConnectorMeshComponentLeft->SetRelativeLocation(FVector(0, 42, StepDepth * 11));
				RailingConnectorMeshComponentLeft->SetRelativeScale3D(FVector(0.3, 0.05, 6));

				RailingConnectorMeshComponentRight->SetRelativeLocation(FVector(0, -42, StepDepth * 11));
				RailingConnectorMeshComponentRight->SetRelativeScale3D(FVector(0.3, 0.05, 6));

			}



			RailingConnectorMeshComponentLeft->SetMaterial(0, RailConnectorMaterial);
			RailingConnectorMeshComponentRight->SetMaterial(0, RailConnectorMaterial);

			RailingConnectorMeshComponentLeft->SetStaticMesh(RailingConnectorMesh);
			RailingConnectorMeshComponentRight->SetStaticMesh(RailingConnectorMesh);

			s.LeftRailComponent = RailingConnectorMeshComponentLeft;
			s.RightRailComponent = RailingConnectorMeshComponentRight;
		}

		//Adding structure of step which contains three components(Step, Left and right pillars) to array.
		ArrayOfStepComponent.Add(s);
	}



	//For adding main pipe on top of pillars, two pipes left and right will be there, at the same angle as the stair.
	if (ShowRailings) {

		//Left main pipe
		auto RailingLeft = NewObject<UStaticMeshComponent>(this, FName("Left"));
		RailingLeft->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		RailingLeft->RegisterComponent();

		RailingLeft->SetRelativeScale3D(FVector(StepScale.Length * (NumberOfSteps) / cos(atan(DepthLengthRatio)), 0.1, 0.2));

		//In case of open stair, angle will be different as distance between steps also needs to be considered.
		if (TypeOfStairs == EStairTypes::OpenStair) {
			RailingLeft->SetRelativeLocation(FVector(
				(StepLength * NumberOfSteps / 2 - StepLength / 2),
				(StepWidth * 0.42),
				(NumberOfSteps / 2) * (StepDepth + distance) + (StepDepth * 6)));
		}
		else {
			RailingLeft->SetRelativeLocation(FVector(
				(StepLength * NumberOfSteps / 2 - StepLength / 2),
				(StepWidth * 0.42),
				(NumberOfSteps / 2) * (StepDepth)+(StepDepth * 6)));
		}

		RailingLeft->SetStaticMesh(RailingMesh);
		RailingLeft->SetMaterial(0, RailMaterial);
		RailingLeft->SetRelativeRotation(FQuat(FRotator(atan(DepthLengthRatio) * 57.2958, 0, 0)));
		Rails.Add(RailingLeft);



		//Right Main Pipe
		auto RailingRight = NewObject<UStaticMeshComponent>(this, FName("Right"));
		RailingRight->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		RailingRight->RegisterComponent();

		RailingRight->SetRelativeScale3D(FVector(StepScale.Length * (NumberOfSteps) / cos(atan(DepthLengthRatio)), 0.1, 0.2));

		//In case of open stair, angle will be different as distance between steps also needs to be considered.
		if (TypeOfStairs == EStairTypes::OpenStair) {
			RailingRight->SetRelativeLocation(FVector(
				(StepLength * NumberOfSteps / 2 - StepLength / 2),
				-(StepWidth * 0.42),
				(NumberOfSteps / 2) * (StepDepth + distance) + (StepDepth * 6)));
		}
		else {
			RailingRight->SetRelativeLocation(FVector(
				(StepLength * NumberOfSteps / 2 - StepLength / 2),
				-(StepWidth * 0.42),
				((NumberOfSteps / 2) * StepDepth) + (StepDepth * 6)));
		}

		RailingRight->SetStaticMesh(RailingMesh);
		RailingRight->SetMaterial(0, RailMaterial);

		//Multiply with 57.2958 to convert radians into degree.
		RailingRight->SetRelativeRotation(FQuat(FRotator(atan(DepthLengthRatio) * 57.2958, 0, 0)));
		Rails.Add(RailingRight);

	}

}
