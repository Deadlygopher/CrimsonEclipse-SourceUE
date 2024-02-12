// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CrimsonEclipse : ModuleRules
{
	public CrimsonEclipse(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core",
															"CoreUObject",
															"Engine",
															"InputCore",
															"HeadMountedDisplay",
															"NavigationSystem",
															"AIModule",
                                                            "OnlineSubsystem",
                                                            "OnlineSubsystemSteam",
															"InventorySystem",
															"HTTP",
															"Json",
															"JsonUtilities",
															/*"UMG"*/});
    }
}
