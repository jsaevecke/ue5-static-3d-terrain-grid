// Copyright 2018, Julien Saevecke, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameSparks/Private/GSApi.h"
#include "GameSparks/Public/GameSparksModule.h"
#include "LoginFormular.generated.h"

UCLASS()
class BLACKBOX_WAR_PROJECT_API ULoginFormular : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category="Authentication")
	FString Username;
	UPROPERTY(BlueprintReadWrite, Category="Authentication")
	FString Password;
	UPROPERTY(BlueprintReadWrite, Category = "Loading")
	TSubclassOf<UUserWidget> LoadingIndicatorBlueprint;

private:
	UUserWidget* LoadingIndicatorWidget;

public:
	UFUNCTION(BlueprintCallable, Category="Authentication")
	void OnSignIn();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Validation")
	bool IsUsernameValid();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Validation")
	bool IsPasswordValid();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Validation")
	bool IsFormularValid();

	void Login(FString Username, FString Password);
};
