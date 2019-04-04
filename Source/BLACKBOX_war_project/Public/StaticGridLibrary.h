// Copyright 2018, Julien Saevecke, All rights reserved.

#pragma once

#include <limits>

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/World.h"
#include "TileData.h"
#include "StaticGridLibrary.generated.h"

/**
 * 
 */
UCLASS()
class BLACKBOX_WAR_PROJECT_API UStaticGridLibrary : public UObject
{
	GENERATED_BODY()
public:
	const static TArray<FVector> TileDirections;

	UFUNCTION(BlueprintCallable, Category="Grid")
	static FORCEINLINE bool IsValidTile(const FTileData& TileData)
	{
		return !(TileData.State == ETileState::NotValid);
	}

	UFUNCTION(BlueprintCallable, Category = "Grid")
	static FORCEINLINE FVector2D CalculateOffsetIndices(int32 Row, int32 Width)
	{
		auto X = FMath::FloorToInt(Row / 2.f);
		auto Y = Width - X;
		return FVector2D{ static_cast<float>(X * (-1)), static_cast<float>(Y) };
	}

	UFUNCTION(BlueprintCallable, Category = "Grid")
	static FORCEINLINE FString GetTileHash(const FVector2D& GridPosition)
	{
		return FString::FromInt(static_cast<int>(GridPosition.X)) + "_" + FString::FromInt(static_cast<int>(GridPosition.Y));
	}

	UFUNCTION(BlueprintCallable, Category = "Grid")
	static bool IsWalkable(UWorld* const World, const FVector& WorldPosition, const float LineTraceLength, bool& bNonWalkable, const TArray<TEnumAsByte<ECollisionChannel>>& WalkableObjects, const TArray<TEnumAsByte<ECollisionChannel>>& NonWalkableObjects);
};