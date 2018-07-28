// Copyright 2018, Julien Saevecke, All rights reserved.

#include "LoginFormular.h"
#include "StaticGameSparkLibrary.h"

void ULoginFormular::OnSignIn()
{
	UE_LOG(LogTemp, Warning, TEXT("OnSignIncalled"));

	UStaticGameSparkLibrary::Login(Username, Password);
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

