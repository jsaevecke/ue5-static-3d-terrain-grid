// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class BLACKBOX_war_projectEditorTarget : TargetRules
{
	public BLACKBOX_war_projectEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "BLACKBOX_war_project" } );
	}
}
