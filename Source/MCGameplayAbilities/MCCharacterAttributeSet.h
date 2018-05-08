// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MCAttributeSet.h"
#include "MCCharacterAttributeSet.generated.h"

/**
 * Characters have more than just health, so they get their own attribute set
 */
UCLASS()
class MCGAMEPLAYABILITIES_API UMCCharacterAttributeSet : public UMCAttributeSet
{
	GENERATED_BODY()

public:
	UMCCharacterAttributeSet();
	
	UPROPERTY(Category = "Attribute | Damage", EditAnywhere, ReplicatedUsing = OnRep_Damage, BlueprintReadWrite) FGameplayAttributeData Damage;
	UFUNCTION() void OnRep_Damage() { GAMEPLAYATTRIBUTE_REPNOTIFY(UMCCharacterAttributeSet, Damage); }
	static FGameplayAttribute AttributeDamage();
	
};
