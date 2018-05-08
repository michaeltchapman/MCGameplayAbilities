// Fill out your copyright notice in the Description page of Project Settings.

#include "MCCharacterAttributeSet.h"
#include "Net/UnrealNetwork.h"

#define GPATTRIBUTE(AttributeName) \
FGameplayAttribute UMCCharacterAttributeSet::Attribute##AttributeName() \
{ \
	static UProperty* Property = FindFieldChecked<UProperty>(UMCCharacterAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UMCCharacterAttributeSet, AttributeName)); \
	return FGameplayAttribute(Property); \
}

UMCCharacterAttributeSet::UMCCharacterAttributeSet() : Super()
{
	Damage = FGameplayAttributeData(1.f);
}

GPATTRIBUTE(Damage)

void UMCCharacterAttributeSet::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UMCCharacterAttributeSet, Damage, COND_None, REPNOTIFY_Always);
}