// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MCBaseAbilityActor.h"
#include "MCAbilityProjectileActor.generated.h"

/**
 * 
 */
UCLASS()
class MCGAMEPLAYABILITIES_API AMCAbilityProjectileActor : public AMCBaseAbilityActor
{
	GENERATED_BODY()

public:
	AMCAbilityProjectileActor(const FObjectInitializer& Initializer);
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* ThisComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* ThisComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ProjectileActor)
	class UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ProjectileActor)
	class USphereComponent* CollisionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ProjectileActor)
	bool bSendOverlapEvents;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ProjectileActor)
	bool bSendHitEvents;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ProjectileActor)
	bool bTreatOverlapAsHit;
	
};
