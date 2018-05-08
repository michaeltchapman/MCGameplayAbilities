// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MCAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class MCGAMEPLAYABILITIES_API UMCAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	UMCAbilitySystemComponent(const FObjectInitializer& ObjectInitializer);

	UPROPERTY()
	TArray<TSubclassOf<class UGameplayAbility>> ActiveSlots;
	TArray<FGameplayAbilitySpecHandle> ActiveHandles;

	// Use the global index to add an ability to a slot
	bool AddAbilityToSlot(int32 Slot, int32 AbilityIndex);

	// Add an ability to a slot by specifying the class directly
	bool AddAbilityToSlot(int32 Slot, TSubclassOf<class UGameplayAbility> Ability);

	bool RemoveAbilityFromSlot(int32 Slot);
	
};
