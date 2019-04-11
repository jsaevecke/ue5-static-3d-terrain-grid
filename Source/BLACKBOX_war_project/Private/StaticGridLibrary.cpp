// Copyright 2018, Julien Saevecke, All rights reserved.

#include "StaticGridLibrary.h"
#include "Components/PrimitiveComponent.h"

const TArray<FVector> UStaticGridLibrary::TileDirections = { FVector{1, 0, -1}, FVector{1, -1, 0}, FVector{0, -1, 1}, FVector{-1, 0, 1}, FVector{-1, 1, 0}, FVector{0, 1, -1} };

bool UStaticGridLibrary::IsWalkable(UWorld* const World, const FVector& WorldPosition, const float LineTraceLength, bool& bNonWalkable, const TArray<TEnumAsByte<ECollisionChannel>>& WalkableObjects, const TArray<TEnumAsByte<ECollisionChannel>>& NonWalkableObjects)
{
	check(IsValid(World) && "Non valid World pointer - without a World checking positions whether they are walkable is not possble!");

	auto ObjectsToCheck = TArray<TEnumAsByte<ECollisionChannel>>{};
	ObjectsToCheck.Append(WalkableObjects);
	ObjectsToCheck.Append(NonWalkableObjects);

	auto Start = FVector::UpVector  * LineTraceLength / 2.f + WorldPosition;
	auto End = (-1) * FVector::UpVector * LineTraceLength + Start;

	auto ObjectParams = FCollisionObjectQueryParams{};
	for (auto CollisionChannel : ObjectsToCheck)
	{
		ObjectParams.AddObjectTypesToQuery(CollisionChannel);
	}

	const auto TraceTag = FName{ "MyTraceTag" };

#if WITH_EDITOR
	World->DebugDrawTraceTag = TraceTag;
#endif

	auto Params = FCollisionQueryParams{};
	Params.TraceTag = TraceTag;

	auto HitResult = FHitResult{};
	if (World->LineTraceSingleByObjectType(HitResult, Start, End, ObjectParams, Params))
	{
		auto CollisionChannel = TEnumAsByte<ECollisionChannel>{ HitResult.Component->GetCollisionObjectType() };

		auto WalkableIndex = int32{ WalkableObjects.Find(CollisionChannel) };
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

