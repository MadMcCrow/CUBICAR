// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UserInterface : ModuleRules
{
    public UserInterface(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        //Public module names that this module uses.
        //In case you would like to add various classes that you're going to use in your game
        //you should add the core,coreuobject and engine dependencies.
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "CUBICAR"  });

        // User interface modules
        PublicDependencyModuleNames.AddRange(new string[] { "UMG", "StreamingPauseRendering" });
        PublicDependencyModuleNames.AddRange(new string[] { "MediaAssets" }); // video support
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
        

        //The path for the header files
        PublicIncludePaths.AddRange(new string[] { "UserInterface/Public" });

        //The path for the source files
        PrivateIncludePaths.AddRange(new string[] { "UserInterface/Private" });
    }
}
