// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class BLACKBOX_war_projectTarget : TargetRules
{
	public BLACKBOX_war_projectTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "BLACKBOX_war_project" } );
	}
}
