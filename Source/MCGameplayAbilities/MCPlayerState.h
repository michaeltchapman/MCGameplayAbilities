// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MCPlayerState.generated.h"

// The controller can use this to know when the current character is changed,
// and recreate or modify its pawn as needed
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCharacterInfoChange);

// This should store the info for a character owned by a player. A player may own multiple characters,
// but should only have one active at any given time
USTRUCT(BlueprintType)
struct FCharacterInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterInfo)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterInfo)
	float Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterInfo)
	TArray<int32> KnownAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterInfo)
	TArray<int32> EquippedAbilities;
};

/**
 * Stores the available characters and the current character for the player
 */
UCLASS()
class MCGAMEPLAYABILITIES_API AMCPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AMCPlayerState();

	// When the player starts the game, this might contact an online service to get
	// the list of characters the current player owns, so that they can select one in the menu.
	// Currently, it just loads a static list of characters to choose from
	void PopulateCharacterInfo();

	// Will be null if no character has been selected yet
	FCharacterInfo* GetCurrentCharacterInfo();

	// Set a new character to be active.
	void SetCharacterInfo(int32 NewCharacterInfo);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetCharacterInfo(int32 NewCharacterInfo);

	// Apply the current character info to a character. Should be called on possession on both client
	// and server, and should be idempotent. Will also be called when the current character info is
	// changed
	bool ApplyCurrentCharacterInfo(class AMCGameplayAbilitiesCharacter* Character);

	UPROPERTY()
	FCharacterInfoChange OnCharacterInfoChange;

	bool bInitialized;
protected:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentCharacter)
	int32 CurrentCharacter;

	UFUNCTION()
	void OnRep_CurrentCharacter();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = CharacterInfo)
	TArray<FCharacterInfo> OwnedCharacters;

};
