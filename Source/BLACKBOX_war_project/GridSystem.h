// Copyright 2018, Julien Saevecke, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridSystem.generated.h"

class UInstancedStaticMeshComponent;
class UStaticMeshComponent;
class AGridHex;

USTRUCT(BlueprintType, Category = "Grid|Hexagon|Data")
struct FHexTileData
{
	GENERATED_BODY()
public:
	FHexTileData() {}

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category ="Grid|Hexagon|Data|Tile")
	FIntVector GridCoordinates;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Grid|Hexagon|Data|Tile")
	FVector WorldCoordinates;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Grid|Hexagon|Data|Tile")
	int32 ISMIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Grid|Hexagon|Data|Tile")
	int32 DataIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Grid|Hexagon|Data|Tile")
	FName HexType;
	
};

USTRUCT(BlueprintType, Category = "Grid|Hexagon|Data")
struct FHexMeasurements
{
	GENERATED_BODY()
public:
	FHexMeasurements()
	{

	}
	FHexMeasurements(float radius)
	{
		Radius = radius;
		Height = Radius * 2.f;
		Width = sqrt(3.f) / 2.f * Height;
		HorizontalSpacing = Width;
		VerticalSpacing = Height * 0.75f;
	}

	//The radius of the hexagon
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Grid|Hexagon|Data|Measurements")
	float Radius = 0.f;
	//The Width of the hexagon
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Grid|Hexagon|Data|Measurements")
	float Width = 0.f;
	//The Height of the hexagon
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Grid|Hexagon|Data|Measurements")
	float Height = 0.f;
	//The vertical distance between hexagons
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Grid|Hexagon|Data|Measurements")
	float VerticalSpacing = 0.f;
	//The horizontal distance between hexagon
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Grid|Hexagon|Data|Measurements")
	float HorizontalSpacing = 0.f;
};

UCLASS(hidecategories = ("Default", "Actor Tick", "Rendering", "Input", "Actor", "LOD", "Replication"))
class BLACKBOX_WAR_PROJECT_API AGridSystem : public AActor
{
	GENERATED_BODY()
public:	
	AGridSystem();

	//Sets the layout for the hexagon grid, can be overriden by blueprints to generate various grids for gameplay
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grid|Layout")
	void SetupGridLayout();

	UFUNCTION(BlueprintCallable, Category = "Grid|Layout")
	FIntVector GetGridDimensions() const;

	//TODO: Does it belong here?
	//TODO:WorldToGrid may only function if its the exact world location of the hex
	UFUNCTION(BlueprintCallable, Category = "Grid|Hexagon|Helper")
	FVector HexGridLocationToWorldLocation(const FIntVector coordinates) const;
	UFUNCTION(BlueprintCallable, Category = "Grid|Hexagon|Helper")
	FIntVector HexWorldLocationToGridLocation(const FVector worldLocation) const;

	UFUNCTION(BlueprintCallable, Category = "Grid|Hexagon|Data")
	FHexMeasurements GetHexMeasurements();
	UFUNCTION(BlueprintCallable, Category = "Grid|Hexagon|Data")
	const TArray<FHexTileData>& GetHexGridData() const;

	UFUNCTION(BlueprintCallable, Category = "Grid|Hexagon|ISM")
	const TMap<FName, AGridHex*>& GetHexISMs() const;
	UFUNCTION(BlueprintCallable, Category = "Grid|Hexagon|ISM")
	AGridHex* const GetHexISM(const FName identifier) const;
	
protected:
	//Gets called before begin play - used to calculate properties that are based on default values set in the blueprint
	virtual void PostInitProperties() override;
	virtual void BeginPlay() override;

public:
	//Base Model for calculating measurements
	UPROPERTY(EditAnywhere, Category = "Grid|Hexagon|Data")
	UStaticMeshComponent* HexMeasurementsModel = nullptr;

	//Visual representation of the grid
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid|Hexagon|ISM")
	TArray<TSubclassOf<AGridHex>> HexAssets;

protected:
	//How many columns and rows the grid has or how big the grid is
	UPROPERTY(EditAnywhere, Category = "Grid|Layout")
	FIntVector Dimensions;

	//Calculated measurements of the base hexagon mesh - needed to place the hexagons on the right location
	UPROPERTY(VisibleAnywhere, Category = "Grid|Hexagon|Data")
	FHexMeasurements HexMeasurements;

	//Data representation of the grid
	UPROPERTY(VisibleAnywhere, Category = "Grid|Hexagon|Data")
	TArray<FHexTileData> HexGridData;

	UPROPERTY(VisibleAnywhere, Category = "Grid|Hexagon|ISM")
	TMap<FName, AGridHex*> InstancedHexISMs;
};
