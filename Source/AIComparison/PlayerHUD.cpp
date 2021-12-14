// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"

APlayerHUD::APlayerHUD()
{
	// Setup crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTextureObj(TEXT("Texture2D'/Game/Textures/HUD/FirstPersonCrosshair.FirstPersonCrosshair'"));
	CrosshairTexture = CrosshairTextureObj.Object;
}

void APlayerHUD::DrawHUD()
{
	Super::DrawHUD();

	if (CrosshairTexture)
	{ 
		// Draw crosshair to screen
		const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
		const FVector2D drawPosition(Center.X - (CrosshairTexture->GetSurfaceWidth() * 0.5f), Center.Y - (CrosshairTexture->GetSurfaceHeight() * 0.5f));

		FCanvasTileItem item(drawPosition, CrosshairTexture->Resource, FLinearColor::Yellow);
		item.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(item);
	}
	

}