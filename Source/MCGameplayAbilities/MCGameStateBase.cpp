// Fill out your copyright notice in the Description page of Project Settings.

#include "MCGameStateBase.h"
#include "Engine/StreamableManager.h"
#include "Engine/DataTable.h"
#include "GameplayAbility.h"

FString AMCGameStateBase::AbilityClassString = TEXT("BlueprintGeneratedClass'/Game/AbilitySystem/Blueprints/Abilities/GA_%s.GA_%s_C'"); //Name, Name 
FString AMCGameStateBase::AbilityIconString = TEXT("Texture2D'/Game/AbilitySystem/Textures/UI/T_Icon_%s.T_Icon_%s'");//Name, Name

void AMCGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	LoadAbilityInfo();
}

void AMCGameStateBase::LoadAbilityInfo()
{
	if (AbilityTable && !bAbilityDataReady)
	{
		FString ContextString;

		TArray<FAbilityTableRow*> TableRows;
		AbilityTable->GetAllRows(ContextString, TableRows);
		for (FAbilityTableRow* ObjectRow : TableRows)
		{
			if (ObjectRow)
			{
				FAbilityData AbilityData;

				FString NewAbilityClassString = FString::Printf(*AbilityClassString, *ObjectRow->BaseName, *ObjectRow->BaseName);
				FSoftObjectPath ClassRef = FSoftObjectPath(NewAbilityClassString);
				AbilityData.Class = TSoftClassPtr<UGameplayAbility>(ClassRef);

				FString NewAbilityIconString = FString::Printf(*AbilityIconString, *ObjectRow->BaseName, *ObjectRow->BaseName);
				FSoftObjectPath IconRef = FSoftObjectPath(NewAbilityIconString);
				AbilityData.Icon = TSoftObjectPtr<UTexture2D>(IconRef);

				GlobalAbilities.Add(AbilityData);
			}
		}

		// TODO: This should be done async on demand when a character is selected by a player
		TArray<FSoftObjectPath> ObjectsToLoad;
		for (auto &Ability : GlobalAbilities)
		{
			ObjectsToLoad.Add(Ability.Class.ToSoftObjectPath());
			ObjectsToLoad.Add(Ability.Icon.ToSoftObjectPath());
		}

		if (ObjectsToLoad.Num() > 0)
		{
			FStreamableManager Manager;
			Manager.RequestSyncLoad(ObjectsToLoad);
		}
		bAbilityDataReady = true;
	}
}
