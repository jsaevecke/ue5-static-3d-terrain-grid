/*
Copyright 2018, Julien Saevecke, All rights reserved.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridSystem.generated.h"

class UInstancedStaticMeshComponent;
class UStaticMeshComponent;
class AGridHex;

/*
FHexData describes the underlying data(properties) of each hex in the grid.
*/
USTRUCT(BlueprintType, Category = "Grid|Hexagon|Data")
struct FHexData
{
	GENERATED_BODY()
public:
	FHexData() {}

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

/*
FHexMeasurments calculates all dimensions information of each hex and the gap between them
given a radius.

@TODO: It is possible to create an FHexMeasurments Object without calculating all variables
*/
USTRUCT(BlueprintType, Category = "Grid|Hexagon|Data")
struct FHexMeasurements
{
	GENERATED_BODY()
public:
	FHexMeasurements()
	{

	}
	//Calculates all dimension- and distance between hex variables based on a given hex radius.
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

/*
AGridSystem represents the visual grid and the underlying data for each hex. It holds
functions to get this information and to setup the grid.

@TODO: It needs to be initializable via code only - in the moment variables like Dimensions are only set
			   in the editor
*/
UCLASS(hidecategories = ("Default", "Actor Tick", "Rendering", "Input", "Actor", "LOD", "Replication"))
class BLACKBOX_WAR_PROJECT_API AGridSystem : public AActor
{
	GENERATED_BODY()
public:	
	AGridSystem();

	/*
	@returns: (columns, rows) that make up the grid
	*/
	UFUNCTION(BlueprintCallable, Category = "Grid|Layout")
	FIntVector GetGridDimensions() const;

	
	/*
	Calculates the world location of a given instance hex grid location.
	@param: grid coordinates of the instanced hex as a (column, row) vector
	@returns: world location of the hex
	@TODO: does it belong here?
	*/
	UFUNCTION(BlueprintCallable, Category = "Grid|Hexagon|Helper")
	FVector HexGridLocationToWorldLocation(const FIntVector coordinates) const;

	/*
	Calculates the grid location of a given instance hex world location.
	@param: world location of the hex
	@returns: grid coordinates of the instanced hex as a (column, row) vector
	@note: may only function if its the exact world location of the hex's origin
	@TODO: does it belong here?
	*/
	UFUNCTION(BlueprintCallable, Category = "Grid|Hexagon|Helper")
	FIntVector HexWorldLocationToGridLocation(const FVector worldLocation) const;

	/*
	@returns: measurements of the base hex model 
	*/
	UFUNCTION(BlueprintCallable, Category = "Grid|Hexagon|Data")
	FHexMeasurements GetHexMeasurements();
	/*
	@returns: the whole data grid that holds information about each instanced hex
	*/
	UFUNCTION(BlueprintCallable, Category = "Grid|Hexagon|Data")
	const TArray<FHexData>& GetHexGridData() const;

	/*
	@returns: the whole visual grid
	*/
	UFUNCTION(BlueprintCallable, Category = "Grid|Hexagon|ISM")
	const TMap<FName, AGridHex*>& GetHexISMs() const;
	/*
	@param: identifier of an GridHex Actor
	@returns: identified GridHex Actor
	*/
	UFUNCTION(BlueprintCallable, Category = "Grid|Hexagon|ISM")
	AGridHex* const GetHexISM(const FName identifier) const;
	
protected:
	/*
	Gets called before BeginPlay() and calculates the required measurements via the HexMeasurementsModel to setup the grid.
	*/
	virtual void PostInitProperties() override;
	/*
	Only calls SetupGridLayout().
	*/
	virtual void BeginPlay() override;

	/*
	Sets up the grids layout (rectangular/circular/..) based on the implementation. Gets called in BeginPlay().
	This function is meant to be overridden in the blueprint or with C++ code.
	
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grid|Layout")
	void SetupGridLayout();

public:
	//Base Model for calculating measurements
	UPROPERTY(EditAnywhere, Category = "Grid|Hexagon|Data")
	UStaticMeshComponent* HexMeasurementsModel = nullptr;

	//Represents all available GridHex Actors that can be used to generate the grid
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid|Hexagon|ISM")
	TArray<TSubclassOf<AGridHex>> HexAssets;

protected:
	//How many columns and rows the grid has or how big the grid is
	UPROPERTY(EditAnywhere, Category = "Grid|Layout")
	FIntVector Dimensions;

	//Represents the measurements for each hex in the grid. Required for setting up the grid.
	UPROPERTY(VisibleAnywhere, Category = "Grid|Hexagon|Data")
	FHexMeasurements HexMeasurements;

	//Data representation of the grid. Represents all underlying data for each hex.
	UPROPERTY(VisibleAnywhere, Category = "Grid|Hexagon|Data")
	TArray<FHexData> HexGridData;

	/*
	Actual visual representation of the grid - each hex model is represented as an GridHex Actor.
	Each GridHex Actor uses its identifier as key. 
	*/
	UPROPERTY(VisibleAnywhere, Category = "Grid|Hexagon|ISM")
	TMap<FName, AGridHex*> InstancedHexISMs;
};
