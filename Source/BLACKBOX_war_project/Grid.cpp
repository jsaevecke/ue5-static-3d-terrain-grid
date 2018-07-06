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

	// Todo: 
	auto Sqrt3 = FMath::Sqrt(3.f);

	GridToWorldX =  FVector2D(Sqrt3, Sqrt3 / 2.f);
	GridToWorldY = FVector2D(0.f, 3.f / 2.f);

	WorldToGridX = FVector2D(Sqrt3 / 3.f, -1.f / 3.f);
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

	for (auto Row = int32{ 1 }; Row < VerticalTileCount; ++Row)
	{
		auto Indices = UStaticGridLibrary::CalculateOffsetIndices(Row, HorizontalTileCount);

		if (Row % 2 == 0)
		{
			Indices.X = Indices.X + 1;
		}
		else
		{
			Indices.Y = Indices.Y - 1;
		}

		for (auto Column = static_cast<int32>(Indices.X); Column < Indices.Y; ++Column)
		{
			AddTile(FVector2D{ static_cast<float>(Column), static_cast<float>(Row) });
		}
	}
}
void AGrid::Destroy()
{
	Tiles.Empty();
}

FVector2D AGrid::ConvertGridToWorld(const FVector2D& GridPosition)
{
	auto Offset = FVector2D{ MinLandscapeBounds.X, MinLandscapeBounds.Y };

	auto X = GridToWorldX * GridPosition;
	auto Y = GridToWorldY * GridPosition;

	return Offset + FVector2D{ X.X + X.Y, Y.X + Y.Y } * OuterRadius;
}
FVector2D AGrid::ConvertWorldToGrid(const FVector2D& WorldPosition)
{
	auto Offset = FVector2D{ MinLandscapeBounds.X, MinLandscapeBounds.Y };
	auto OffsetWorldPosition = WorldPosition - Offset;

	auto X = WorldToGridX * OffsetWorldPosition;
	auto Y = WorldToGridY * OffsetWorldPosition;

	auto GridPosition = FVector2D{ X.X + X.Y, Y.X + Y.Y };

	GridPosition = GridPosition / OuterRadius;

	auto Fractional = FVector{ GridPosition.X, GridPosition.Y, -GridPosition.X - GridPosition.Y };
	auto Rounded = FVector{ static_cast<float>(FMath::RoundToInt(Fractional.X)), static_cast<float>(FMath::RoundToInt(Fractional.Y)), static_cast<float>(FMath::RoundToInt(Fractional.Z)) };
	auto Diff = FVector{ Fractional - Rounded };

	Diff.X = FMath::Abs(Diff.X);
	Diff.Y = FMath::Abs(Diff.Y);
	Diff.Z = FMath::Abs(Diff.Z);

	if (Diff.X > Diff.Y && Diff.X > Diff.Z)
	{
		Rounded.X = -Rounded.Y - Rounded.Z;
	}
	else if (Diff.Y > Diff.Z)
	{
		Rounded.Y = -Rounded.X - Rounded.Z;
	}
	else
	{
		Rounded.Z = -Rounded.X - Rounded.Y;
	}

	return FVector2D{ Rounded.X, Rounded.Y };
}

const FTileData& AGrid::AddTile(const FVector2D& GridPosition)
{
	auto WorldPosition = ConvertGridToWorld(GridPosition);

	auto bNonWalkable = false;
	auto bHit = UStaticGridLibrary::IsWalkable(GetWorld(), FVector(WorldPosition.X, WorldPosition.Y, 0.f), LineTraceLength, bNonWalkable, WalkableObjects, NonWalkableObjects);

	auto TileData = FTileData{};
	TileData.GridPosition = FVector{ GridPosition.X, GridPosition.Y, -GridPosition.X - GridPosition.Y };
	TileData.WorldPosition = FVector{ WorldPosition.X, WorldPosition.Y, 0.f };

	if (bHit)
	{
		TileData.State = ETileState::Walkable;
	}
	else if (bNonWalkable)
	{
		TileData.State = ETileState::Obstructed;
	}

	if (bHit || bNonWalkable)
	{
		auto Hash = UStaticGridLibrary::GetTileHash(GridPosition);
		TileData.Hash = Hash;
		Tiles.Add(TileData.Hash, TileData);
		return Tiles[TileData.Hash];
	}

	return InvalidTile;
}
const bool AGrid::RemoveTile(const FVector2D& GridPosition)
{
	auto Hash = UStaticGridLibrary::GetTileHash(GridPosition);

	if (Tiles.Contains(Hash))
	{
		Tiles.Remove(Hash);
		return true;
	}

	return false;
}
const bool AGrid::UpdateTile(const FTileData& TileData)
{
	auto Hash = UStaticGridLibrary::GetTileHash(FVector2D(TileData.GridPosition.X, TileData.GridPosition.Y));

	if (Tiles.Contains(Hash))
	{
		Tiles[Hash] = TileData;
		return true;
	}

	return false;
}
const FTileData& AGrid::GetTile(const FVector2D& GridPosition)
{
	auto Hash = UStaticGridLibrary::GetTileHash(GridPosition);

	auto TileData = Tiles.Find(Hash);

	if (TileData)
	{
		return *TileData;
	}

	return InvalidTile;
}
const TMap<FString, FTileData>& AGrid::GetAllTiles()
{
	return Tiles;
}

