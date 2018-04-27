// Copyright 2018, Julien Saevecke, All rights reserved.

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

	UPROPERTY(VisibleAnywhere)
	uint8 X;
	UPROPERTY(VisibleAnywhere)
	uint8 Y;
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

	//Should be a map <string, ism> - the key is an identifier for the ism, that identifier helps to spawn the correct ism's while loading a level
	//Visual representation of the grid
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

	//Calculates hexagon world position based on the column and row
	UFUNCTION(BlueprintCallable, Category = "Hexagon")
	FVector GetHexWorldLocation(FIntVector coordinates);
	UFUNCTION(BlueprintCallable, Category = "Hexagon")
	FHexMeasurements GetHexMeasurements();

	//TODO: When to use const and & properly
private:

protected:
	//How many columns and rows the grid has
	UPROPERTY(EditAnywhere, Category = "Layout")
	FIntVector Dimensions;

	UPROPERTY(VisibleAnywhere, Category = "Hexagon")
	FHexMeasurements HexMeasurements;

	//Data representation of the grid
	UPROPERTY(VisibleAnywhere, Category = "Hexagon|Data")
	TArray<FHexTileData> HexTileData;
};
