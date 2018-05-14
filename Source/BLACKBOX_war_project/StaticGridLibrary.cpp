// Copyright 2018, Julien Saevecke, All rights reserved.

#include "StaticGridLibrary.h"
#include "Components/PrimitiveComponent.h"

bool UStaticGridLibrary::IsWalkable(UWorld* const world, const FVector& worldPosition, float lineTraceLength, bool& isNonWalkable, TArray<TEnumAsByte<ECollisionChannel>> walkableObjects, TArray<TEnumAsByte<ECollisionChannel>> nonWalkableObjects)
{
	TArray<TEnumAsByte<ECollisionChannel>> objectsToCheck = TArray<TEnumAsByte<ECollisionChannel>>();
	objectsToCheck.Append(walkableObjects);
	objectsToCheck.Append(nonWalkableObjects);

	FVector start = FVector::UpVector  * lineTraceLength / 2.f + worldPosition;
	FVector end = (-1) * FVector::UpVector * lineTraceLength + start;

	FCollisionObjectQueryParams objectParams;
	for (auto collisionChannel : objectsToCheck)
	{
		objectParams.AddObjectTypesToQuery(collisionChannel);
	}

	const FName TraceTag("MyTraceTag");

	world->DebugDrawTraceTag = TraceTag;

	FCollisionQueryParams params;
	params.TraceTag = TraceTag;

	FHitResult hitResult;
	if (world->LineTraceSingleByObjectType(hitResult, start, end, objectParams, params))
	{
		TEnumAsByte<ECollisionChannel> collisionChannel = hitResult.Component->GetCollisionObjectType();
		
		int32 isWalkable = walkableObjects.Find(collisionChannel);
		if (isWalkable != INDEX_NONE)
		{
			isNonWalkable = false;
			return true;
		}
		
		isNonWalkable = true;
		return false;
	}

	return false;
}

