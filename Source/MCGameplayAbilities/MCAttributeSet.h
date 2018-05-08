// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "DelegateCombinations.h"
#include "MCAttributeSet.generated.h"

//const struct FGameplayModifierEvaluatedData &Data, const struct FGameplayEffectSpec &Spec
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDeathDelegate, const FGameplayModifierEvaluatedData&, Data, const FGameplayEffectSpec&, Spec);

/**
 * Base Attribute Set. Probably only health really belongs where, so that things can receive damage
 */
UCLASS()
class MCGAMEPLAYABILITIES_API UMCAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:	
	UMCAttributeSet();

	UPROPERTY(Category = "Attribute | Health", EditAnywhere, ReplicatedUsing = OnRep_Health, BlueprintReadWrite) FGameplayAttributeData Health;
	UFUNCTION() void OnRep_Health() { GAMEPLAYATTRIBUTE_REPNOTIFY(UMCAttributeSet, Health); }
	static FGameplayAttribute AttributeHealth();

	UPROPERTY(Category = "Attribute | Max Health", EditAnywhere, ReplicatedUsing = OnRep_MaxHealth, BlueprintReadWrite) FGameplayAttributeData MaxHealth;
	UFUNCTION() void OnRep_MaxHealth() { GAMEPLAYATTRIBUTE_REPNOTIFY(UMCAttributeSet, MaxHealth); }
	static FGameplayAttribute AttributeMaxHealth();

	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData &Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MCAttributeSet")
	FDeathDelegate OnDeath;
};
