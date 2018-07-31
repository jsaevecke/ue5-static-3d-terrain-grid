// Copyright 2018, Julien Saevecke, All rights reserved.

#include "GInstance.h"
#include "Private/GameSparksObject.h"
#include "Blueprint/UserWidget.h"

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

void UGInstance::ShowLoadingIndicator(bool bShow)
{
	check(IsValid(LoadingIndicatorBlueprint));

	if(!IsValid(LoadingIndicator))
		LoadingIndicator = CreateWidget<UUserWidget>(this, LoadingIndicatorBlueprint);

	if (IsValid(LoadingIndicator))
	{
		if (!bShow && LoadingIndicator->IsInViewport())
			LoadingIndicator->RemoveFromParent();
		if (bShow && !LoadingIndicator->IsInViewport())
			LoadingIndicator->AddToViewport();
	}
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
	if (StateWidgetBlueprints.Contains(NewState))
	{
		if (!StateWidgets.Contains(NewState))
		{
			auto Widget = CreateWidget<UUserWidget>(this, StateWidgetBlueprints[NewState]);
			StateWidgets.Add(NewState, Widget);
		}

		if (IsValid(ActiveStateWidget))
			ActiveStateWidget->RemoveFromParent();
		
		ActiveStateWidget = StateWidgets[NewState];
		ActiveStateWidget->AddToViewport();

		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("State widget does not exist for this state"));
}


