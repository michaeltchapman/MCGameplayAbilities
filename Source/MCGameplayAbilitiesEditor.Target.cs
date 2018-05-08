// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MCGameplayAbilitiesEditorTarget : TargetRules
{
	public MCGameplayAbilitiesEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.Add("MCGameplayAbilities");
	}
}
