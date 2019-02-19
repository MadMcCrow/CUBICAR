// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CUBICAR : ModuleRules
{
	public CUBICAR(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "EngineSettings", "InputCore", "Race","UMG" });
        PublicDependencyModuleNames.AddRange(new string[] { "PhysXVehicles" });
        PublicDependencyModuleNames.AddRange(new string[] { "StreamingPauseRendering" });
        //PublicDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem", "OnlineSubsystemUtils" }); // multiplayer
        PublicDependencyModuleNames.AddRange(new string[] { "MediaAssets" }); // video support

        PublicDependencyModuleNames.AddRange(new string[] { "HeadMountedDisplay" });


        PublicDefinitions.Add("HMD_MODULE_INCLUDED=1");


        //PrivateDependencyModuleNames.AddRange(new string[] { "Race" });
        // Uncomment for AI
        //PublicDependencyModuleNames.AddRange(new string[] { "AIModule" });

        // Uncomment if you are using Slate UI
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
        


        PublicDependencyModuleNames.AddRange(new string[] { "Race" });
        // Uncomment if you are using online features
        //PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
        //DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");
    }
}
