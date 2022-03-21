#include "PluginAPI.h"
#include "ParamInfos.h"
#include "GameAPI.h"

#include "Logging.hpp"
#include "Interfaces.hpp"
#include "ScriptCommands.hpp"

extern "C" {

	bool OBSEPlugin_Query(const OBSEInterface *obse, PluginInfo *info) {

		Log_Print("OBSEPlugin::Query, Start");

		info->infoVersion = PluginInfo::kInfoVersion;
		info->name = "objson";
		info->version = 2;

		if (obse->isEditor) {
			Log_Print("OBSEPlugin::Query, Editor >> skip other checks)");
			return true;
		}

		if (obse->obseVersion < OBSE_VERSION_INTEGER) {
			Log_Print("OBSEPlugin::Query, OBSE version too old, got %08X, expected >= %08X", obse->obseVersion, OBSE_VERSION_INTEGER);
			return false;
		}

		if (obse->oblivionVersion != OBLIVION_VERSION) {
			Log_Print("OBSEPlugin::Query, Incorrect Oblivion version, got %08X, need %08X", obse->oblivionVersion, OBLIVION_VERSION);
			return false;
		}

		if ((objson::Interfaces::kOBSEScript = (OBSEScriptInterface*)obse->QueryInterface(kInterface_Script)) == NULL) {
			Log_Print("OBSEPlugin::Query, Failed to acquire ScriptInterface");
			return false;
		}

		if ((objson::Interfaces::kOBSEStringVar = (OBSEStringVarInterface *)obse->QueryInterface(kInterface_StringVar)) == NULL) {
			Log_Print("OBSEPlugin::Query, Failed to acquire StringVarInterface");
			return false;
		}

		if ((objson::Interfaces::kOBSEArrayVar = (OBSEArrayVarInterface *)obse->QueryInterface(kInterface_ArrayVar)) == NULL) {
			Log_Print("OBSEPlugin::Query, Failed to acquire ArrayVarInterface");
			return false;
		}

		Log_Print("OBSEPlugin::Query, Passed");

		return true; // version checks passed

	}

	bool OBSEPlugin_Load(const OBSEInterface *obse) {

		Log_Print("OBSEPlugin::Load, Start");

		objson::Interfaces::kPluginHandle = obse->GetPluginHandle();

		if (!obse->isEditor) {
			objson::Interfaces::kOblivionDirectory = std::string(obse->GetOblivionDirectory());
			Log_Print("OBSEPlugin::Load, OblivionDir (%s)", objson::Interfaces::kOblivionDirectory.c_str());
		}

		obse->SetOpcodeBase(0x2890); // the plugin was assigned 0x2890 - 289F

		obse->RegisterTypedCommand(&objson::ScriptCommands::kCommandInfo_JsonGetString, kRetnType_String);
		obse->RegisterCommand(&objson::ScriptCommands::kCommandInfo_JsonSetString);

		obse->RegisterCommand(&objson::ScriptCommands::kCommandInfo_JsonGetFloat);
		obse->RegisterCommand(&objson::ScriptCommands::kCommandInfo_JsonSetFloat);

		obse->RegisterCommand(&objson::ScriptCommands::kCommandInfo_JsonGetInt);
		obse->RegisterCommand(&objson::ScriptCommands::kCommandInfo_JsonSetInt);

		obse->RegisterTypedCommand(&objson::ScriptCommands::kCommandInfo_JsonGetForm, kRetnType_Form);
		obse->RegisterCommand(&objson::ScriptCommands::kCommandInfo_JsonSetForm);

		obse->RegisterCommand(&objson::ScriptCommands::kCommandInfo_JsonEraseKey);
		obse->RegisterTypedCommand(&objson::ScriptCommands::kCommandInfo_JsonListKeys, kRetnType_Array);

		Log_Print("OBSEPlugin::Load, Finished");

		return true;
	}

};
