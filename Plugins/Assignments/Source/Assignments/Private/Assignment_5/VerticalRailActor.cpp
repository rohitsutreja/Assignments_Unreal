// Fill out your copyright notice in the Description page of Project Settings.


#include "Assignment_5/VerticalRailActor.h"

// Sets default values
AVerticalRailActor::AVerticalRailActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVerticalRailActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVerticalRailActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

