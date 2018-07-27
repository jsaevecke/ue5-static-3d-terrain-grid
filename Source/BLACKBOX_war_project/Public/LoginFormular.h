// Copyright 2018, Julien Saevecke, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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
public:
	UFUNCTION(BlueprintCallable, Category="Authentication")
	void OnLogin();
};
