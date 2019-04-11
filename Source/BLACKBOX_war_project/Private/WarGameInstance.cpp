// Copyright 2018, Julien Saevecke, All rights reserved.


#include "WarGameInstance.h"

#include "Blueprint/UserWidget.h"

//Note: Consider to create an instance for each state on init - to remove the possibility of delayed transition between widgets

UWarGameInstance::UWarGameInstance()
	: CurrentState(EState::None)
{
}

void UWarGameInstance::ChangeState(EState newState)
{
	check(newState != EState::None);

	if (newState != CurrentState)
	{
		OnStateChange(newState);

		CurrentState = newState;
	}
}

EState UWarGameInstance::GetState()
{
	return CurrentState;
}

void UWarGameInstance::ShowLoadingIndicator(bool showIndicator)
{
	if (!IsValid(LoadingIndicatorBlueprint))
		return;

	if (!IsValid(LoadingIndicator))
		LoadingIndicator = CreateWidget<UUserWidget>(this, LoadingIndicatorBlueprint);

	if (IsValid(LoadingIndicator))
	{
		if (!showIndicator && LoadingIndicator->IsInViewport())
			LoadingIndicator->RemoveFromParent();
		if (showIndicator && !LoadingIndicator->IsInViewport())
			LoadingIndicator->AddToViewport();
	}
}

void UWarGameInstance::OnStateChange(EState newState)
{
	//Assume that OnStateChange only gets calles with a valid state
	check(StateWidgetBlueprints.Contains(newState));

	//Create the widget if no instance already exists
	if (!StateWidgets.Contains(newState))
	{
		auto Widget = CreateWidget<UUserWidget>(this, StateWidgetBlueprints[newState]);
		StateWidgets.Add(newState, Widget);
	}

	//Remove the active widget from the viewport
	if (IsValid(ActiveStateWidget))
		ActiveStateWidget->RemoveFromParent();

	//Add the new state widget to the viewport
	ActiveStateWidget = StateWidgets[newState];
	ActiveStateWidget->AddToViewport();

	return;
}
