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
	HexBase->SetCollisionProfileName(FName("BlockAll"));
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

FIntVector AGridSystem::GetGridDimensions()
{
	return Dimensions;
}
FVector AGridSystem::GetHexWorldLocation(FIntVector coordinates)
{
	return FVector{ HexMeasurements.HorizontalSpacing * (coordinates.X + coordinates.Y / 2.f), HexMeasurements.VerticalSpacing * coordinates.Y, 25.f };
}
FHexMeasurements AGridSystem::GetHexMeasurements()
{
	return HexMeasurements;
}

void AGridSystem::SetupGridLayout_Implementation()
{
	if (HexBase)
	{
		for (uint8 currentColumn = 0; currentColumn < Dimensions.X; ++currentColumn)
		{
			for (uint8 currentRow = 0; currentRow < Dimensions.Y; ++currentRow)
			{
				int32 index = HexBase->AddInstance(FTransform{ GetHexWorldLocation(FIntVector(currentColumn, currentRow, 0.f)) });
			}
		}
	}
}

