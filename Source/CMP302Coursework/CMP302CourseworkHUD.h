// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CMP302CourseworkHUD.generated.h"

UCLASS()
class ACMP302CourseworkHUD : public AHUD
{
	GENERATED_BODY()

public:
	ACMP302CourseworkHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

