// Copyright 2018, Julien Saevecke, All rights reserved.

#include "BLACKBOX_MenuGameMode.h"
#include "GameSparks/Public/GameSparksModule.h"

void ABLACKBOX_MenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	GameSparks = NewObject<UGameSparksComponent>(this, FName("GameSparks Component"));

	if (!UGameSparksModule::GetModulePtr()->IsInitialized())
	{
		GameSparks->OnGameSparksAvailableDelegate.AddDynamic(this, &ABLACKBOX_MenuGameMode::OnGameSparksAvailable);

		GameSparks->Disconnect();
		GameSparks->Connect(FString("v356285XYYnC"), FString("KEUTGVaTqY1cfqGbwl6Z7xFhfrTeUBxX"), true, false);
	}
}
void ABLACKBOX_MenuGameMode::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);

	if (GameSparks)
	{
		GameSparks->Disconnect();
	}
}

void ABLACKBOX_MenuGameMode::OnGameSparksAvailable(bool bAvailable)
{
	if (bAvailable && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::MakeRandomColor(), TEXT("Gamesparks available"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::MakeRandomColor(), TEXT("Gamesparks not available"));
	}
}
