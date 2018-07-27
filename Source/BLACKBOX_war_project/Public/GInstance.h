// Copyright 2018, Julien Saevecke, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GInstance.generated.h"

UENUM(BlueprintType)
enum class EState : uint8
{
	None UMETA(DisplayName = "None"),
	LoginMenu UMETA(DisplayName = "LoginMenu"),
	MainMenu UMETA(DisplayName = "MainMenu"),
	Game UMETA(DisplayName = "Game")
};

class UUserWidget;
class UGameSparksObject;

UCLASS()
class BLACKBOX_WAR_PROJECT_API UGInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameSpark")
	UGameSparksObject* GameSparksObject;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State")
	TMap<EState, TSubclassOf<UUserWidget>> StateWidgetBlueprints;
private:
	UPROPERTY()
	TMap<EState, UUserWidget*> StateWidgets;
	UPROPERTY()
	UUserWidget* ActiveStateWidget;
	EState CurrentState;
public:
	UGInstance();
	~UGInstance() = default;

	UFUNCTION(BlueprintCallable, Category = "State")
	void ChangeState(EState NewState);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State")
	EState GetState();
private:
	virtual void Init() override;
	virtual void Shutdown() override;

	void OnStateChange(EState NewState);
};
