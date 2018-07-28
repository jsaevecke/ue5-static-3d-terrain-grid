// Copyright 2018, Julien Saevecke, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameSparks/Private/GSApi.h"
#include "GameSparks/Public/GameSparksModule.h"
#include "StaticGameSparkLibrary.generated.h"

/**
 * 
 */
UCLASS()
class BLACKBOX_WAR_PROJECT_API UStaticGameSparkLibrary : public UObject
{
	GENERATED_BODY()
public:
	static void LoginRequestResponse(GameSparks::Core::GS& GameSpark, const GameSparks::Api::Responses::AuthenticationResponse& Response);
	static void LoginSuccessDetailsResponse(GameSparks::Core::GS& GameSpark, const GameSparks::Api::Responses::AccountDetailsResponse& Response);
	static void RegistrationRequestResponse(GameSparks::Core::GS& GameSpark, const GameSparks::Api::Responses::RegistrationResponse& Response);

	static void Login(FString Username, FString Password);
	static void Register(FString Username, FString Displayname, FString Email, FString Password);
};
