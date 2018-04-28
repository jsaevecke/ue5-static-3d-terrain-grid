// Copyright 2018, Julien Saevecke, All rights reserved.

#include "GridSystem.h"
#include "Engine/StaticMesh.h"
#include "Components/InstancedStaticMeshComponent.h"


// Sets default values
AGridSystem::AGridSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it. Is overidden bei blueprint?
	PrimaryActorTick.bCanEverTick = false;
	//this->SetActorTicksEnables(true/false)
	//PrimaryActorTick.bStartWithTickEnabled

	HexBase = CreateDefaultSubobject<UInstancedStaticMeshComponent>(FName("HexBase"));
	HexBase->SetCollisionProfileName(FName("Grid"));
	Dimensions = FIntVector(1.f);
}

void AGridSystem::PostInitProperties()
{
	Super::PostInitProperties();

	//Calculate properties needed for proper positioning of tiles in the grid
	if (HexBase && HexBase->GetStaticMesh())
	{
		HexMeasurements = FHexMeasurements(HexBase->GetStaticMesh()->GetBoundingBox().Max.Y);
	}
}

void AGridSystem::BeginPlay()
{
	Super::BeginPlay();

	SetupGridLayout();
}

FIntVector AGridSystem::GetGridDimensions() const
{
	return Dimensions;
}
FVector AGridSystem::HexGridLocationToWorldLocation(const FIntVector coordinates) const
{
	return FVector{ HexMeasurements.HorizontalSpacing * (coordinates.X + coordinates.Y / 2.f), HexMeasurements.VerticalSpacing * coordinates.Y, 25.f };
}
FIntVector AGridSystem::HexWorldLocationToGridLocation(const FVector worldLocation) const
{
	int32 coordY = (int32)(worldLocation.Y / HexMeasurements.VerticalSpacing);
	int32 coordX = (int32)((worldLocation.X / HexMeasurements.HorizontalSpacing) - coordY / 2.f);
	return FIntVector{ coordX, coordY, 1 };
}
FHexMeasurements AGridSystem::GetHexMeasurements()
{
	return HexMeasurements;
}
const TArray<FHexTileData>&  AGridSystem::GetHexGridData() const
{
	return HexGridData;
}

void AGridSystem::SetupGridLayout_Implementation()
{
	if (HexBase)
	{
		HexGridData.Reserve(Dimensions.X * Dimensions.Y);

		for (uint8 currentColumn = 0; currentColumn < Dimensions.X; ++currentColumn)
		{
			for (uint8 currentRow = 0; currentRow < Dimensions.Y; ++currentRow)
			{
				FVector worldLocation{ HexGridLocationToWorldLocation({ currentColumn, currentRow, 0 }) };
				FTransform hexTransform{ worldLocation };

				int32 index = HexBase->AddInstance(hexTransform);
				
				FHexTileData hexTileData;
				hexTileData.GridCoordinates = { currentColumn, currentRow, 0 };
				hexTileData.WorldCoordinates = worldLocation;
				hexTileData.Index = index;
				hexTileData.HexType = { "Base" };

				HexGridData.Add(hexTileData);
			}
		}
	}
}

