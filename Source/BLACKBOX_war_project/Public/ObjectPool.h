// Copyright 2018, Julien Saevecke, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectPool.generated.h"

UCLASS()
class BLACKBOX_WAR_PROJECT_API AObjectPool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectPool();

	UFUNCTION(BlueprintCallable, Category = "ObjectPool")
	AActor* const GetObject(bool bHideOnCreation = true);
	UFUNCTION(BlueprintCallable, Category = "ObjectPool")
	void ReturnObject(AActor* const Object);
	UFUNCTION(BlueprintCallable, Category = "ObjectPool")
	void Empty();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void ReserveObjects();
	UFUNCTION()
	AActor* const CreateObject(bool bInUse, bool bHide = true);

public:		
	UPROPERTY(EditDefaultsOnly, Category = "Pool|Config")
	TSubclassOf<AActor> ObjectToPool;
	UPROPERTY(EditDefaultsOnly, Category = "Pool|Config")
	uint16 ReserveObjectCount;
	UPROPERTY(EditDefaultsOnly, Category = "Pool|Config")
	uint16 MaxObjectCount;

	UPROPERTY(EditDefaultsOnly, Category = "Pool|Config|WorldOutliner")
	FName PoolFolder;
	UPROPERTY(EditDefaultsOnly, Category = "Pool|Config|WorldOutliner")
	FName ObjectFolder;

private:
	UPROPERTY()
	TArray<AActor*> NotInUseObjects;
	UPROPERTY()
	TArray<AActor*> InUseObjects;
	UPROPERTY()
	uint16 CurrentObjectCount;
};
