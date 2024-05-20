// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshSelectionScrollBox.h"

TSharedRef<SWidget> UMeshSelectionScrollBox::RebuildWidget()
{
	ScrollBar = SNew(SMeshSelectionScrollBox).MeshAssetManager(MeshAssetManager).ScrollBarAssetType(ScrollBarType).SizeOfThumbnail(SizeOfThumbnail).Title(Title).ThumbnailBackgroundColor(ThumbnailBackgroundColor).TitleTextSize(TitleTextSize).TextSize(TextSize);

	ScrollBar->OnMeshThumbnailSelected.BindUObject(this, &UMeshSelectionScrollBox::HandleMeshThumbnailSelected);
	ScrollBar->OnMaterialThumbnailSelected.BindUObject(this, &UMeshSelectionScrollBox::HandleMaterialThumbnailSelected);
	ScrollBar->OnTextureThumbnailSelected.BindUObject(this, &UMeshSelectionScrollBox::HandleTextureThumbnailSelected);

	return ScrollBar.ToSharedRef();
}

const FText UMeshSelectionScrollBox::GetPaletteCategory()
{
	Super::GetPaletteCategory();
	return FText::FromString("Panel");
}


void UMeshSelectionScrollBox::HandleMeshThumbnailSelected(const FMeshData& MeshData) 
{
	OnMeshThumbnailSelectedEvent.ExecuteIfBound(MeshData);
}

void UMeshSelectionScrollBox::HandleMaterialThumbnailSelected(const FMaterialData& MaterialData)
{
	OnMaterialThumbnailSelectedEvent.ExecuteIfBound(MaterialData);
}

void UMeshSelectionScrollBox::HandleTextureThumbnailSelected(const FTextureData& TextureData)
{
	OnTextureThumbnailSelectedEvent.ExecuteIfBound(TextureData);
}

void UMeshSelectionScrollBox::ReleaseSlateResources(bool bResourceRelease)
{
	Super::ReleaseSlateResources(bResourceRelease);
	if (ScrollBar) ScrollBar.Reset();
}

