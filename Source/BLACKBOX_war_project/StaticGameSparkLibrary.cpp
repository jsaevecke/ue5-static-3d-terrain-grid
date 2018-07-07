// Copyright 2018, Julien Saevecke, All rights reserved.

#include "StaticGameSparkLibrary.h"

UWorld* UStaticGameSparkLibrary::World = nullptr;
bool UStaticGameSparkLibrary::bInitialized = false;

void UStaticGameSparkLibrary::InitStatics(UWorld* CurrentWorld)
{
	if (!bInitialized)
	{
		World = CurrentWorld;
		bInitialized = true;
	}
}
void UStaticGameSparkLibrary::LoginRequestResponse(GameSparks::Core::GS& GameSpark, const GameSparks::Api::Responses::AuthenticationResponse& Response)
{
	if (bInitialized)
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
}
void UStaticGameSparkLibrary::LoginSuccessDetailsResponse(GameSparks::Core::GS& GameSpark, const GameSparks::Api::Responses::AccountDetailsResponse& Response)
{
	if (bInitialized)
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
}
void UStaticGameSparkLibrary::RegistrationRequestResponse(GameSparks::Core::GS& GameSpark, const GameSparks::Api::Responses::RegistrationResponse& Response)
{
	if (bInitialized)
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
}

