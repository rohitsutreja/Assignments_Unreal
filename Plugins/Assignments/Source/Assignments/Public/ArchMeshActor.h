#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "ArchMeshActor.generated.h"

UCLASS()
class ASSIGNMENTS_API AArchMeshActor : public AStaticMeshActor
{
	GENERATED_BODY()


public:
	void ChangeMaterial(UMaterialInterface* Material) const;
	void ChangeTexture(UTexture* TextureData) const;
	void ChangeMesh(UStaticMesh* Mesh) const;
	
};
