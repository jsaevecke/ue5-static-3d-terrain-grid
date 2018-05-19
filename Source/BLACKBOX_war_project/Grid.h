// Copyright 2018, Julien Saevecke, All rights reserved.

// TODO: Comments, Function Descriptions, Class Descriptions
// TODO: Pathfinding
// TODO: Default Walkable/NonWalkable/Raycast Length
// TODO: Variable naming convention (C++&Blueprint)
// TODO: ETileState refactor
// TODO : Path- Start and End cannot be placed on obstructed tiles
// TODO : TArray works with references
// TODO : Need Edge Information for pathfinding
// TODO : A* instead of simple breadth search
// TODO : Exclude Obstructed Tiles

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StaticGridLibrary.h"
#include "Grid.generated.h"

class AObjectPool;
class ADecalActor;

UCLASS()
class BLACKBOX_WAR_PROJECT_API AGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	AGrid();
	~AGrid();

	UFUNCTION(BlueprintCallable, Category = "Grid")
	void Spawn();
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void Destroy();

	UFUNCTION(BlueprintCallable, Category="Grid")
	FVector2D ConvertGridToWorld(const FVector2D&  gridPosition);
	UFUNCTION(BlueprintCallable, Category = "Grid")
	FVector2D ConvertWorldToGrid(const FVector2D&  worldPosition);

	UFUNCTION(BlueprintCallable, Category = "Grid|Tile")
	const FTileData& AddTile(const FVector2D& gridPosition);
	UFUNCTION(BlueprintCallable, Category = "Grid|Tile")
	bool RemoveTile(const FVector2D& gridPosition);
	UFUNCTION(BlueprintCallable, Category = "Grid|Tile")
	bool UpdateTile(const FTileData& tileData);
	UFUNCTION(BlueprintCallable, Category = "Grid|Tile")
	const FTileData& GetTile(const FVector2D& gridPosition);
	UFUNCTION(BlueprintCallable, Category = "Grid|Tile")
	const TMap<FString, FTileData>& GetAllTiles();

	UFUNCTION(BlueprintCallable, Category = "Grid|Tile")
	void FindPath(const FTileData& start, const FTileData& end, TArray<FTileData>& outPath);
	UFUNCTION(BlueprintCallable, Category = "Grid|Tile")
	void GetTilesInRange(const FVector2D& origin, uint8 range, TArray<FTileData>& outTiles);
	UFUNCTION(BlueprintCallable, Category = "Grid|Tile")
	void GetNeighbors(const FVector2D& origin, TArray<FTileData>& outNeighbors);

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void DetermineMeasurements();

public:
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TSubclassOf<ADecalActor> BPTileDecal;

	UPROPERTY(EditDefaultsOnly, Category = "Config|Organisation")
	FName GridFolder;

	UPROPERTY(EditDefaultsOnly, Category = "Config|Navigation")
	TArray<TEnumAsByte<ECollisionChannel>> WalkableObjects;
	UPROPERTY(EditDefaultsOnly, Category = "Config|Navigation")
	TArray<TEnumAsByte<ECollisionChannel>> NonWalkableObjects;
	UPROPERTY(EditDefaultsOnly, Category = "Config|Navigation")
	float LineTraceLength;

private:
	UPROPERTY()
	TMap<FString, FTileData> Tiles;
	UPROPERTY()
	FTileData InvalidTile;

	UPROPERTY()
	float OuterRadius;
	UPROPERTY()
	float VerticalSpacing;
	UPROPERTY()
	float HorizontalSpacing;
	UPROPERTY()
	FVector MinLandscapeBounds;
	UPROPERTY()
	FVector MaxLandscapeBounds;
	UPROPERTY()
	FVector AbsoluteLandscapeSize;
	UPROPERTY()
	uint8 VerticalTileCount;
	UPROPERTY()
	uint8 HorizontalTileCount;

	UPROPERTY()
	FVector2D GridToWorldX;
	UPROPERTY()
	FVector2D GridToWorldY;
	UPROPERTY()
	FVector2D WorldToGridX;
	UPROPERTY()
	FVector2D WorldToGridY;
};
