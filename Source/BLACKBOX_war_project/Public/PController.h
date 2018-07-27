// Copyright 2018, Julien Saevecke, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PController.generated.h"

/**
 * 
 */
UCLASS()
class BLACKBOX_WAR_PROJECT_API APController : public APlayerController
{
	GENERATED_BODY()
private:
	float ConnectionDelay;
	float ConnectionDelayMultiplier;
	uint8 ConnectionTries;
	uint8 ConnectionMaxTries;
public:
	APController();
	~APController() = default;
private:
	UFUNCTION()
	void OnGameSparksAvailable(bool bAvailable);
	UFUNCTION()
	void OnConnect();

	virtual void BeginPlay() override;
};
