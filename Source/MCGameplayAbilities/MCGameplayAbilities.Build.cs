// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MCGameplayAbilities : ModuleRules
{
	public MCGameplayAbilities(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "GameplayTasks", "GameplayTags", "GameplayAbilities" });
		PrivateDependencyModuleNames.AddRange(new string[] { "GameplayAbilities" });
	}
}
