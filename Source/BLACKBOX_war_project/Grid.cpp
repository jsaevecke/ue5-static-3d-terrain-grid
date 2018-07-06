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

	float Sqrt3{ FMath::Sqrt(3.f) };

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

	for (int32 Row{ 1 }; Row < VerticalTileCount; ++Row)
	{
		FVector2D Indices{ UStaticGridLibrary::CalculateOffsetIndices(Row, HorizontalTileCount) };

		if (Row % 2 == 0)
		{
			Indices.X = Indices.X + 1;
		}
		else
		{
			Indices.Y = Indices.Y - 1;
		}

		for (int32 Column{ static_cast<int32>(Indices.X) }; Column < Indices.Y; ++Column)
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
	FVector2D Offset{ FVector2D(MinLandscapeBounds.X, MinLandscapeBounds.Y) };

	FVector2D X{ GridToWorldX * GridPosition };
	FVector2D Y{ GridToWorldY * GridPosition };

	return Offset + FVector2D{ X.X + X.Y, Y.X + Y.Y } * OuterRadius;
}
FVector2D AGrid::ConvertWorldToGrid(const FVector2D& WorldPosition)
{
	FVector2D Offset{ FVector2D(MinLandscapeBounds.X, MinLandscapeBounds.Y) };
	FVector2D OffsetWorldPosition{ WorldPosition - Offset };

	FVector2D X{ WorldToGridX * OffsetWorldPosition };
	FVector2D Y{ WorldToGridY * OffsetWorldPosition };

	FVector2D GridPosition = FVector2D{ X.X + X.Y, Y.X + Y.Y };

	GridPosition = GridPosition / OuterRadius;

	FVector Fractional{ GridPosition.X, GridPosition.Y, -GridPosition.X - GridPosition.Y };
	FVector Rounded{ static_cast<float>(FMath::RoundToInt(Fractional.X)), static_cast<float>(FMath::RoundToInt(Fractional.Y)), static_cast<float>(FMath::RoundToInt(Fractional.Z)) };
	FVector Diff{ Fractional - Rounded };

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
	FVector2D WorldPosition{ ConvertGridToWorld(GridPosition) };

	bool bNonWalkable{ false };
	bool bHit{ UStaticGridLibrary::IsWalkable(GetWorld(), FVector(WorldPosition.X, WorldPosition.Y, 0.f), LineTraceLength, bNonWalkable, WalkableObjects, NonWalkableObjects) };

	FTileData TileData{};
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
		FString Hash{ UStaticGridLibrary::GetTileHash(GridPosition) };
		TileData.Hash = Hash;
		Tiles.Add(TileData.Hash, TileData);
		return Tiles[TileData.Hash];
	}

	return InvalidTile;
}
const bool AGrid::RemoveTile(const FVector2D& GridPosition)
{
	FString Hash{ UStaticGridLibrary::GetTileHash(GridPosition) };

	if (Tiles.Contains(Hash))
	{
		Tiles.Remove(Hash);
		return true;
	}

	return false;
}
const bool AGrid::UpdateTile(const FTileData& TileData)
{
	FString Hash{ UStaticGridLibrary::GetTileHash(FVector2D(TileData.GridPosition.X, TileData.GridPosition.Y)) };

	if (Tiles.Contains(Hash))
	{
		Tiles[Hash] = TileData;
		return true;
	}

	return false;
}
const FTileData& AGrid::GetTile(const FVector2D& GridPosition)
{
	FString Hash{ UStaticGridLibrary::GetTileHash(GridPosition) };

	FTileData* TileData{ Tiles.Find(Hash) };

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
	TArray<FTileData> Frontier{};
	Frontier.Add(Start);

	TMap<FString, FTileData> From{};
	From.Add(Start.Hash, FTileData{});

	while (Frontier.Num() > 0)
	{
		FTileData Current{ Frontier[0] };
		Frontier.RemoveAt(0);

		if (Current.Hash.Equals(End.Hash))
		{
			FTileData& Parent{ From[Current.Hash] };
			while (Parent.State != ETileState::NotValid)
			{
				outPath.Add(Parent);
				Parent = From[Parent.Hash];
			}
			return;
		}

		TArray<FTileData> Neighbors{};
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
	for (int16 Column{ -Range }; Column <= Range; ++Column)
	{
		for (int16 Row{ FMath::Max<int16>(-Range, -Column - Range) }; Row <= FMath::Min<int16>(Range, -Column + Range); ++Row)
		{
			FString Hash{ UStaticGridLibrary::GetTileHash(Origin + FVector2D(Column, Row)) };

			FTileData* TileData{ Tiles.Find(Hash) };
			
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
		FString Hash{ UStaticGridLibrary::GetTileHash(Origin + FVector2D(direction)) };

		FTileData* Neighbor{ Tiles.Find(Hash) };

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

	ADecalActor* DecalActor{ Cast<ADecalActor>(GetWorld()->SpawnActor(BPTileDecal)) };
	FVector DecalSize{ DecalActor->GetDecal()->DecalSize };
	OuterRadius = FMath::Max(DecalSize.Y, DecalSize.Z);
	DecalActor->Destroy();

	VerticalSpacing = OuterRadius * 1.5f;
	HorizontalSpacing = FMath::Sqrt(3.f) * OuterRadius;

	for (TActorIterator<ALandscape> ActorItr{ GetWorld() }; ActorItr; ++ActorItr)
	{
		ALandscape *Landscape{ *ActorItr };

		check(IsValid(Landscape) && "No Landscape is found in the level!");

		FVector Origin{}, Extent{};
		Landscape->GetActorBounds(false, Origin, Extent);
		MinLandscapeBounds = Origin - Extent;
		MaxLandscapeBounds = Origin + Extent;
		AbsoluteLandscapeSize = MaxLandscapeBounds - MinLandscapeBounds;
		HorizontalTileCount = static_cast<uint8>(AbsoluteLandscapeSize.X / HorizontalSpacing);
		VerticalTileCount = static_cast<uint8>(AbsoluteLandscapeSize.Y / VerticalSpacing);

		break;
	}
}