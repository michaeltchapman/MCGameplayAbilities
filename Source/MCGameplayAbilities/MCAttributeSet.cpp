// Fill out your copyright notice in the Description page of Project Settings.

#include "MCAttributeSet.h"
#include "UnrealNetwork.h"


UMCAttributeSet::UMCAttributeSet() : Super()
{
	Health = FGameplayAttributeData(100.f);
	MaxHealth = FGameplayAttributeData(100.f);
}

bool UMCAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData &Data)
{
	return true;
}

void UMCAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData &Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (AttributeHealth() == Data.EvaluatedData.Attribute)
	{
		if (Health.GetCurrentValue() < 0.f)
		{
			OnDeath.Broadcast(Data.EvaluatedData, Data.EffectSpec);
		}
	}
}

void UMCAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{

}

FGameplayAttribute UMCAttributeSet::AttributeHealth()
{
	static UProperty* Property = FindFieldChecked<UProperty>(UMCAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UMCAttributeSet, Health));
	return FGameplayAttribute(Property);
}

FGameplayAttribute UMCAttributeSet::AttributeMaxHealth()
{
	static UProperty* Property = FindFieldChecked<UProperty>(UMCAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UMCAttributeSet, MaxHealth));
	return FGameplayAttribute(Property);
}

void UMCAttributeSet::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UMCAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMCAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}