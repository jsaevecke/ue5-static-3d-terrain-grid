// Copyright 2018, Julien Saevecke, All rights reserved.

//TODO: When to use const and & properly

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridSystem.generated.h"

class UInstancedStaticMeshComponent;

USTRUCT(BlueprintType)
struct FHexTileData
{
	GENERATED_BODY()
public:
	FHexTileData() {}

	UPROPERTY(VisibleAnywhere, Category = "Position")
	FIntVector GridCoordinates;
	UPROPERTY(VisibleAnywhere, Category = "Position")
	FVector WorldCoordinates;
	UPROPERTY(VisibleAnywhere, Category = "Properties")
	FName HexType;
};

USTRUCT(BlueprintType)
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
	UPROPERTY(VisibleAnywhere)
	float Radius = 0.f;
	//The Width of the hexagon
	UPROPERTY(VisibleAnywhere)
	float Width = 0.f;
	//The Height of the hexagon
	UPROPERTY(VisibleAnywhere)
	float Height = 0.f;
	//The vertical distance between hexagons
	UPROPERTY(VisibleAnywhere)
	float VerticalSpacing = 0.f;
	//The horizontal distance between hexagons
	UPROPERTY(VisibleAnywhere)
	float HorizontalSpacing = 0.f;
};

UCLASS()
class BLACKBOX_WAR_PROJECT_API AGridSystem : public AActor
{
	GENERATED_BODY()
public:	
	AGridSystem();

	//Visual representation of the grid
	//TODO: Should be a map <string, ism> - the key is an identifier for the ism, that identifier helps to spawn the correct ism's while loading a level
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hexagon|Static Meshes")
	UInstancedStaticMeshComponent* HexBase = nullptr;

protected:
	//Gets called before begin play - used to calculate properties that are based on default values set in the blueprint
	virtual void PostInitProperties() override;
	virtual void BeginPlay() override;

public:
	//Sets the layout for the hexagon grid, can be overriden by blueprints to generate various grids for gameplay
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Layout")
	void SetupGridLayout();

	UFUNCTION(BlueprintCallable, Category = "Layout")
	FIntVector GetGridDimensions();

	//TODO: Does it belong here?
	//TODO:WorldToGrid may only function if its the exact world location of the hex
	UFUNCTION(BlueprintCallable, Category = "Hexagon")
	FVector HexGridLocationToWorldLocation(FIntVector coordinates);
	UFUNCTION(BlueprintCallable, Category = "Hexagon")
	FIntVector HexWorldLocationToGridLocation(FVector worldLocation);

	UFUNCTION(BlueprintCallable, Category = "Hexagon")
	FHexMeasurements GetHexMeasurements();
	
protected:
	//How many columns and rows the grid has or how big the grid is
	UPROPERTY(EditAnywhere, Category = "Layout")
	FIntVector Dimensions;

	//Calculated measurements of the base hexagon mesh - needed to place the hexagons on the right location
	UPROPERTY(VisibleAnywhere, Category = "Hexagon")
	FHexMeasurements HexMeasurements;

	//Data representation of the grid
	UPROPERTY(VisibleAnywhere, Category = "Hexagon|Data")
	TArray<FHexTileData> HexTileData;
};
