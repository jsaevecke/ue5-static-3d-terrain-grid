// Copyright 2018, Julien Saevecke, All rights reserved.

#include "StaticGameSparkLibrary.h"

void UStaticGameSparkLibrary::LoginRequestResponse(GameSparks::Core::GS& GameSpark, const GameSparks::Api::Responses::AuthenticationResponse& Response)
{
	if (!Response.GetHasErrors())
	{
		GameSparks::Api::Requests::AccountDetailsRequest AccountDetailsRequest(GameSpark);
		AccountDetailsRequest.Send(LoginSuccessDetailsResponse);
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::MakeRandomColor(), TEXT("Login Failed"));
		}
	}
}
void UStaticGameSparkLibrary::LoginSuccessDetailsResponse(GameSparks::Core::GS& GameSpark, const GameSparks::Api::Responses::AccountDetailsResponse& Response)
{
	if (!Response.GetHasErrors())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::MakeRandomColor(), Response.GetJSONString().c_str());
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::MakeRandomColor(), TEXT("Account Details Request Failed"));
		}
	}
}
void UStaticGameSparkLibrary::RegistrationRequestResponse(GameSparks::Core::GS& GameSpark, const GameSparks::Api::Responses::RegistrationResponse& Response)
{
	if (!Response.GetHasErrors())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::MakeRandomColor(), TEXT("Registration Successful!"));
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::MakeRandomColor(), Response.GetJSONString().c_str());
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::MakeRandomColor(), TEXT("Registration Failed"));
		}
	}
}

void UStaticGameSparkLibrary::Login(FString Username, FString Password)
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

void UStaticGameSparkLibrary::Register(FString Username, FString Displayname, FString Email, FString Password)
{
	if (Username.IsEmpty() || Displayname.IsEmpty() || Email.IsEmpty()|| Password.IsEmpty())
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

