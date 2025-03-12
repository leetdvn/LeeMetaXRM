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
            "GameplayTags",
            "GameplayTasks",
            "AIModule",
            "NavigationSystem",
            "HeadMountedDisplay",
            "XRBase",
            "OculusXRInput",
            "Niagara",
            "OculusHandPoseRecognition",
            "GameplayAbilities",
            "GameplayTasks",
            "GameplayTags",
            "OculusUtils",
        });

        //PrivateIncludePaths.AddRange(new string[] {
        //    "Slate",
        //    "SlateCore",

        //});


        PublicIncludePaths.AddRange(new string[] {
            "LeeMetaXRM/Common",
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
                "UMGEditor",
                "Localization",
				"AssetTools",
				"MaterialEditor",
                "Kismet",
                "AnimationBlueprintEditor",
                "IKRigEditor",
                "AIGraph",
                "AIModule",
                "Niagara",

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

        // Fix for CS0103: The name 'bCanContainContent' does not exist in the current context
    }
}