void AGrid::FindPath(const FTileData& Start, const FTileData& End, TArray<FTileData>& outPath)
{
	auto Frontier = TArray<FTileData>{};
	Frontier.Add(Start);

	auto From = TMap<FString, FTileData>{};
	From.Add(Start.Hash, FTileData{});

	while (Frontier.Num() > 0)
	{
		auto Current = Frontier[0];
		Frontier.RemoveAt(0);

		if (Current.Hash.Equals(End.Hash))
		{
			auto& Parent = From[Current.Hash];
			while (Parent.State != ETileState::NotValid)
			{
				outPath.Add(Parent);
				Parent = From[Parent.Hash];
			}
			return;
		}

		auto Neighbors = TArray<FTileData>{};
		GetNeighbors(FVector2D{Current.GridPosition}, Neighbors);

		for (auto& Neighbor : Neighbors)
		{
			if (!From.Contains(Neighbor.Hash) && Neighbor.State == ETileState::Walkable)
			{
				Frontier.Add(Neighbor);
				From.Add(Neighbor.Hash, Current);
			}
		}
	}
}
// TODO: Think about returning just Vectors instead of whole FTileData structs?
void AGrid::GetTilesInRange(const FVector2D& Origin, const uint8 Range, TArray<FTileData>& outTiles)
{
	for (auto Column = int16{ -Range }; Column <= Range; ++Column)
	{
		for (auto Row = int16{ FMath::Max<int16>(-Range, -Column - Range) }; Row <= FMath::Min<int16>(Range, -Column + Range); ++Row)
		{
			auto Hash = UStaticGridLibrary::GetTileHash(Origin + FVector2D(Column, Row));

			auto TileData = Tiles.Find(Hash);
			
			if (TileData != nullptr)
			{
				outTiles.Add(*TileData);
			}
		}
	}
}
void AGrid::GetNeighbors(const FVector2D& Origin, TArray<FTileData>& outNeighbors)
{
	for (auto direction : UStaticGridLibrary::TileDirections)
	{
		auto Hash = UStaticGridLibrary::GetTileHash(Origin + FVector2D(direction));

		auto Neighbor = Tiles.Find(Hash);

		if (Neighbor != nullptr)
		{
			outNeighbors.Add(*Neighbor);
		}
	}
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

	auto DecalActor = Cast<ADecalActor>(GetWorld()->SpawnActor(BPTileDecal));
	auto DecalSize = FVector{ DecalActor->GetDecal()->DecalSize };
	OuterRadius = FMath::Max(DecalSize.Y, DecalSize.Z);
	DecalActor->Destroy();

	VerticalSpacing = OuterRadius * 1.5f;
	HorizontalSpacing = FMath::Sqrt(3.f) * OuterRadius;

	for (auto ActorItr = TActorIterator<ALandscape>{ GetWorld() }; ActorItr; ++ActorItr)
	{
		auto Landscape = *ActorItr;

		check(IsValid(Landscape) && "No Landscape is found in the level!");

		auto Origin = FVector{};
		auto Extent = FVector{};
		Landscape->GetActorBounds(false, Origin, Extent);
		MinLandscapeBounds = Origin - Extent;
		MaxLandscapeBounds = Origin + Extent;
		AbsoluteLandscapeSize = MaxLandscapeBounds - MinLandscapeBounds;
		HorizontalTileCount = static_cast<uint8>(AbsoluteLandscapeSize.X / HorizontalSpacing);
		VerticalTileCount = static_cast<uint8>(AbsoluteLandscapeSize.Y / VerticalSpacing);

		break;
	}
}