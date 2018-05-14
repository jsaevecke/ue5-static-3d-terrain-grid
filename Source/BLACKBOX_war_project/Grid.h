// Copyright 2018, Julien Saevecke, All rights reserved.

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
	// Sets default values for this actor's properties
	AGrid();
	~AGrid();

	UFUNCTION(BlueprintCallable, Category="Grid|Conversion")
	FVector2D ConvertGridToWorld(const FVector2D&  gridPosition);
	UFUNCTION(BlueprintCallable, Category = "Grid|Conversion")
	FVector2D ConvertWorldToGrid(const FVector2D&  worldPosition);

	UFUNCTION(BlueprintCallable, Category = "Grid|Tile")
	FTileData GetTileData(const FVector2D& gridPosition);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void DetermineMeasurements();
	UFUNCTION()
	void SpawnGrid();
	UFUNCTION()
	void DestroyGrid();

	UFUNCTION()
	void AddTile(const FVector2D& gridPosition);

public:
	UPROPERTY(EditDefaultsOnly, Category = "Grid|Tile")
	TSubclassOf<ADecalActor> BPTileDecal;

	UPROPERTY(EditDefaultsOnly, Category = "WorldOutliner")
	FName GridFolder;
	UPROPERTY(EditDefaultsOnly, Category = "WorldOutliner")
	FName PoolFolder;

	UPROPERTY(EditDefaultsOnly, Category = "Grid|Tile")
	TArray<TEnumAsByte<ECollisionChannel>> WalkableObjects;
	UPROPERTY(EditDefaultsOnly, Category = "Grid|Tile")
	TArray<TEnumAsByte<ECollisionChannel>> NonWalkableObjects;
	UPROPERTY(EditDefaultsOnly, Category = "Grid|Tile")
	float LineTraceLength;

private:
	UPROPERTY()
	AObjectPool* TilePool = nullptr;
	UPROPERTY()
	float OuterRadius;
	UPROPERTY()
	float VerticalSpacing;
	UPROPERTY()
	float HorizontalSpacing;

	UPROPERTY()
	TMap<FString, FTileData> GridData;

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
