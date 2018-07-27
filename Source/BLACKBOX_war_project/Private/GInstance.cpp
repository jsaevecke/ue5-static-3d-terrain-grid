// Copyright 2018, Julien Saevecke, All rights reserved.

#include "GInstance.h"
#include "Private/GameSparksObject.h"

UGInstance::UGInstance()
	: GameSparksObject(nullptr), CurrentState(EState::None)
{
}

void UGInstance::ChangeState(EState NewState)
{
	check(NewState != EState::None);

	if (NewState != CurrentState)
	{
		OnStateChange(NewState);

		CurrentState = NewState;
	}
}

EState UGInstance::GetState()
{
	return CurrentState;
}

void UGInstance::Init()
{
	UGameInstance::Init();

	GameSparksObject = NewObject<UGameSparksObject>(this, UGameSparksObject::StaticClass());
}

void UGInstance::Shutdown()
{
	UGameInstance::Shutdown();

	if (IsValid(GameSparksObject))
		GameSparksObject->Disconnect();
}

void UGInstance::OnStateChange(EState NewState)
{
	switch (NewState)
	{
	case EState::LoginMenu:
		UE_LOG(LogTemp, Warning, TEXT("Changed to login menu"));
		break;
	case EState::MainMenu:
		UE_LOG(LogTemp, Warning, TEXT("Changed to main menu"));
		break;
	case EState::Game:
		UE_LOG(LogTemp, Warning, TEXT("Changed to game"));
		break;
	default:
		break;
	}
}


