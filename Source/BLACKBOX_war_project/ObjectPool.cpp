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

	ReserveObjects();
}

void AObjectPool::ReserveObjects()
{
	for (uint8 count = 0; count < ReserveObjectCount; ++count)
	{
		CreateObject(false);
	}
}

AActor* AObjectPool::CreateObject(bool isInUse)
{
	if (ObjectToPool && (CurrentObjectCount < MaxObjectCount || MaxObjectCount == 0))
	{
		AActor* object = GetWorld()->SpawnActor(ObjectToPool);
		object->SetActorHiddenInGame(true);
		CurrentObjectCount = CurrentObjectCount + 1;

		if (isInUse)
		{
			InUseObjects.Add(object);
		}
		else
		{
			NotInUseObjects.Add(object);
		}

		return object;
	}

	return nullptr;
}

AActor* AObjectPool::GetObject()
{
	if (NotInUseObjects.Num() > 0)
	{
		AActor* object = NotInUseObjects.Pop();

		InUseObjects.Add(object);

		return object;
	}
	else
	{
		return CreateObject(true);
	}
}

void AObjectPool::ReturnObject(AActor* object)
{
	int32 index;
	if (InUseObjects.Find(object, index))
	{
		InUseObjects.Remove(object);
		NotInUseObjects.Add(object);
	}
}

void AObjectPool::Empty()
{
	CurrentObjectCount = 0;

	for (auto& object : NotInUseObjects)
	{
		object->Destroy();
	}

	NotInUseObjects.Empty();

	for (auto& object : InUseObjects)
	{
		object->Destroy();
	}

	InUseObjects.Empty();
}

