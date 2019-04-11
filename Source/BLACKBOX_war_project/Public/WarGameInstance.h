// Copyright 2018, Julien Saevecke, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UnrealNetwork.h"
#include <Online.h>
#include "WarGameInstance.generated.h"

UENUM(BlueprintType)
enum class EState : uint8
{
	None UMETA(DisplayName = "None"),
	MainMenu UMETA(DisplayName = "Main"),
	HostMenu UMETA(DisplayName = "Host"),
	ServerMenu UMETA(DisplayName = "Server"),
	OptionsMenu UMETA(DisplayName = "Options"),
	Game UMETA(DisplayName = "Game")
};

class UUserWidget;

UCLASS()
class BLACKBOX_WAR_PROJECT_API UWarGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WarGame|State")
	TMap<EState, TSubclassOf<UUserWidget>> StateWidgetBlueprints;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WarGame|State")
	TSubclassOf<UUserWidget> LoadingIndicatorBlueprint;

private:
	UPROPERTY()
	TMap<EState, UUserWidget*> StateWidgets;
	UPROPERTY()
	UUserWidget* ActiveStateWidget;
	UPROPERTY()
	UUserWidget* LoadingIndicator;

	EState CurrentState;
	TSharedPtr<FOnlineSessionSettings> SessionSettings;

public:
	UWarGameInstance();
	~UWarGameInstance() = default;

	UFUNCTION(BlueprintCallable, Category = "WarGame|State")
	void ChangeState(EState newState);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "WarGame|State")
	EState GetState();
	UFUNCTION(BlueprintCallable, Category = "WarGame|State")
	void ShowLoadingIndicator(bool showIndicator);

private:
	void OnStateChange(EState newState);
};
