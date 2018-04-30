// Copyright 2018, Julien Saevecke, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BLACKBOX_GameMode.generated.h"

class AGridSystem;

/**
 * 
 */
UCLASS()
class BLACKBOX_WAR_PROJECT_API ABLACKBOX_GameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ABLACKBOX_GameMode();

	UFUNCTION(BlueprintCallable, Category = "Grid")
	AGridSystem* GetHexagonGrid();
private:
	UPROPERTY(EditAnywhere, Category = "Grid")
	TSubclassOf<class AGridSystem> GridToSpawn;
	UPROPERTY()
	AGridSystem* HexagonGrid = nullptr;

protected:
	virtual void StartPlay() override;
};
