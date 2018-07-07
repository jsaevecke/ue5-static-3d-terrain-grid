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
	static void InitStatics(UWorld* CurrentWorld);
	static void LoginRequestResponse(GameSparks::Core::GS& GameSpark, const GameSparks::Api::Responses::AuthenticationResponse& Response);
	static void LoginSuccessDetailsResponse(GameSparks::Core::GS& GameSpark, const GameSparks::Api::Responses::AccountDetailsResponse& Response);
	static void RegistrationRequestResponse(GameSparks::Core::GS& GameSpark, const GameSparks::Api::Responses::RegistrationResponse& Response);

	static void Login(FString Username, FString Password)
	{
		if (Username.IsEmpty() || Password.IsEmpty())
		{
			return;
		}

		GameSparks::Core::GS& GameSpark = UGameSparksModule::GetModulePtr()->GetGSInstance();

		GameSparks::Api::Requests::AuthenticationRequest Request(GameSpark);
		Request.SetUserName(std::string(TCHAR_TO_UTF8(*Username)));
		Request.SetPassword(std::string(TCHAR_TO_UTF8(*Password)));
		Request.Send(LoginRequestResponse);
	}
	static void Register(FString Username, FString Password)
	{
		if (Username.IsEmpty() || Password.IsEmpty())
		{
			return;
		}

		GameSparks::Core::GS& GameSpark = UGameSparksModule::GetModulePtr()->GetGSInstance();

		GameSparks::Api::Requests::RegistrationRequest Request(GameSpark);
		Request.SetDisplayName(std::string(TCHAR_TO_UTF8(*Username)));
		Request.SetUserName(std::string(TCHAR_TO_UTF8(*Username)));
		Request.SetPassword(std::string(TCHAR_TO_UTF8(*Password)));
		Request.Send(RegistrationRequestResponse);
	}
private:
	static UWorld* World;
	static bool bInitialized;
};
