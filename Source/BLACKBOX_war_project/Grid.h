// Copyright 2018, Julien Saevecke, All rights reserved.

// TODO : Comments, Function Descriptions, Class Descriptions
// TODO : ETileState refactor
// TODO : Path- Start and End cannot be placed on obstructed tiles
// TODO : Need Edge Information for pathfinding or more raycasts?
// TODO : Exclude Obstructed Tiles
// TODO : Multiplayer
// TODO : Camera
// Priority: Unimportant right now
// TODO : A* instead of simple breadth search
// TODO : Dynamic A*
// TODO : Smartphone/Tablet Support

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
	FVector2D ConvertGridToWorld(const FVector2D&  GridPosition);
	UFUNCTION(BlueprintCallable, Category = "Grid")
	FVector2D ConvertWorldToGrid(const FVector2D&  WorldPosition);

	UFUNCTION(BlueprintCallable, Category = "Grid|Tile")
	const FTileData& AddTile(const FVector2D& GridPosition);
	UFUNCTION(BlueprintCallable, Category = "Grid|Tile")
	bool RemoveTile(const FVector2D& GridPosition);
	UFUNCTION(BlueprintCallable, Category = "Grid|Tile")
	bool UpdateTile(const FTileData& TileData);
	UFUNCTION(BlueprintCallable, Category = "Grid|Tile")
	const FTileData& GetTile(const FVector2D& GridPosition);
	UFUNCTION(BlueprintCallable, Category = "Grid|Tile")
	const TMap<FString, FTileData>& GetAllTiles();

	UFUNCTION(BlueprintCallable, Category = "Grid|Tile")
	void FindPath(const FTileData& Start, const FTileData& End, TArray<FTileData>& outPath);
	UFUNCTION(BlueprintCallable, Category = "Grid|Tile")
	void GetTilesInRange(const FVector2D& Origin, uint8 Range, TArray<FTileData>& outTiles);
	UFUNCTION(BlueprintCallable, Category = "Grid|Tile")
	void GetNeighbors(const FVector2D& Origin, TArray<FTileData>& outNeighbors);

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
