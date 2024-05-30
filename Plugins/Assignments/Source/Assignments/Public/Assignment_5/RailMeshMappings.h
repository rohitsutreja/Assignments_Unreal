// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VerticalRailActor.h"
#include "Engine/DataAsset.h"
#include "RailMeshMappings.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENTS_API URailMeshMappings : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ERailType, UStaticMesh*> RailMeshMappingMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ERailType, TSubclassOf<AVerticalRailActor>> RailClassMappingMap;
	
};
