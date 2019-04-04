// Copyright 2018, Julien Saevecke, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "TileData.generated.h"


UENUM(BlueprintType, Category = "Grid")
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
struct BLACKBOX_WAR_PROJECT_API FTileData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Tile|Data")
	FVector GridPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Tile|Data")
	FVector WorldPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Tile|Data")
	ETileState State;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Tile|Nav")
	float GCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Tile|Nav")
	float HCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Tile|Nav")
	float FCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Tile|Data")
	FString Hash;

	FTileData* Parent;

	FTileData()
		: GridPosition(0.f, 0.f, 0.f), WorldPosition(0.f, 0.f, 0.f), State(ETileState::NotValid), GCost(0.f), HCost(0.f), FCost(0.f), Hash("0_0"), Parent(nullptr)
	{

	}
};
