// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SpawnFromUI : ModuleRules
{
	public SpawnFromUI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "InputCore" ,
                "HeadMountedDisplay",
                "NavigationSystem",
                "AIModule",
                "Niagara",
                "EnhancedInput",
                "Assignments_1_2"});

		PrivateDependencyModuleNames.AddRange(new string[] {  });



        PublicIncludePaths.AddRange(new string[] {
             "D:/Unreal Projects/Assignments/Assignments_4/SpawnFromUI/Plugins/Assignments_1_2/Source/Assignments_1_2/Public" });

  

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
