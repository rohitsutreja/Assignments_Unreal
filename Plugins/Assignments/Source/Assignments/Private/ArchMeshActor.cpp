// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchMeshActor.h"

void AArchMeshActor::ChangeMesh(UStaticMesh* Mesh) const
{
	GetStaticMeshComponent()->SetStaticMesh(Mesh);
}
void AArchMeshActor::ChangeMaterial(UMaterialInterface* Material) const
{
	GetStaticMeshComponent()->SetMaterial(0, Material);
}
void AArchMeshActor::ChangeTexture(UTexture* Texture) const
{
			TArray<UMaterialInterface*> Materials = GetStaticMeshComponent()->GetMaterials();
			int i = 0;
			for (UMaterialInterface* Material : Materials)
			{
				if (Material)
				{
					UMaterialInstanceDynamic* DynMaterial = Cast<UMaterialInstanceDynamic>(Material);
					if (!DynMaterial)
					{
						DynMaterial = UMaterialInstanceDynamic::Create(Material, nullptr);
						GetStaticMeshComponent()->SetMaterial(i++, DynMaterial);
					}
					DynMaterial->SetTextureParameterValue(FName("BaseColorTexture"), Texture);
					
				}
			}
		
	
}
