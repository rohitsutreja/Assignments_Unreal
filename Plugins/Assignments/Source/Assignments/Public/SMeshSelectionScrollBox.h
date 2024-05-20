// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeshAssetManager.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Layout/SScrollBox.h"

UENUM(BlueprintType)
enum class EAssetType : uint8 {
	Mesh,
	Material,
	Texture
};



DECLARE_DELEGATE_OneParam(FMeshThumbnailSelected, const FMeshData&);
DECLARE_DELEGATE_OneParam(FMaterialThumbnailSelected, const FMaterialData&);
DECLARE_DELEGATE_OneParam(FTextureThumbnailSelected, const FTextureData&);


class ASSIGNMENTS_API SMeshSelectionScrollBox : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMeshSelectionScrollBox)
		{}
		SLATE_ARGUMENT(EAssetType, ScrollBarAssetType)
		SLATE_ARGUMENT(TWeakObjectPtr<UMeshAssetManager>, MeshAssetManager)
		SLATE_ARGUMENT(int,SizeOfThumbnail)
		SLATE_ARGUMENT(FString,Title)
		SLATE_ARGUMENT(int, TitleTextSize)
		SLATE_ARGUMENT(int, TextSize)
		SLATE_ARGUMENT(FSlateColor,ThumbnailBackgroundColor)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	void RefreshWidget();
	void CreateMeshAssetScrollBox();
	void CreateMaterialAssetScrollBox();
	void CreateTextureAssetScrollBox();

	int SizeOfThumbnail{75};
	TWeakObjectPtr<UMeshAssetManager> MeshAssetManager{};
	TSharedPtr<SVerticalBox> ParentVerticalBox{};
	EAssetType TypeOfAsset = EAssetType::Mesh;
	FString Title;
	FSlateColor ThumbnailBackgroundColor;
	int TitleTextSize {20};
	int	TextSize {16};

	FMeshThumbnailSelected OnMeshThumbnailSelected;
	FMaterialThumbnailSelected OnMaterialThumbnailSelected;
	FTextureThumbnailSelected OnTextureThumbnailSelected;
};
