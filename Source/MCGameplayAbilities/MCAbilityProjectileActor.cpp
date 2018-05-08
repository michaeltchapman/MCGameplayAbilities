// Fill out your copyright notice in the Description page of Project Settings.

#include "MCAbilityProjectileActor.h"
#include "Components/SphereComponent.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AbilitySystemBlueprintLibrary.h"


AMCAbilityProjectileActor::AMCAbilityProjectileActor(const FObjectInitializer& Initializer)
	: Super(Initializer)
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(CollisionComponent);
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
}

void AMCAbilityProjectileActor::BeginPlay()
{
	Super::BeginPlay();

	if (bSendOverlapEvents)
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMCAbilityProjectileActor::OnOverlap);
	}
	if (bSendHitEvents)
	{
		CollisionComponent->OnComponentHit.AddDynamic(this, &AMCAbilityProjectileActor::OnHit);
	}
}

void AMCAbilityProjectileActor::OnOverlap(UPrimitiveComponent* ThisComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->GetUniqueID() != Instigator->GetUniqueID())
	{
		FGameplayEventData Data;
		Data.Target = OtherActor;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Instigator, FGameplayTag::RequestGameplayTag(FName(TEXT("Event.Overlap"))), Data);
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Instigator, FGameplayTag::RequestGameplayTag(FName(TEXT("Event.ProjectileDestruction"))), FGameplayEventData());
		MovementComponent->StopMovementImmediately();
		CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Destroy();
	}
}

void AMCAbilityProjectileActor::OnHit(UPrimitiveComponent* ThisComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != Instigator)
	{
		FGameplayEventData Data;
		Data.Target = OtherActor;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Instigator, FGameplayTag::RequestGameplayTag(FName(TEXT("Event.Hit"))), Data);
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Instigator, FGameplayTag::RequestGameplayTag(FName(TEXT("Event.ProjectileDestruction"))), FGameplayEventData());
		MovementComponent->StopMovementImmediately();
		Destroy();
	}
}
