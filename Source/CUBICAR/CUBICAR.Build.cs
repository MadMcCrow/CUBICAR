// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CUBICAR : ModuleRules
{
	public CUBICAR(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "EngineSettings", "InputCore", });

        PublicDependencyModuleNames.Add("UserInterface");
        PrivateDependencyModuleNames.Add("UserInterface");

        PublicDependencyModuleNames.Add("Race");

        PublicDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem", "OnlineSubsystemUtils" }); // multiplayer


        // VR Support
        PublicDependencyModuleNames.AddRange(new string[] { "HeadMountedDisplay" });
        PublicDefinitions.Add("HMD_MODULE_INCLUDED=1");




        // uncomment for private race access
        //PrivateDependencyModuleNames.AddRange(new string[] { "Race" });

        // Uncomment for AI
        //PublicDependencyModuleNames.AddRange(new string[] { "AIModule" });


        // Uncomment if you are using online features
        PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
        DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");
    }
}
