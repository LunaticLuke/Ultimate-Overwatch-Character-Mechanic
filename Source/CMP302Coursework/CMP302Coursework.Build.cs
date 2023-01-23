// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CMP302Coursework : ModuleRules
{
	public CMP302Coursework(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay","CableComponent" });
	}
}
