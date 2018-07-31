// Copyright 2018, Julien Saevecke, All rights reserved.

#include "RegisterFormular.h"
#include "GInstance.h"

void URegisterFormular::OnRegister()
{
	Register(Username, Displayname, Email, Password);
}

bool URegisterFormular::IsUsernameValid()
{
	return !Username.IsEmpty();
}

bool URegisterFormular::IsDisplaynameValid()
{
	return !Displayname.IsEmpty();
}

bool URegisterFormular::IsEmailValid()
{
	return !Email.IsEmpty();
}

bool URegisterFormular::IsPasswordValid()
{
	return !Password.IsEmpty() && !PasswordConfirmation.IsEmpty() && Password.Equals(PasswordConfirmation);
}

bool URegisterFormular::IsFormularValid()
{
	return IsUsernameValid() && IsDisplaynameValid() && IsEmailValid() && IsPasswordValid();
}

void URegisterFormular::Register(FString Username, FString Displayname, FString Email, FString Password)
{
	if (Username.IsEmpty() || Displayname.IsEmpty() || Email.IsEmpty() || Password.IsEmpty())
	{
		return;
	}

	SetIsEnabled(false);
	Cast<UGInstance>(GetGameInstance())->ShowLoadingIndicator(true);

	auto& GameSpark = UGameSparksModule::GetModulePtr()->GetGSInstance();

	auto ScriptData = GameSparks::Core::GSRequestData{};
	ScriptData.AddString("Email", std::string(TCHAR_TO_UTF8(*Email)));

	auto Request = GameSparks::Api::Requests::RegistrationRequest{ GameSpark };
	Request.SetDisplayName(std::string(TCHAR_TO_UTF8(*Username)));
	Request.SetUserName(std::string(TCHAR_TO_UTF8(*Username)));
	Request.SetPassword(std::string(TCHAR_TO_UTF8(*Password)));
	Request.SetScriptData(ScriptData);
	Request.Send([&](GameSparks::Core::GS& GameSpark, const GameSparks::Api::Responses::RegistrationResponse& Response){
		if (!Response.GetHasErrors())
		{
			// Todo
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::MakeRandomColor(), TEXT("Registration Successful!"));
			}
		}
		else
		{
			// Todo
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::MakeRandomColor(), TEXT("Registration Failed"));
			}
			
		}

		SetIsEnabled(true);
		Cast<UGInstance>(GetGameInstance())->ShowLoadingIndicator(false);
	});
}