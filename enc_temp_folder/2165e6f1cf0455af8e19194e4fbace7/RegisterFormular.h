// Copyright 2018, Julien Saevecke, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RegisterFormular.generated.h"

/**
 * 
 */
UCLASS()
class BLACKBOX_WAR_PROJECT_API URegisterFormular : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = "Authentication")
	FString Username;
	UPROPERTY(BlueprintReadWrite, Category = "Authentication")
	FString Displayname;
	UPROPERTY(BlueprintReadWrite, Category = "Authentication")
	FString Email;
	UPROPERTY(BlueprintReadWrite, Category = "Authentication")
	FString Password;

public:
	UFUNCTION(BlueprintCallable, Category = "Authentication")
	void OnRegister();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Validation")
	bool IsUsernameValid();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Validation")
	bool IsDisplaynameValid();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Validation")
	bool IsEmailValid();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Validation")
	bool IsPasswordValid();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Validation")
	bool IsFormularValid();
};
