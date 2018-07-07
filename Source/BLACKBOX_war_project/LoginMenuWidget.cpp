// Copyright 2018, Julien Saevecke, All rights reserved.

#include "LoginMenuWidget.h"


void ULoginMenuWidget::OnConnectClicked()
{
	UStaticGameSparkLibrary::Login(Username, Password);
}

void ULoginMenuWidget::OnRegisterClicked()
{
	UStaticGameSparkLibrary::Register(Username, Password);
}
