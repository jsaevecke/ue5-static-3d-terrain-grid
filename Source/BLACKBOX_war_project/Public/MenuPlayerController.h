// Copyright 2018, Julien Saevecke, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MenuPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BLACKBOX_WAR_PROJECT_API AMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AMenuPlayerController();
	~AMenuPlayerController() = default;
};
