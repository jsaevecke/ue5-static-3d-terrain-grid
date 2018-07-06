// Copyright 2018, Julien Saevecke, All rights reserved.

#include "StaticGridLibrary.h"
#include "Components/PrimitiveComponent.h"

const TArray<FVector> UStaticGridLibrary::TileDirections = { FVector{1, 0, -1}, FVector{1, -1, 0}, FVector{0, -1, 1}, FVector{-1, 0, 1}, FVector{-1, 1, 0}, FVector{0, 1, -1} };

bool UStaticGridLibrary::IsWalkable(UWorld* const World, const FVector& WorldPosition, const float LineTraceLength, bool& bNonWalkable, const TArray<TEnumAsByte<ECollisionChannel>>& WalkableObjects, const TArray<TEnumAsByte<ECollisionChannel>>& NonWalkableObjects)
{
	check(IsValid(World) && "Non valid World pointer - without a World checking positions whether they are walkable is not possble!");

	TArray<TEnumAsByte<ECollisionChannel>> ObjectsToCheck{ TArray<TEnumAsByte<ECollisionChannel>>{} };
	ObjectsToCheck.Append(WalkableObjects);
	ObjectsToCheck.Append(NonWalkableObjects);

	FVector Start{ FVector::UpVector  * LineTraceLength / 2.f + WorldPosition };
	FVector End{ (-1) * FVector::UpVector * LineTraceLength + Start };

	FCollisionObjectQueryParams ObjectParams{};
	for (auto CollisionChannel : ObjectsToCheck)
	{
		ObjectParams.AddObjectTypesToQuery(CollisionChannel);
	}

	const FName TraceTag{ "MyTraceTag" };

	World->DebugDrawTraceTag = TraceTag;

	FCollisionQueryParams Params{};
	Params.TraceTag = TraceTag;

	FHitResult HitResult{};
	if (World->LineTraceSingleByObjectType(HitResult, Start, End, ObjectParams, Params))
	{
		TEnumAsByte<ECollisionChannel> CollisionChannel{ HitResult.Component->GetCollisionObjectType() };

		int32 WalkableIndex{ WalkableObjects.Find(CollisionChannel) };
		if (WalkableIndex != INDEX_NONE)
		{
			bNonWalkable = false;
			return true;
		}

		bNonWalkable = true;
		return false;
	}

	return false;
}

