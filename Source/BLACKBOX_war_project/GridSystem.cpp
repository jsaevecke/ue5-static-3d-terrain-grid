// Copyright 2018, Julien Saevecke, All rights reserved.

#include "GridSystem.h"
#include "Engine/StaticMesh.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GridHex.h"

AGridSystem::AGridSystem()
{
	SetActorTickEnabled(false);
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	HexMeasurementsModel = CreateDefaultSubobject<UStaticMeshComponent>(FName("HexBase"));
	HexMeasurementsModel->SetHiddenInGame(true);
	Dimensions = FIntVector(1.f);
}

void AGridSystem::PostInitProperties()
{
	Super::PostInitProperties();

	//Calculate properties needed for proper positioning of tiles in the grid
	if (HexMeasurementsModel && HexMeasurementsModel->GetStaticMesh())
	{
		HexMeasurements = FHexMeasurements(HexMeasurementsModel->GetStaticMesh()->GetBoundingBox().Max.Y);
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
const TArray<FHexData>&  AGridSystem::GetHexGridData() const
{
	return HexGridData;
}

const TMap<FName, AGridHex*>& AGridSystem::GetHexISMs() const
{
	return InstancedHexISMs;
}
AGridHex* const AGridSystem::GetHexISM(const FName identifier) const
{
	if (InstancedHexISMs.Contains(identifier))
	{
		return InstancedHexISMs[identifier];
	}

	return nullptr;
}

void AGridSystem::SetupGridLayout_Implementation()
{
	for (auto hexAsset : HexAssets)
	{
		AGridHex* instancedHexISM = Cast<AGridHex>(GetWorld()->SpawnActor(hexAsset.Get()));
		InstancedHexISMs.Add(instancedHexISM->Identifier, instancedHexISM);
	}

	if (HexAssets.Num() > 0)
	{
		HexGridData.Reserve(Dimensions.X * Dimensions.Y);

		int32 dataIndex = 0;
		for (uint8 currentColumn = 0; currentColumn < Dimensions.X; ++currentColumn)
		{
			for (uint8 currentRow = 0; currentRow < Dimensions.Y; ++currentRow)
			{
				AGridHex* instancedHexISM = nullptr;

				if (currentColumn % 2 == 0 || currentRow % 2 == 0)
				{
					instancedHexISM = InstancedHexISMs["Base_01"];
				}
				else
				{
					instancedHexISM = InstancedHexISMs["Base_02"];
				}

				FVector worldLocation{ HexGridLocationToWorldLocation({ currentColumn, currentRow, 0 }) };
				FTransform hexTransform{ worldLocation };
				
				int32 ismIndex = instancedHexISM->HexISM->AddInstance(hexTransform);
				
				instancedHexISM->HexDataGridIndices.Add(dataIndex);

				FHexData hexTileData;
				hexTileData.GridCoordinates = { currentColumn, currentRow, 0 };
				hexTileData.WorldCoordinates = worldLocation;
				hexTileData.ISMIndex = ismIndex;
				hexTileData.DataIndex = dataIndex;
				hexTileData.HexType = instancedHexISM->Type;

				HexGridData.Add(hexTileData);

				++dataIndex;
			}
		}
	}
}

