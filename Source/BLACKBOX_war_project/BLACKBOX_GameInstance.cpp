// Copyright 2018, Julien Saevecke, All rights reserved.

#include "BLACKBOX_GameInstance.h"
//#include "StaticGameSparkLibrary.h"
//#include "GameSparks/Public/GameSparksModule.h"
#include <stdexcept>

UBLACKBOX_GameInstance::UBLACKBOX_GameInstance()
	: CurrentGameState(EGameState::None)
{

}

EGameState UBLACKBOX_GameInstance::GetGameState()
{
	return CurrentGameState;
}

void UBLACKBOX_GameInstance::ChangeGameState(EGameState NewGameState)
{
	if (CurrentGameState != NewGameState && OnGameStateChange(CurrentGameState))
	{
		CurrentGameState = NewGameState;
	}
}

void UBLACKBOX_GameInstance::Init()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::MakeRandomColor(), TEXT("INIT"));
	// Todo: Super::Init??

	//GameSparks = NewObject<UGameSparksComponent>(this, FName("GameSparks Component"));

	/*if (!UGameSparksModule::GetModulePtr()->IsInitialized())
	{
		GameSparks->OnGameSparksAvailableDelegate.AddDynamic(this, &UBLACKBOX_GameInstance::OnGameSparksAvailable);

		GameSparks->Disconnect();
		
		// Todo: Secure?
		GameSparks->Connect(FString("v356285XYYnC"), FString("KEUTGVaTqY1cfqGbwl6Z7xFhfrTeUBxX"), true, false);
	}*/

	//UStaticGameSparkLibrary::InitStatics(GetWorld());
}

void UBLACKBOX_GameInstance::Shutdown()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::MakeRandomColor(), TEXT("SHUTDOWN"));
	//Super::Shutdown();

	//if (GameSparks)
// 	//{
// 		GameSparks->Disconnect();
//	}
}

bool UBLACKBOX_GameInstance::OnGameStateChange(EGameState NewGameState)
{
	if (true)//bGameSparksAvailable)
	{
		switch (NewGameState)
		{
		case EGameState::Menu:
			break;
		case EGameState::Game:
			break;
		default:
			throw std::invalid_argument("Invalid GameState");
		}

		return true;
	}

	return false;
}

void UBLACKBOX_GameInstance::OnGameSparksAvailable(bool bAvailable)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::MakeRandomColor(), TEXT("ONAVAILABLE"));
	//bGameSparksAvailable = bAvailable;

	//if (bAvailable && GEngine)
// 	//{
// 		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::MakeRandomColor(), TEXT("Gamesparks available"));
// 	}
// 	else
// 	{
// 		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::MakeRandomColor(), TEXT("Gamesparks not available"));
// 	}
}
