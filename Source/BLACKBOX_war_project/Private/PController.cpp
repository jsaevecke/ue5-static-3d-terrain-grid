// Copyright 2018, Julien Saevecke, All rights reserved.

#include "PController.h"
#include "GInstance.h"
#include "Public/GameSparksModule.h"
#include "Private/GameSparksObject.h"


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
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Could not connect to gamesparks. After max. %i tries."), ConnectionMaxTries);
		}
	}
}

void APController::OnConnect()
{
	auto* GameInstance = Cast<UGInstance>(GetGameInstance());
	GameInstance->GameSparksObject->Connect("v356285XYYnC", "KEUTGVaTqY1cfqGbwl6Z7xFhfrTeUBxX");
}

void APController::BeginPlay()
{
	auto* GameInstance = Cast<UGInstance>(GetGameInstance());

	check(IsValid(GameInstance->GameSparksObject));

	GameInstance->GameSparksObject->OnGameSparksAvailableDelegate.AddDynamic(this, &APController::OnGameSparksAvailable);
	GameInstance->GameSparksObject->Disconnect();
	GameInstance->GameSparksObject->Connect("v356285XYYnC", "KEUTGVaTqY1cfqGbwl6Z7xFhfrTeUBxX");
}
