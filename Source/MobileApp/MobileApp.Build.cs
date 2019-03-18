// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MobileApp : ModuleRules
{
    public MobileApp(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        //Public module names that this module uses.
        //In case you would like to add various classes that you're going to use in your game
        //you should add the core,coreuobject and engine dependencies.
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "CUBICAR"  });

        // User interface modules
        //PublicDependencyModuleNames.AddRange(new string[] { "UMG", "StreamingPauseRendering" });
        //PublicDependencyModuleNames.AddRange(new string[] { "MediaAssets" }); // video support
        //PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        //PublicDependencyModuleNames.Add("UserInterface");

        //The path for the header files
        PublicIncludePaths.AddRange(new string[] { "MobileApp/Public" });

        //The path for the source files
        PrivateIncludePaths.AddRange(new string[] { "MobileApp/Private" });

        // Uncomment if you are using online features
        PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
        DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");
    }
}
