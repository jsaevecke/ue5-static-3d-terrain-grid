// Copyright 2018, Julien Saevecke, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridHex.generated.h"

class UInstancedStaticMeshComponent;

UCLASS(hidecategories = ("Actor Tick", "Rendering", "Input", "Actor", "LOD", "Replication"))
class BLACKBOX_WAR_PROJECT_API AGridHex : public AActor
{
	GENERATED_BODY()
	
public:	
	AGridHex();

public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hexagon")
	FName Type = "Unknown";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hexagon")
	FName Identifier = "Unknown";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hexagon")
	UInstancedStaticMeshComponent* HexISM = nullptr;
	/*
	This grid is used to convert an InstancedStaticMesh index to an HexDataGrid index.
	HexDataGridIndices[ISMIndex] = HexDataGridIndex
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hexagon")
	TArray<int32> HexDataGridIndices;
};
