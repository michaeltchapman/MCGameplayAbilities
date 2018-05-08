// Fill out your copyright notice in the Description page of Project Settings.

#include "MCBaseAbilityActor.h"


// Sets default values
AMCBaseAbilityActor::AMCBaseAbilityActor(const FObjectInitializer& Initializer)
	: Super(Initializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMCBaseAbilityActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMCBaseAbilityActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

