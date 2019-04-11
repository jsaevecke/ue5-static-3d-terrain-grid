// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class BLACKBOX_war_project : ModuleRules
{
	public BLACKBOX_war_project(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Landscape", "OnlineSubsystem", "OnlineSubsystemUtils" , "Slate", "SlateCore", "UMG" });
        DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");
    }
}
