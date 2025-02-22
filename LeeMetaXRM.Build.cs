// Copyright Epic Games, Inc. All Rights Reserved.

using EpicGames.Core;
using System;
using UnrealBuildTool;

public class LeeMetaXRM : ModuleRules
{
	public LeeMetaXRM(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] 
		{
			"Core", 
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"Json",
            "JsonUtilities",
			"HTTP",
			"AnimGraphRuntime",
			"UMG",
			"Slate",
			"SlateCore",
            "AnimGraphRuntime",
            "RenderCore",
            "IKRig",
            "GameplayAbilities",
            "GameplayTags",
            "GameplayTasks",
            "AIModule",
            "NavigationSystem",

        });

        //PrivateIncludePaths.AddRange(new string[] {
        //    "Slate",
        //    "SlateCore",
        //});


        PublicIncludePaths.AddRange(new string[] {
            "LeeMetaXRM/Common",
            "LeeMetaXRM/Ultils",
            "LeeMetaXRM/",
            "LeeMetaXRM/Shaders",
        });

        if (Target.bBuildEditor==true)
        {
            PublicDependencyModuleNames.AddRange(new string[]
            {
                "AnimGraph",
                "AnimationModifiers",
                "AnimationBlueprintLibrary",
                "UnrealEd",
                "UMGEditor",
                "Localization",
				"AssetTools",
				"MaterialEditor",
                "Kismet",
                "AnimationBlueprintEditor",
                "IKRigEditor",
                "AIGraph",
                "AIModule"
            });


        }


        if (Target.Platform == UnrealTargetPlatform.Android) 
		{

		}
	

		if(Target.Platform == UnrealTargetPlatform.Linux)
		{

		}

        if (Target.Platform == UnrealTargetPlatform.IOS || Target.Platform == UnrealTargetPlatform.Mac)
        {

        }


    }
}
