// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class CUBICARTarget : TargetRules
{
	public CUBICARTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.Add("CUBICAR");
		//ExtraModuleNames.Add("Race");
	}
}
