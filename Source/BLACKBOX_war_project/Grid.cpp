// Copyright 2018, Julien Saevecke, All rights reserved.

#include "Grid.h"
#include "ObjectPool.h"
#include "Engine/World.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"
#include "EngineUtils.h"
#include "Landscape.h"

AGrid::AGrid()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	float sqrt3 = sqrtf(3.f);

	GridToWorldX = FVector2D(sqrt3, sqrt3 / 2.f);
	GridToWorldY = FVector2D(0.f, 3.f / 2.f);

	WorldToGridX = FVector2D(sqrt3 / 3.f, -1.f / 3.f);
	WorldToGridY = FVector2D(0.f, 2.f / 3.f);
}
AGrid::~AGrid()
{
	Destroy();
}

void AGrid::Spawn()
{
	Destroy();

	DetermineMeasurements();

	for (int32 row = 1; row < VerticalTileCount; ++row)
	{
		FVector2D indices = UStaticGridLibrary::CalculateOffsetIndices(row, HorizontalTileCount);

		if (row % 2 == 0)
		{
			indices.X = indices.X + 1;
		}
		else
		{
			indices.Y = indices.Y - 1;
		}

		for (int32 column = indices.X; column < indices.Y; ++column)
		{
			AddTile(FVector2D(column, row));
		}
	}
}
void AGrid::Destroy()
{
	Tiles.Empty();
}

FVector2D AGrid::ConvertGridToWorld(const FVector2D& gridPosition)
{
	FVector2D offset = FVector2D(MinLandscapeBounds.X, MinLandscapeBounds.Y);

	FVector2D x = GridToWorldX * gridPosition;
	FVector2D y = GridToWorldY * gridPosition;

	return offset + FVector2D(x.X + x.Y, y.X + y.Y) * OuterRadius;
}
FVector2D AGrid::ConvertWorldToGrid(const FVector2D& worldPosition)
{
	FVector2D offset = FVector2D(MinLandscapeBounds.X, MinLandscapeBounds.Y);
	FVector2D offsetWorldPosition = worldPosition - offset;

	FVector2D x = WorldToGridX * offsetWorldPosition;
	FVector2D y = WorldToGridY * offsetWorldPosition;

	FVector2D gridPosition = FVector2D(x.X + x.Y, y.X + y.Y);

	gridPosition = gridPosition / OuterRadius;

	FVector fractional = FVector(gridPosition.X, gridPosition.Y, -gridPosition.X - gridPosition.Y);
	FVector rounded = FVector(FMath::RoundToInt(fractional.X), FMath::RoundToInt(fractional.Y), FMath::RoundToInt(fractional.Z));
	FVector diff = fractional - rounded;

	diff.X = FMath::Abs(diff.X);
	diff.Y = FMath::Abs(diff.Y);
	diff.Z = FMath::Abs(diff.Z);

	if (diff.X > diff.Y && diff.X > diff.Z)
	{
		rounded.X = -rounded.Y - rounded.Z;
	}
	else if (diff.Y > diff.Z)
	{
		rounded.Y = -rounded.X - rounded.Z;
	}
	else
	{
		rounded.Z = -rounded.X - rounded.Y;
	}

	return FVector2D(rounded.X, rounded.Y);
}

const FTileData& AGrid::AddTile(const FVector2D& gridPosition)
{
	FVector2D worldPosition = ConvertGridToWorld(gridPosition);

	bool isNonWalkable = false;
	bool hit = UStaticGridLibrary::IsWalkable(GetWorld(), FVector(worldPosition.X, worldPosition.Y, 0.f), LineTraceLength, isNonWalkable, WalkableObjects, NonWalkableObjects);

	FTileData tileData;
	tileData.GridPosition = FVector(gridPosition.X, gridPosition.Y, -gridPosition.X - gridPosition.Y);
	tileData.WorldPosition = FVector(worldPosition.X, worldPosition.Y, 0.f);

	if (hit)
	{
		tileData.State = ETileState::Walkable;
	}
	else if (isNonWalkable)
	{
		tileData.State = ETileState::Obstructed;
	}


	if (hit || isNonWalkable)
	{
		FString hash = UStaticGridLibrary::GetTileHash(gridPosition);
		Tiles.Add(hash, tileData);
		return Tiles[hash];
	}

	return InvalidTile;
}
bool AGrid::RemoveTile(const FVector2D& gridPosition)
{
	FString hash = UStaticGridLibrary::GetTileHash(gridPosition);

	if (Tiles.Contains(hash))
	{
		Tiles.Remove(hash);
		return true;
	}

	return false;
}
bool AGrid::UpdateTile(const FTileData& tileData)
{
	FString hash = UStaticGridLibrary::GetTileHash(FVector2D(tileData.GridPosition.X, tileData.GridPosition.Y));

	if (Tiles.Contains(hash))
	{
		Tiles[hash] = tileData;
		return true;
	}

	return false;
}
const FTileData& AGrid::GetTile(const FVector2D& gridPosition)
{
	FString hash = UStaticGridLibrary::GetTileHash(gridPosition);

	FTileData* tileData = Tiles.Find(hash);

	if (tileData)
	{
		return *tileData;
	}

	return InvalidTile;
}
const TMap<FString, FTileData>& AGrid::GetAllTiles()
{
	return Tiles;
}

void AGrid::FindPath(const FTileData& start, const FTileData& end)
{

}
void AGrid::GetTilesInRange(const FTileData& origin, uint8 range)
{

}

void AGrid::BeginPlay()
{
	Super::BeginPlay();

	this->SetFolderPath(GridFolder);

	Spawn();
}

void AGrid::DetermineMeasurements()
{
	check(IsValid(BPTileDecal) && "No TileDecal is set - it is needed to determine the positions of the tiles!");

	ADecalActor* decalActor = Cast<ADecalActor>(GetWorld()->SpawnActor(BPTileDecal));
	FVector decalSize = decalActor->GetDecal()->DecalSize;
	OuterRadius = FMath::Max(decalSize.Y, decalSize.Z);
	decalActor->Destroy();

	VerticalSpacing = OuterRadius * 1.5f;
	HorizontalSpacing = sqrtf(3.f) * OuterRadius;

	for (TActorIterator<ALandscape> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ALandscape *landscape = *ActorItr;

		check(IsValid(landscape) && "No landscape is found in the level!");

		FVector origin, extent;
		landscape->GetActorBounds(false, origin, extent);
		MinLandscapeBounds = origin - extent;
		MaxLandscapeBounds = origin + extent;
		AbsoluteLandscapeSize = MaxLandscapeBounds - MinLandscapeBounds;
		HorizontalTileCount = static_cast<uint8>(AbsoluteLandscapeSize.X / HorizontalSpacing);
		VerticalTileCount = static_cast<uint8>(AbsoluteLandscapeSize.Y / VerticalSpacing);

		break;
	}
}