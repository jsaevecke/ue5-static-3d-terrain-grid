// Copyright 2018, Julien Saevecke, All rights reserved.

#include "ObjectPool.h"
#include "Engine/World.h"


// Sets default values
AObjectPool::AObjectPool()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

// Called when the game starts or when spawned
void AObjectPool::BeginPlay()
{
	Super::BeginPlay();

#if WITH_EDITOR
	this->SetFolderPath(PoolFolder);
#endif

	ReserveObjects();
}

void AObjectPool::ReserveObjects()
{
	for (auto Count = uint8{ 0 }; Count < ReserveObjectCount; ++Count)
	{
		CreateObject(false);
	}
}

AActor* const AObjectPool::CreateObject(bool bInUse, bool bHide)
{
	if (ObjectToPool && (CurrentObjectCount < MaxObjectCount || MaxObjectCount == 0))
	{
		auto Object = GetWorld()->SpawnActor(ObjectToPool);
		Object->SetActorHiddenInGame(bHide);
#if WITH_EDITOR
		Object->SetFolderPath(ObjectFolder);
#endif

		CurrentObjectCount = CurrentObjectCount + 1;

		if (bInUse)
		{
			InUseObjects.Add(Object);
		}
		else
		{
			NotInUseObjects.Add(Object);
		}

		return Object;
	}

	return nullptr;
}

AActor* const AObjectPool::GetObject(bool bHideOnCreation)
{
	if (NotInUseObjects.Num() > 0)
	{
		auto const Object = NotInUseObjects.Pop();

		InUseObjects.Add(Object);

		return Object;
	}
	else
	{
		return CreateObject(true, bHideOnCreation);
	}
}

void AObjectPool::ReturnObject(AActor* const Object)
{
	check(IsValid(Object) && "AObjectPool::ReturnObject : Non valid pointer is given!");

	auto Index = int32{};
	if (InUseObjects.Find(Object, Index))
	{
		InUseObjects.Remove(Object);
		NotInUseObjects.Add(Object);
	}
}

void AObjectPool::Empty()
{
	CurrentObjectCount = 0;

	for (auto& Object : NotInUseObjects)
	{
		Object->Destroy();
	}

	NotInUseObjects.Empty();

	for (auto& Object : InUseObjects)
	{
		Object->Destroy();
	}

	InUseObjects.Empty();
}

