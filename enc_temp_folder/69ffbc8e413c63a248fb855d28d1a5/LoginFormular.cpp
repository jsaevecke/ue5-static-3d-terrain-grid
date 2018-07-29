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

	if (!IsValid(LoadingIndicatorWidget) && IsValid(LoadingIndicatorBlueprint))
		LoadingIndicatorWidget = CreateWidget<UUserWidget>(this, LoadingIndicatorBlueprint);

	if (IsValid(LoadingIndicatorWidget) && !LoadingIndicatorWidget->IsInViewport())
		LoadingIndicatorWidget->AddToViewport();

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

					auto* PlayerState = Cast<APState>(GetWorld()->GetFirstPlayerController()->PlayerState);
							
					if (Response.GetDisplayName().HasValue())
						PlayerState->Displayname = UTF8_TO_TCHAR(Response.GetDisplayName().GetValue().c_str());
					if (Response.GetUserId().HasValue())
						PlayerState->UserId = UTF8_TO_TCHAR(Response.GetUserId().GetValue().c_str());
				
					auto* GameInstance = Cast<UGInstance>(GetGameInstance());
					GameInstance->ChangeState(EState::MainMenu);
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::MakeRandomColor(), TEXT("AccountDetails Failed"));
				}
			});
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::MakeRandomColor(), TEXT("Login Failed"));
			}
		}

		if (IsValid(LoadingIndicatorWidget) && LoadingIndicatorWidget->IsInViewport())
			LoadingIndicatorWidget->RemoveFromParent();

		SetIsEnabled(true);
	});
}

