// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class CUBICAREditorTarget : TargetRules
{
	public CUBICAREditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.Add("CUBICAR");
        //ExtraModuleNames.Add("Race");
    }
}
