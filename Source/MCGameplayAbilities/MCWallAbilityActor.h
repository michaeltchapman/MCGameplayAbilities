// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MCBaseAbilityActor.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "MCWallAbilityActor.generated.h"

/**
 * 
 */
UCLASS()
class MCGAMEPLAYABILITIES_API AMCWallAbilityActor : public AMCBaseAbilityActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AMCWallAbilityActor(const FObjectInitializer& Initializer);
	virtual void BeginPlay() override;

	UFUNCTION()
    void OnDeath(const struct FGameplayModifierEvaluatedData& Data, const struct FGameplayEffectSpec& Spec);

	class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AbilitySystem")
	class UMCAbilitySystemComponent* AbilitySystem;

	UPROPERTY()
	class UMCAttributeSet* AttributeSet;
	
	
};
