// Copyright Epic Games, Inc. All Rights Reserved.

#include "PJDRC.h"
#include "Data/StaticDataManager.h"
#include "Modules/ModuleManager.h"

#ifdef LOCTEXT_NAMESPACE
#undef LOCTEXT_NAMESPACE
#endif

#define LOCTEXT_NAMESPACE "PJDRC"

#if WITH_EDITORONLY_DATA
#include "ISettingsModule.h"
#endif

void FOriginalGameModuleImplModule::StartupModule()
{
#if WITH_EDITORONLY_DATA
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (SettingsModule != nullptr)
	{
		SettingsModule->RegisterSettings(
			"Project", // ※２
			"Game",
			"StaticData",
			LOCTEXT("StaticDataName", "StaticData"),
			LOCTEXT("StaticDataDescription", "Set Static Datas"),
			GetMutableDefault<UStaticDataManager>()
		);
	}
#endif
}
void FOriginalGameModuleImplModule::ShutdownModule()
{
#if WITH_EDITORONLY_DATA
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (SettingsModule != nullptr)
	{
		SettingsModule->UnregisterSettings(
			"Project", // ※３
			"Game",
			"StaticData"
		);
	}
#endif
}
IMPLEMENT_PRIMARY_GAME_MODULE( FOriginalGameModuleImplModule, PJDRC, "PJDRC" );

#undef LOCTEXT_NAMESPACE