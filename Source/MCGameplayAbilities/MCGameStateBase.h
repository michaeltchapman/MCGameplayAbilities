// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/GameStateBase.h"
#include "MCGameStateBase.generated.h"

// DataTable row definition for abilities
USTRUCT(BlueprintType)
struct FAbilityTableRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	// Used to load the ability class and the icon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability)
	FString BaseName;

	// TODO probably add description and tooltip and stuff
};

// Use this to store the class and things needed by the UI. Another option
// is to put all of this in the CDO and only store the class
USTRUCT(BlueprintType)
struct FAbilityData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability)
	TSoftClassPtr<class UGameplayAbility> Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability)
	TSoftObjectPtr<class UTexture2D> Icon;
};


/**
 * 
 */
UCLASS()
class MCGAMEPLAYABILITIES_API AMCGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	// This is a list of every ability in the game, will be populated by the datatable
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities")
	TArray<FAbilityData> GlobalAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities")
	UDataTable* AbilityTable;

	virtual void BeginPlay() override;

	// Since things are loaded asynchronously, things might not be ready immediately
	UFUNCTION(BlueprintPure, Category = "MCGameState")
	bool IsAbilityDataReady() { return bAbilityDataReady; };

	void LoadAbilityInfo();
protected:
	bool bAbilityDataReady;
private:
	static FString AbilityClassString;
	static FString AbilityIconString;
	
	
	
};
