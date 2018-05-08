// Fill out your copyright notice in the Description page of Project Settings.

#include "MCAbilitySystemComponent.h"
#include "GameplayAbility.h"
#include "MCGameStateBase.h"




UMCAbilitySystemComponent::UMCAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	for (int32 i = 0; i < 6; i++)
	{
		ActiveSlots.Add(nullptr);
		ActiveHandles.Add(FGameplayAbilitySpecHandle());
	}
}
bool UMCAbilitySystemComponent::AddAbilityToSlot(int32 Slot, int32 AbilityIndex)
{
	AMCGameStateBase* MCGSB = Cast<AMCGameStateBase>(GetWorld()->GetGameState());
	if (MCGSB)
	{
		if (!MCGSB->IsAbilityDataReady())
		{
			MCGSB->LoadAbilityInfo();
		}
		return AddAbilityToSlot(Slot, MCGSB->GlobalAbilities[AbilityIndex].Class.Get());
	}
	return false;
}

bool UMCAbilitySystemComponent::AddAbilityToSlot(int32 Slot, TSubclassOf<UGameplayAbility> Ability)
{
	if (Ability && ActiveSlots.IsValidIndex(Slot))
	{
		UGameplayAbility* CDO = Cast<UGameplayAbility>(Ability->GetDefaultObject());
		if (CDO)
		{
			if (GetOwner()->HasAuthority())
			{
				ActiveHandles[Slot] = GiveAbility(FGameplayAbilitySpec(CDO, 1.f));
			}
			ActiveSlots[Slot] = Ability;

			return true;
		}
	}
	return false;
}

bool UMCAbilitySystemComponent::RemoveAbilityFromSlot(int32 Slot)
{
	if (ActiveHandles.IsValidIndex(Slot) && ActiveHandles[Slot].IsValid())
	{
		if (GetOwner()->HasAuthority())
		{
			ClearAbility(ActiveHandles[Slot]);
		}
		ActiveHandles[Slot] = FGameplayAbilitySpecHandle();
		return true;
	}
	return false;
}
