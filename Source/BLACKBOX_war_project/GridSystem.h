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
	AGridSystem();

protected:
	//Gets called before begin play - used to calculate properties that are based on default values set in the blueprint
	virtual void PostInitProperties() override;
	virtual void BeginPlay() override;

public:
	//Sets the layout for the hexagon grid, can be overriden by blueprints to generate various grids for gameplay
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Layout")
	void SetupGridLayout();

	//Calculates hexagon world position based on the column and row using cube coordinates
	UFUNCTION(BlueprintCallable, Category = "Hexagon")
	FVector getHexagonWorldLocation(uint8 column, uint8 row);

	//TODO: When to use const and & properly 
private:
	//How many columns the grid has
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	uint8 columns = 1;
	//How many rows the grid has
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	uint8 rows = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hexagon", meta = (AllowPrivateAccess = "true"))
	class UInstancedStaticMeshComponent* hexagonBase = nullptr;

	//The radius of the hexagon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hexagon", meta = (AllowPrivateAccess = "true"))
	float size = 0.f;
	//The Width of the hexagon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hexagon", meta = (AllowPrivateAccess = "true"))
	float width = 0.f;
	//The Height of the hexagon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hexagon", meta = (AllowPrivateAccess = "true"))
	float height = 0.f;
	//The vertical distance between hexagons
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hexagon", meta = (AllowPrivateAccess = "true"))
	float verticalSpacing = 0.f;
	//The horizontal distance between hexagons
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hexagon", meta = (AllowPrivateAccess = "true"))
	float horizontalSpacing = 0.f;
};
