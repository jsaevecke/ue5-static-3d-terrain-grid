// Copyright 2018, Julien Saevecke, All rights reserved.

#include "LoginFormular.h"
#include "PState.h"
#include "GInstance.h"

void ULoginFormular::OnSignIn()
{
	Login(Username, Password);
}

bool ULoginFormular::IsUsernameValid()
{
	return !Username.IsEmpty();
}

bool ULoginFormular::IsPasswordValid()
{
	return !Password.IsEmpty();
}

bool ULoginFormular::IsFormularValid()
{
	return IsUsernameValid() && IsPasswordValid();
}

void ULoginFormular::Login(FString Username, FString Password)
{
	if (Username.IsEmpty() || Password.IsEmpty())
	{
		return;
	}

	SetIsEnabled(false);
	Cast<UGInstance>(GetGameInstance())->ShowLoadingIndicator(true);

	auto& GameSpark = UGameSparksModule::GetModulePtr()->GetGSInstance();

	auto Request = GameSparks::Api::Requests::AuthenticationRequest{ GameSpark };
	Request.SetUserName(std::string(TCHAR_TO_UTF8(*Username)));
	Request.SetPassword(std::string(TCHAR_TO_UTF8(*Password)));
	Request.Send([&](GameSparks::Core::GS& GameSpark, const GameSparks::Api::Responses::AuthenticationResponse& Response)
	{
		if (!Response.GetHasErrors())
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::MakeRandomColor(), TEXT("Login Success"));
			}


			auto Request = GameSparks::Api::Requests::AccountDetailsRequest{ GameSpark };
			Request.Send([&](GameSparks::Core::GS& GameSpark, const GameSparks::Api::Responses::AccountDetailsResponse& Response)
			{
				if (!Response.GetHasErrors())
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::MakeRandomColor(), TEXT("AccountDetails Success"));

					auto* PState = Cast<APState>(GetWorld()->GetFirstPlayerController()->PlayerState);

					if (Response.GetDisplayName().HasValue())
						PState->Displayname = UTF8_TO_TCHAR(Response.GetDisplayName().GetValue().c_str());
					if (Response.GetUserId().HasValue())
						PState->UserId = UTF8_TO_TCHAR(Response.GetUserId().GetValue().c_str());

					auto* GInstance = Cast<UGInstance>(GetGameInstance());
					GInstance->ShowLoadingIndicator(false);
					GInstance->ChangeState(EState::Lobby);
				}
				else
				{
					// Todo
					GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::MakeRandomColor(), TEXT("AccountDetails Failed"));
				}
			});
		}
		else
		{
			// Todo
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::MakeRandomColor(), TEXT("Login Failed"));
			}
		}

		Cast<UGInstance>(GetGameInstance())->ShowLoadingIndicator(false);
		SetIsEnabled(true);
	});
}