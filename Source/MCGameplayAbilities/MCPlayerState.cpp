// Fill out your copyright notice in the Description page of Project Settings.

#include "MCPlayerState.h"
#include "UnrealNetwork.h"
#include "MCAbilitySystemComponent.h"
#include "MCCharacterAttributeSet.h"
#include "MCGameplayAbilitiesCharacter.h"



AMCPlayerState::AMCPlayerState()
	: Super()
{
	CurrentCharacter = -1;
	bInitialized;

	AbilitySystem = CreateDefaultSubobject<UMCAbilitySystemComponent>(TEXT("AbilitySystem"));
	AttributeSet = CreateDefaultSubobject<UMCCharacterAttributeSet>(TEXT("AttributeSet"));

}

void AMCPlayerState::PopulateCharacterInfo()
{
	FCharacterInfo BaseCharacterInfo;
	BaseCharacterInfo.Name = TEXT("Character");
	BaseCharacterInfo.KnownAbilities.Add(0);
	BaseCharacterInfo.KnownAbilities.Add(1);
	BaseCharacterInfo.EquippedAbilities.Add(0);
	BaseCharacterInfo.EquippedAbilities.Add(1);

	OwnedCharacters.Add(BaseCharacterInfo);

	FCharacterInfo AwesomeCharacterInfo;
	AwesomeCharacterInfo.Name = TEXT("Character");
	AwesomeCharacterInfo.KnownAbilities.Add(1);
	AwesomeCharacterInfo.KnownAbilities.Add(2);
	AwesomeCharacterInfo.KnownAbilities.Add(3);
	AwesomeCharacterInfo.EquippedAbilities.Add(1);
	AwesomeCharacterInfo.EquippedAbilities.Add(2);
	AwesomeCharacterInfo.EquippedAbilities.Add(3);

	OwnedCharacters.Add(AwesomeCharacterInfo);

	bInitialized = true;
}

FCharacterInfo* AMCPlayerState::GetCurrentCharacterInfo()
{
	if (OwnedCharacters.IsValidIndex(CurrentCharacter))
	{
		return &OwnedCharacters[CurrentCharacter];
	}
	else {
		return nullptr;
	}
}

void AMCPlayerState::SetCharacterInfo(int32 NewCharacterInfo)
{
	if (!bInitialized)
	{
		PopulateCharacterInfo();
	}

	if (HasAuthority())
	{
		CurrentCharacter = NewCharacterInfo;
		OnRep_CurrentCharacter();
	}
	else {
		Server_SetCharacterInfo(NewCharacterInfo);
	}
}

void AMCPlayerState::Server_SetCharacterInfo_Implementation(int32 NewCharacterInfo)
{
	SetCharacterInfo(NewCharacterInfo);
}

bool AMCPlayerState::Server_SetCharacterInfo_Validate(int32 NewCharacterInfo)
{
	return true;
}

bool AMCPlayerState::ApplyCurrentCharacterInfo(AMCGameplayAbilitiesCharacter* Character)
{
	FCharacterInfo* Current = GetCurrentCharacterInfo();
	if (!Current || !Character)
	{
		return false;
	}

	Character->SetAbilitySystem(AbilitySystem);
	AbilitySystem->InitAbilityActorInfo(this, Character);
	
	// Set things on the character's ASC
	if (HasAuthority())
	{
		AbilitySystem->ClearAllAbilities();
	}
	for (int32 i = 0; i < Current->EquippedAbilities.Num(); i++)
	{
		AbilitySystem->AddAbilityToSlot(i, Current->EquippedAbilities[i]);
	}

	return true;
}

void AMCPlayerState::OnRep_CurrentCharacter()
{
	OnCharacterInfoChange.Broadcast();
}

void AMCPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMCPlayerState, CurrentCharacter);
}