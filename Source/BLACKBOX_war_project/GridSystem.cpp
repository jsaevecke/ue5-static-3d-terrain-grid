// Fill out your copyright notice in the Description page of Project Settings.

#include "GridSystem.h"
#include "Engine/StaticMesh.h"
#include "Components/InstancedStaticMeshComponent.h"


// Sets default values
AGridSystem::AGridSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	hexagonBase = CreateDefaultSubobject<UInstancedStaticMeshComponent>(FName("hexagonBase"));
}

// Called when the game starts or when spawned
void AGridSystem::BeginPlay()
{
	Super::BeginPlay();

	ensure(hexagonBase != nullptr);

	UStaticMesh* staticMesh = hexagonBase->GetStaticMesh();

	ensure(staticMesh != nullptr);

	size = staticMesh->GetBoundingBox().Max.X;
	height = size * 2.f;
	width = sqrt(3.f) / 2.f * height;
	horizontalSpacing = width;
	verticalSpacing = height * 0.75f;

	UE_LOG(LogTemp, Warning, TEXT("Hex Props - size: %f - height: %f - width: %f - vsp: %f - hsp: %f"), size, height, width, verticalSpacing, horizontalSpacing);

	Setup();
}

void AGridSystem::Setup()
{
	for (uint8 currentColumn = 0; currentColumn < columns; ++currentColumn)
	{
		for (uint8 currentRow = 0; currentRow < rows; ++currentRow)
		{
			FVector location{ horizontalSpacing * (currentColumn + currentRow/2.f), verticalSpacing * currentRow, 25.f };
			UE_LOG(LogTemp, Warning, TEXT("Hex Props - Position: %s"), *location.ToString());
			FTransform newInstanceTransform{ location };
			hexagonBase->AddInstance(newInstanceTransform);
		}
	}
}

