// Copyright 2018, Julien Saevecke, All rights reserved.

#pragma once

#include <limits>

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/World.h"
#include "StaticGridLibrary.generated.h"


UENUM(BlueprintType, Category="Grid")
enum class ETileState : uint8
{
	NotValid,
	Walkable,
	Obstructed,
	RangeIndicator,
	Path,
	PathStart,
	PathEnd,
	Default = NotValid
};

USTRUCT(BlueprintType, Category = "Grid")
struct FTileNavData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Grid|Tile|Nav")
	float GCost;
	UPROPERTY(BlueprintReadWrite, Category = "Grid|Tile|Nav")
	float HCost;

	FTileNavData() 
		: GCost(std::numeric_limits<float>::max()), HCost(std::numeric_limits<float>::max())
	{
	}
};

USTRUCT(BlueprintType, Category="Grid")
struct FTileData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Grid|Tile|Data")
	FVector GridPosition;
	UPROPERTY(BlueprintReadWrite, Category = "Grid|Tile|Data")
	FVector WorldPosition;
	UPROPERTY(BlueprintReadWrite, Category = "Grid|Tile|Data")
	ETileState State;
	UPROPERTY(BlueprintReadWrite, Category = "Grid|Tile|Data")
	FTileNavData NavData;

	FTileData()
		: GridPosition(0.f, 0.f, 0.f), WorldPosition(0.f, 0.f, 0.f), State(ETileState::NotValid), NavData()
	{

	}
};

/**
 * 
 */
UCLASS()
class BLACKBOX_WAR_PROJECT_API UStaticGridLibrary : public UObject
{
	GENERATED_BODY()

	const static TArray<FVector> TileDirections;

public:
	UFUNCTION(BlueprintCallable, Category="Grid")
	static FORCEINLINE bool IsValidTile(const FTileData& tileData)
	{
		return !(tileData.State == ETileState::NotValid);
	}

	UFUNCTION(BlueprintCallable, Category = "Grid")
	static FORCEINLINE FVector2D CalculateOffsetIndices(int32 row, int32 width)
	{
		int32 x = FMath::FloorToInt(row / 2.f);
		int32 y = width - x;
		return FVector2D(x * (-1), y);
	}

	UFUNCTION(BlueprintCallable, Category = "Grid")
	static FORCEINLINE FString GetTileHash(const FVector2D& gridPosition)
	{
		return FString::FromInt(static_cast<int>(gridPosition.X)) + "_" + FString::FromInt(static_cast<int>(gridPosition.Y));
	}

	UFUNCTION(BlueprintCallable, Category = "Grid")
	static bool IsWalkable(UWorld* const world, const FVector& worldPosition, float lineTraceLength, bool& isNonWalkable, TArray<TEnumAsByte<ECollisionChannel>> walkableObjects, TArray<TEnumAsByte<ECollisionChannel>> nonWalkableObjects);
};