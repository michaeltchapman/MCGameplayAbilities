// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MCGameplayAbilitiesTarget : TargetRules
{
	public MCGameplayAbilitiesTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.Add("MCGameplayAbilities");
	}
}
