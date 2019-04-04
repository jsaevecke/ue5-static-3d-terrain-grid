// Copyright 2018, Julien Saevecke, All rights reserved.


#include "WarGameInstance.h"

#include "Blueprint/UserWidget.h"

UWarGameInstance::UWarGameInstance()
	: CurrentState(EState::None)
{
}

void UWarGameInstance::ChangeState(EState NewState)
{
	check(NewState != EState::None);

	if (NewState != CurrentState)
	{
		OnStateChange(NewState);

		CurrentState = NewState;
	}
}

EState UWarGameInstance::GetState()
{
	return CurrentState;
}

void UWarGameInstance::ShowLoadingIndicator(bool bShow)
{
	check(IsValid(LoadingIndicatorBlueprint));

	if (!IsValid(LoadingIndicator))
		LoadingIndicator = CreateWidget<UUserWidget>(this, LoadingIndicatorBlueprint);

	if (IsValid(LoadingIndicator))
	{
		if (!bShow && LoadingIndicator->IsInViewport())
			LoadingIndicator->RemoveFromParent();
		if (bShow && !LoadingIndicator->IsInViewport())
			LoadingIndicator->AddToViewport();
	}
}


void UWarGameInstance::Init()
{
	UGameInstance::Init();
}

void UWarGameInstance::Shutdown()
{
	UGameInstance::Shutdown();
}

void UWarGameInstance::OnStateChange(EState NewState)
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
