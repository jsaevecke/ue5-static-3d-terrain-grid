// Copyright 2018, Julien Saevecke, All rights reserved.

#include "GridHex.h"
#include "Components/InstancedStaticMeshComponent.h"

AGridHex::AGridHex()
{
	SetActorTickEnabled(false);
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	HexISM = CreateDefaultSubobject<UInstancedStaticMeshComponent>(FName("HexISM"));
	HexISM->SetCollisionProfileName(FName("Grid"));
}