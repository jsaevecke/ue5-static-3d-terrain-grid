// Fill out your copyright notice in the Description page of Project Settings.

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

	hexagonBase = CreateDefaultSubobject<UInstancedStaticMeshComponent>(FName("hexagonBase"));
	hexagonBase->SetCollisionProfileName(FName("BlockAll"));
}

void AGridSystem::PostInitProperties()
{
	Super::PostInitProperties();

	//Calculate properties needed for proper positioning of tiles in the grid
	if (hexagonBase && hexagonBase->GetStaticMesh())
	{
		size = hexagonBase->GetStaticMesh()->GetBoundingBox().Max.X;
		height = size * 2.f;
		width = sqrt(3.f) / 2.f * height;
		horizontalSpacing = width;
		verticalSpacing = height * 0.75f;
	}
}

void AGridSystem::BeginPlay()
{
	Super::BeginPlay();

	SetupGridLayout();
}

FVector AGridSystem::getHexagonWorldLocation(uint8 column, uint8 row)
{
	return FVector{ horizontalSpacing * (column + row / 2.f), verticalSpacing * row, 25.f };
}

void AGridSystem::SetupGridLayout_Implementation()
{
	if (hexagonBase)
	{
		for (uint8 currentColumn = 0; currentColumn < columns; ++currentColumn)
		{
			for (uint8 currentRow = 0; currentRow < rows; ++currentRow)
			{
				hexagonBase->AddInstance(FTransform{ getHexagonWorldLocation(currentColumn, currentRow) });
			}
		}
	}
}

