// Fill out your copyright notice in the Description page of Project Settings.

#include "MCPlayerController.h"
#include "MCGameplayAbilitiesCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "MCPlayerState.h"

void AMCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	AMCPlayerState* MCP = Cast<AMCPlayerState>(PlayerState);
	if (MCP)
	{
		MCP->PopulateCharacterInfo();
		MCP->SetCharacterInfo(0);
		MCP->OnCharacterInfoChange.AddDynamic(this, &AMCPlayerController::UpdateCharacter);

	}
}

void AMCPlayerController::Possess(APawn* P)
{
	Super::Possess(P);

	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(P);
	if (ASC)
	{
		ASC->InitAbilityActorInfo(P, P);
		UpdateCharacter();
	}
}

void AMCPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(P);
	if (ASC)
	{
		ASC->InitAbilityActorInfo(P, P);
		UpdateCharacter();
	}

}

void AMCPlayerController::UpdateCharacter()
{
	AMCGameplayAbilitiesCharacter* Char = Cast<AMCGameplayAbilitiesCharacter>(GetPawn());
	if (!Char)
	{
		if (HasAuthority())
		{
			// We're possessing something else for some reason
			if (GetPawn())
			{
				return;
			}

			FActorSpawnParameters ASP;
			ASP.Owner = this;
			ASP.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			Char = GetWorld()->SpawnActor<AMCGameplayAbilitiesCharacter>(AbilityCharacterClass, ASP);
			Possess(Char);
		}
	}

	AMCPlayerState* MCP = Cast<AMCPlayerState>(PlayerState);
	if (MCP && Char)
	{
		if (!MCP->GetCurrentCharacterInfo())
		{
			MCP->SetCharacterInfo(0);
		}

		if (MCP->ApplyCurrentCharacterInfo(Char))
		{
			GLog->Log(ELogVerbosity::Warning, FString::Printf(TEXT("%s: Applied character info"), *GetNameSafe(this)));
		}
	}
}
