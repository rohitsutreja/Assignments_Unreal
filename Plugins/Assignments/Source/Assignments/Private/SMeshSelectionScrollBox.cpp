// Fill out your copyright notice in the Description page of Project Settings.


#include "SMeshSelectionScrollBox.h"
#include "SlateOptMacros.h"
#include "Styling/StyleColors.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SMeshSelectionScrollBox::Construct(const FArguments& InArgs)
{

	ParentVerticalBox = SNew(SVerticalBox);
	MeshAssetManager = InArgs._MeshAssetManager;
	TypeOfAsset = InArgs._ScrollBarAssetType;
	SizeOfThumbnail = InArgs._SizeOfThumbnail;
	Title = InArgs._Title;
	ThumbnailBackgroundColor = InArgs._ThumbnailBackgroundColor;
	TitleTextSize = InArgs._TitleTextSize;
	TextSize = InArgs._TextSize;

	ChildSlot
		[
			ParentVerticalBox.ToSharedRef()
		];

	RefreshWidget();
}

void SMeshSelectionScrollBox::RefreshWidget() {

	ParentVerticalBox->ClearChildren();

	switch (TypeOfAsset) {
	case EAssetType::Mesh: {
		CreateMeshAssetScrollBox();
		break;
	}
	case EAssetType::Material: {
		CreateMaterialAssetScrollBox();
		break;
	}
	case EAssetType::Texture: {
		CreateTextureAssetScrollBox();
		break;
	}
	}
}

