// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PROJECT_A : ModuleRules
{
	public PROJECT_A(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore","MotionWarping","EnhancedInput","PhysicsCore" ,"UMG","Niagara","NavigationSystem","AIModule","GameplayTasks"});

		PrivateDependencyModuleNames.AddRange(new string[] {"AnimGraphRuntime"});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
