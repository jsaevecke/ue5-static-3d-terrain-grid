// Copyright 2018, Julien Saevecke, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameSparks/Private/GameSparksComponent.h"
#include "BLACKBOX_MenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BLACKBOX_WAR_PROJECT_API ABLACKBOX_MenuGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;

private:
	UPROPERTY(VisibleAnywhere)
	UGameSparksComponent* GameSparks;

	UFUNCTION()
	void OnGameSparksAvailable(bool bAvailable);
};
