using UnrealBuildTool;
using System.Collections.Generic;

public class PJDRCClientTarget : TargetRules
{
	public PJDRCClientTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Client;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("PJDRC");
		BuildEnvironment = TargetBuildEnvironment.Unique;
		bUseLoggingInShipping = true;
	}
}