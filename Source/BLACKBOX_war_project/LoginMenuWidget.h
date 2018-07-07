// Copyright 2018, Julien Saevecke, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StaticGameSparkLibrary.h"
#include "LoginMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLACKBOX_WAR_PROJECT_API ULoginMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Login")
	FString Username;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Login")
	FString Password;
	UFUNCTION(BlueprintCallable, Category = "Login")
	void OnConnectClicked();
	UFUNCTION(BlueprintCallable, Category = "Login")
	void OnRegisterClicked();
};
