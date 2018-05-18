// Copyright 2018, Julien Saevecke, All rights reserved.

// TODO: Comments, Function Descriptions, Class Descriptions
// TODO: Pathfinding
// TODO: Tile Update Function
// TODO: Default Walkable/NonWalkable/Raycast Length
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

	UFUNCTION(BlueprintCallable, Category = "Grid")
	const FTileData& AddTile(const FVector2D& gridPosition);

	UFUNCTION(BlueprintCallable, Category = "Grid")
	const FTileData& GetTile(const FVector2D& gridPosition);
	UFUNCTION(BlueprintCallable, Category = "Grid")
	const TMap<FString, FTileData>& GetAllTiles();

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
