// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Cpp_SessionPlugin : ModuleRules
{
	public Cpp_SessionPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem", "OnlineSubsystemSteam" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", 
			"InputCore", "EnhancedInput" });
	}
}
