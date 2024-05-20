// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "SMeshSelectionScrollBox.h"
#include "MeshSelectionScrollBox.generated.h"

DECLARE_DELEGATE_OneParam(FMeshThumbnailSelectedEvent, const FMeshData&)
DECLARE_DELEGATE_OneParam(FMaterialThumbnailSelectedEvent, const FMaterialData&)
DECLARE_DELEGATE_OneParam(FTextureThumbnailSelectedEvent, const FTextureData&)




UCLASS()
class ASSIGNMENTS_API UMeshSelectionScrollBox : public UWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMeshAssetManager* MeshAssetManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAssetType ScrollBarType;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TitleTextSize{20};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TextSize{16};


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SizeOfThumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateColor ThumbnailBackgroundColor;

	virtual TSharedRef<SWidget> RebuildWidget() override;

	const FText GetPaletteCategory() override;



	UFUNCTION()
	void HandleMeshThumbnailSelected(const FMeshData& MeshData);

	UFUNCTION()
	void HandleMaterialThumbnailSelected(const FMaterialData& MaterialData);

	UFUNCTION()
	void HandleTextureThumbnailSelected(const FTextureData& TextureData);

	virtual void ReleaseSlateResources(bool bResourceRelease) override;

	TSharedPtr<SMeshSelectionScrollBox> ScrollBar;

	FMeshThumbnailSelectedEvent OnMeshThumbnailSelectedEvent;
	FMaterialThumbnailSelectedEvent OnMaterialThumbnailSelectedEvent;
	FTextureThumbnailSelectedEvent OnTextureThumbnailSelectedEvent;

};

