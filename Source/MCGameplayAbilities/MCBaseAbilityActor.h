// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MCBaseAbilityActor.generated.h"

/*
 * Give the spawn actor task a base actor to work with, so that if we want to insert anything in
 * the actor at spawn time based on the instigating ability or the target data it's easy
 */
UCLASS()
class MCGAMEPLAYABILITIES_API AMCBaseAbilityActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMCBaseAbilityActor(const FObjectInitializer& Initializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
