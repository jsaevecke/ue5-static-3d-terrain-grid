// Copyright 2018, Julien Saevecke, All rights reserved.

#include "BLACKBOX_GameMode.h"
#include "GridSystem.h"
#include "Engine/World.h"

ABLACKBOX_GameMode::ABLACKBOX_GameMode()
{
	
}

AGridSystem* ABLACKBOX_GameMode::GetHexagonGrid()
{
	return HexagonGrid;
}

void ABLACKBOX_GameMode::StartPlay()
{
	Super::StartPlay();

	if (GridToSpawn)
	{
		UWorld* world = GetWorld();
		
		if (world)
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;

			FRotator rotator{ 0.f };
			FVector location{ 0.f };

			HexagonGrid = world->SpawnActor<AGridSystem>(GridToSpawn, location, rotator, spawnParams);
		}
	}
}


