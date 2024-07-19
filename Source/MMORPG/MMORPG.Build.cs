// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MMORPG : ModuleRules
{
	public MMORPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		//系统模块
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput",
			"Slate",
			"SlateCore",
		 });

        //自定义插件
        PublicDependencyModuleNames.AddRange(new string[] {
            "SimpleNetChannel",
			"SimpleThread",
			"MMORPGCommon",
            "SimpleActorBrowsing",
         });
    }
}
