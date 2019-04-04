// Copyright 2018, Julien Saevecke, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GamePlayPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BLACKBOX_WAR_PROJECT_API AGamePlayPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AGamePlayPlayerController();
	~AGamePlayPlayerController() = default;
};
