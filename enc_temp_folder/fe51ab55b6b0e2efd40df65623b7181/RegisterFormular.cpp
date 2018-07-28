// Copyright 2018, Julien Saevecke, All rights reserved.

#include "RegisterFormular.h"
#include "StaticGameSparkLibrary.h"



void URegisterFormular::OnRegister()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRegister called"));
	UStaticGameSparkLibrary::Register(Username, Displayname, Email, Password);
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
	return !Password.IsEmpty();
}

bool URegisterFormular::IsFormularValid()
{
	return IsUsernameValid() && IsDisplaynameValid() && IsEmailValid() && IsPasswordValid();
}
