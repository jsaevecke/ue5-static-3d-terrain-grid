// Copyright 2018, Julien Saevecke, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PState.generated.h"

UCLASS()
class BLACKBOX_WAR_PROJECT_API APState : public APlayerState
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = "Profile")
	FString Displayname;
	UPROPERTY(BlueprintReadWrite, Category = "Profile")
	FString UserId;
};