void SMeshSelectionScrollBox::CreateMeshAssetScrollBox() {

	auto ScrollBox = SNew(SScrollBox);
	ScrollBox->SetOrientation(Orient_Horizontal);

	if (MeshAssetManager.IsValid()) {
		for (auto& MeshData : MeshAssetManager->MeshDataArray) {
			if (auto ThumbnailTexture = MeshData.Thumbnail) {

				auto ThumbnailBrush = new FSlateBrush();
				ThumbnailBrush->SetResourceObject(ThumbnailTexture);
				ThumbnailBrush->ImageSize = FVector2D(SizeOfThumbnail);

				auto ThumbnailImage = SNew(SImage)
					.DesiredSizeOverride(FVector2D(SizeOfThumbnail))
					.Image(ThumbnailBrush)
					.Cursor(EMouseCursor::Hand)
					.OnMouseButtonDown_Lambda([this, MeshData](const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
					if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
						OnMeshThumbnailSelected.ExecuteIfBound(MeshData);
						return FReply::Handled();
					}
					return FReply::Unhandled();
						});
				auto VerticalBox = SNew(SVerticalBox);
				auto Text = SNew(STextBlock)
					.Text(FText::FromString(MeshData.Name))
					.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), TextSize))
					.ColorAndOpacity(FSlateColor(FLinearColor::Black));
				auto ImageBorder = SNew(SBorder);

				auto BorderImage = new FSlateBrush();
				BorderImage->DrawAs = ESlateBrushDrawType::Type::RoundedBox;
				FSlateBrushOutlineSettings OutlineSettings{};
				OutlineSettings.CornerRadii = FVector4{ 8,8,8,8 };
				OutlineSettings.RoundingType = ESlateBrushRoundingType::Type::FixedRadius;
				BorderImage->OutlineSettings = OutlineSettings;

				ImageBorder->SetBorderImage(BorderImage);
				ImageBorder->SetBorderBackgroundColor(ThumbnailBackgroundColor);
				ImageBorder->SetContent(ThumbnailImage);
				ImageBorder->SetPadding(5);
	

				VerticalBox->AddSlot().Padding(10).AutoHeight()
					[
						ImageBorder
					];

				VerticalBox->AddSlot().Padding(10, 1).HAlign(EHorizontalAlignment::HAlign_Center)
					[
						Text
					];

				ScrollBox->AddSlot()
					[
						VerticalBox
					];


			}
		}

		auto TitleText = SNew(STextBlock)
			.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), TitleTextSize))
			.ColorAndOpacity(FSlateColor(FLinearColor::Black))
			.Text(FText::FromString(Title));


		ParentVerticalBox->AddSlot().AutoHeight().Padding(10, 10, 0, 0)
			[
				TitleText
			];

		ParentVerticalBox->AddSlot().AutoHeight()
			[
				ScrollBox
			];

	}
}
void SMeshSelectionScrollBox::CreateMaterialAssetScrollBox() {

	auto ScrollBox = SNew(SScrollBox);
	ScrollBox->SetOrientation(Orient_Horizontal);

	if (MeshAssetManager.IsValid()) {
		for (auto& MaterialData : MeshAssetManager->MaterialDataArray) {

			auto ThumbnailBrush = new FSlateBrush();


			ThumbnailBrush->SetResourceObject(MaterialData.Thumbnail);
			ThumbnailBrush->ImageSize = FVector2D(SizeOfThumbnail);

			auto ThumbnailImage = SNew(SImage)
				.Image(ThumbnailBrush)
				.Cursor(EMouseCursor::Hand)
				.OnMouseButtonDown_Lambda([this, MaterialData](const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
				if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
					OnMaterialThumbnailSelected.ExecuteIfBound(MaterialData);
					return FReply::Handled();
				}
				return FReply::Unhandled();
					});


			auto VerticalBox = SNew(SVerticalBox);
			auto Text = SNew(STextBlock)
				.Text(FText::FromString(MaterialData.Name))
				.ColorAndOpacity(FSlateColor(FLinearColor::Black))
				.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), TextSize));

			auto ImageBorder = SNew(SBorder);

			FSlateBrush* BorderImage = new FSlateBrush();
			BorderImage->DrawAs = ESlateBrushDrawType::Type::RoundedBox;
			FSlateBrushOutlineSettings OutlineSettings{};
			OutlineSettings.CornerRadii = FVector4{ 8,8,8,8 };
			OutlineSettings.RoundingType = ESlateBrushRoundingType::Type::FixedRadius;
			BorderImage->OutlineSettings = OutlineSettings;

			ImageBorder->SetBorderImage(BorderImage);
			ImageBorder->SetBorderBackgroundColor(ThumbnailBackgroundColor);
			ImageBorder->SetContent(ThumbnailImage);
			ImageBorder->SetPadding(5);

			VerticalBox->AddSlot().AutoHeight().Padding(10)
				[
					ImageBorder
				];

			VerticalBox->AddSlot().Padding(10, 1).HAlign(EHorizontalAlignment::HAlign_Center)
				[
					Text
				];

			ScrollBox->AddSlot()
				[
					VerticalBox
				];




		}

		auto TitleText = SNew(STextBlock)

			.Text(FText::FromString(Title))
			.ColorAndOpacity(FSlateColor(FLinearColor::Black))
			.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), TitleTextSize));

		ParentVerticalBox->AddSlot().AutoHeight().Padding(10, 10, 0, 0)
			[
				TitleText
			];
		ParentVerticalBox->AddSlot().AutoHeight()
			[
				ScrollBox
			];
	}
}
void SMeshSelectionScrollBox::CreateTextureAssetScrollBox() {

	auto ScrollBox = SNew(SScrollBox);
	ScrollBox->SetOrientation(Orient_Horizontal);

	if (MeshAssetManager.IsValid()) {
		for (auto& TextureData : MeshAssetManager->TextureDataArray) {
			if (auto ThumbnailTexture = Cast<UTexture2D>(TextureData.Texture)) {
				auto ThumbnailBrush = new FSlateBrush();

				ThumbnailBrush->SetResourceObject(ThumbnailTexture);
				ThumbnailBrush->ImageSize = FVector2D(SizeOfThumbnail);

				auto ThumbnailImage = SNew(SImage)
					.Image(ThumbnailBrush)
					.Cursor(EMouseCursor::Hand)
					.OnMouseButtonDown_Lambda([this, TextureData](const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
					if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
						OnTextureThumbnailSelected.ExecuteIfBound(TextureData);
						return FReply::Handled();
					}
					return FReply::Unhandled();
						});


				auto VerticalBox = SNew(SVerticalBox);

				auto Text = SNew(STextBlock)
					.Text(FText::FromString(TextureData.Name))
					.ColorAndOpacity(FSlateColor(FLinearColor::Black))
					.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), TextSize));;

				auto ImageBorder = SNew(SBorder);

				FSlateBrush* BorderImage = new FSlateBrush();
				BorderImage->DrawAs = ESlateBrushDrawType::Type::RoundedBox;
				FSlateBrushOutlineSettings OutlineSettings{};
				OutlineSettings.CornerRadii = FVector4{ 8,8,8,8 };
				OutlineSettings.RoundingType = ESlateBrushRoundingType::Type::FixedRadius;
				BorderImage->OutlineSettings = OutlineSettings;

				ImageBorder->SetBorderImage(BorderImage);
				ImageBorder->SetBorderBackgroundColor(ThumbnailBackgroundColor);
				ImageBorder->SetContent(ThumbnailImage);
				ImageBorder->SetPadding(5);


				VerticalBox->AddSlot().AutoHeight().Padding(10)
					[
						ImageBorder
					];

				VerticalBox->AddSlot().Padding(10, 1).HAlign(EHorizontalAlignment::HAlign_Center)
					[
						Text
					];

				ScrollBox->AddSlot()
					[
						VerticalBox
					];

			}
		}

		auto TitleText = SNew(STextBlock)
			
			.Text(FText::FromString(Title))
			.ColorAndOpacity(FSlateColor(FLinearColor::Black))
			.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), TitleTextSize));

		ParentVerticalBox->AddSlot().AutoHeight().Padding(10,10,0,0)
			[
				TitleText
			];
		ParentVerticalBox->AddSlot().AutoHeight()
			[
				ScrollBox
			];
	}
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION
