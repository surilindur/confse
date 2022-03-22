#include "common/IDebugLog.h"

#include "obse/PluginAPI.h"
#include "obse/ParamInfos.h"
#include "obse/GameAPI.h"

#include "Interfaces.hpp"
#include "ScriptCommands.hpp"

#define Log_Print(...) _MESSAGE(__VA_ARGS__)

#define OBJSON_LOGFILE "ObJson.log" // ends up in the game directory
#define OBJSON_CACHE_MAX_FILES 100  // when this many files in cache, drops all of them
#define OBJSON_OPCODE_BASE 0x2890   // the plugin was assigned 0x2890 - 289F
#define OBJSON_MOD_NAME_LENGTH 0x100
#define OBJSON_REF_ID_LENGTH 0x20

IDebugLog gLog(OBJSON_LOGFILE);

extern "C"
{
	bool OBSEPlugin_Query(const OBSEInterface *obse, PluginInfo *info)
	{
		Log_Print("OBSEPlugin_Query running");

		info->infoVersion = PluginInfo::kInfoVersion;
		info->name = "objson";
		info->version = 2;

		if (obse->isEditor)
		{
			Log_Print("Editor mode, skip other checks)");
		}
		else
		{
			if (obse->obseVersion < OBSE_VERSION_INTEGER)
			{
				Log_Print("OBSE version too old, got %08X, expected >= %08X", obse->obseVersion, OBSE_VERSION_INTEGER);
				return false;
			}
			else if (obse->oblivionVersion != OBLIVION_VERSION)
			{
				Log_Print("Incorrect Oblivion version, got %08X, need %08X", obse->oblivionVersion, OBLIVION_VERSION);
				return false;
			}
			else if ((objson::Interfaces::kOBSEScript = (OBSEScriptInterface*)obse->QueryInterface(kInterface_Script)) == NULL)
			{
				Log_Print("Failed to acquire ScriptInterface");
				return false;
			}
			else if ((objson::Interfaces::kOBSEStringVar = (OBSEStringVarInterface *)obse->QueryInterface(kInterface_StringVar)) == NULL)
			{
				Log_Print("Failed to acquire StringVarInterface");
				return false;
			}
			else if ((objson::Interfaces::kOBSEArrayVar = (OBSEArrayVarInterface *)obse->QueryInterface(kInterface_ArrayVar)) == NULL)
			{
				Log_Print("Failed to acquire ArrayVarInterface");
				return false;
			}
		}

		Log_Print("OBSEPlugin_Query passed");
		return true;
	}

	bool OBSEPlugin_Load(const OBSEInterface *obse)
	{
		Log_Print("OBSEPlugin_Load running");

		objson::Interfaces::kPluginHandle = obse->GetPluginHandle();

		if (!obse->isEditor)
		{
			objson::Interfaces::kOblivionDirectory = std::string(obse->GetOblivionDirectory());
			Log_Print("OblivionDirectory: %s", objson::Interfaces::kOblivionDirectory.c_str());
		}

		obse->SetOpcodeBase(OBJSON_OPCODE_BASE);

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

		Log_Print("OBSEPlugin_Load finished");
		return true;
	}
};
