// Copyright 2018, Julien Saevecke, All rights reserved.

#include "PController.h"
#include "GInstance.h"
#include "Public/GameSparksModule.h"
#include "Private/GameSparksObject.h"
#include "GameSparks/Private/GSApi.h"
#include "PState.h"


APController::APController()
	: ConnectionDelay(1.f), ConnectionDelayMultiplier(5.f), ConnectionTries(0), ConnectionMaxTries(5)
{
	bShowMouseCursor = true;
}

void APController::OnGameSparksAvailable(bool bAvailable)
{
	if (bAvailable)
	{
		if (IsValid(GEngine))
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Connected to gamesparks"));

		auto* GInstance = Cast<UGInstance>(GetGameInstance());
		
		GInstance->ShowLoadingIndicator(false);

		if (GInstance->GetState() == EState::None)
			GInstance->ChangeState(EState::Authentication);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Could not connect to gamesparks. Tries: %i"), ConnectionTries);

		if (ConnectionTries < ConnectionMaxTries)
		{
			ConnectionDelay *= ConnectionDelayMultiplier;
			ConnectionTries += 1;

			auto UnusedHandle = FTimerHandle{};
			GetWorldTimerManager().SetTimer(UnusedHandle, this, &APController::OnConnect, ConnectionDelay, false);
		}
		else
		{
			// Todo
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Could not connect to gamesparks. After max. %i tries."), ConnectionMaxTries);
		}
	}
}

void APController::OnConnect()
{
	Cast<UGInstance>(GetGameInstance())->GameSparksObject->Connect("v356285XYYnC", "KEUTGVaTqY1cfqGbwl6Z7xFhfrTeUBxX");
}

void APController::BeginPlay()
{
	auto* GInstance = Cast<UGInstance>(GetGameInstance());

	check(IsValid(GInstance->GameSparksObject));

	if (!GInstance->GameSparksObject->IsAvailable())
	{
		GInstance->GameSparksObject->OnGameSparksAvailableDelegate.AddDynamic(this, &APController::OnGameSparksAvailable);
		GInstance->GameSparksObject->Disconnect();
		GInstance->GameSparksObject->Connect("v356285XYYnC", "KEUTGVaTqY1cfqGbwl6Z7xFhfrTeUBxX");
		GInstance->ShowLoadingIndicator(true);
	}
}
