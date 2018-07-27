// Copyright 2018, Julien Saevecke, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
//#include "GameSparks/Private/GameSparksComponent.h"
//#include "GameSparksModule.h"
//#include <GameSparks/GS.h>
//#include <GameSparks/generated/GSResponses.h>
//#include <GameSparks/generated/GSRequests.h>
#include "BLACKBOX_GameInstance.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8
{
	Menu,
	Game,
	None
};

/**
 * 
 */
UCLASS()
class BLACKBOX_WAR_PROJECT_API UBLACKBOX_GameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UBLACKBOX_GameInstance();

	UFUNCTION(BlueprintCallable, Category = "GameState")
	EGameState GetGameState();
	UFUNCTION(BlueprintCallable, Category = "GameState")
	void ChangeGameState(EGameState NewGameState);
private:
	void Init() override;
	void Shutdown() override;

	UFUNCTION()
	bool OnGameStateChange(EGameState NewGameState);
	UFUNCTION()
	void OnGameSparksAvailable(bool bAvailable);
private:
	UPROPERTY()
	EGameState CurrentGameState;
	//UPROPERTY(VisibleAnywhere)
	//UGameSparksComponent* GameSparks;
	//UPROPERTY(VisibleAnywhere)
	//UGSMessageListenersObject* GameSparksMessageListener;
	//UPROPERTY()
	//bool bGameSparksAvailable;
};
