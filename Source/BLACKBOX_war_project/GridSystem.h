// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridSystem.generated.h"

UCLASS()
class BLACKBOX_WAR_PROJECT_API AGridSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridSystem();

private:
	UPROPERTY(EditAnywhere, Category = "Hexagons")
	class UInstancedStaticMeshComponent* hexagonBase = nullptr;

	UPROPERTY(EditAnywhere, Category = "Grid")
	uint8 columns = 1;
	UPROPERTY(EditAnywhere, Category = "Grid")
	uint8 rows = 1;

	//Tooltip Yey!
	UPROPERTY(VisibleAnywhere, Category = "Tile")
	float size = 0.f;
	UPROPERTY(VisibleAnywhere, Category = "Tile")
	float width = 0.f;
	UPROPERTY(VisibleAnywhere, Category = "Tile")
	float height = 0.f;
	UPROPERTY(VisibleAnywhere, Category = "Tile")
	float verticalSpacing = 0.f;
	UPROPERTY(VisibleAnywhere, Category = "Tile")
	float horizontalSpacing = 0.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//TODO: BlueprintNativeEvent - default implementation but blueprint overridable
	UFUNCTION()
	virtual void Setup();
};
