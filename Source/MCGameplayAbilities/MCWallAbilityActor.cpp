// Fill out your copyright notice in the Description page of Project Settings.

#include "MCWallAbilityActor.h"
#include "MCAttributeSet.h"
#include "MCAbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"


AMCWallAbilityActor::AMCWallAbilityActor(const FObjectInitializer& Initializer)
	: Super(Initializer)
{
	AbilitySystem = CreateDefaultSubobject<UMCAbilitySystemComponent>(TEXT("AbilitySystem"));
	AttributeSet = CreateDefaultSubobject<UMCAttributeSet>(TEXT("AttributeSet"));
}

void AMCWallAbilityActor::BeginPlay()
{
	Super::BeginPlay();

	AttributeSet->OnDeath.AddDynamic(this, &AMCWallAbilityActor::OnDeath);
}

UAbilitySystemComponent* AMCWallAbilityActor::GetAbilitySystemComponent() const
{
	return Cast<UAbilitySystemComponent>(AbilitySystem);
}

void AMCWallAbilityActor::OnDeath(const FGameplayModifierEvaluatedData& Data, const FGameplayEffectSpec& Spec)
{
	Destroy();
}
