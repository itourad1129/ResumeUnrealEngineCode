using UnrealBuildTool;
using System.Collections.Generic;

public class PJDRCServerTarget : TargetRules
{
	public PJDRCServerTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("PJDRC");
		BuildEnvironment = TargetBuildEnvironment.Unique;
		bUseLoggingInShipping = true;
	}
}