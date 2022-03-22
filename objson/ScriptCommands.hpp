#include "obse/GameForms.h"
#include "obse/PluginAPI.h"
#include "obse/GameAPI.h"

#include "Interfaces.hpp"
#include "JsonManager.hpp"
#include "TESForms.hpp"

#define ARG_MAX_CHARS 512
#define ExtractArgsEx(...) ObJson::Interfaces::kOBSEScript->ExtractArgsEx(__VA_ARGS__)
#define PASS_EXTRACT_ARGS_EX paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList

namespace ObJson::ScriptCommands
{
	static bool Cmd_ObJson_JsonGetString_Execute(COMMAND_ARGS)
    {
		char filename[ARG_MAX_CHARS], key[ARG_MAX_CHARS], default_return[ARG_MAX_CHARS];
		*result = 0;

		if (ExtractArgs(PASS_EXTRACT_ARGS, &filename, &key, &default_return)) {
			std::string temp_str(default_return);
			ObJson::JsonManager::JsonObject JObject(filename);
			JObject.Get(key, temp_str);
			ObJson::Interfaces::kOBSEStringVar->Assign(PASS_COMMAND_ARGS, temp_str.c_str());
		}

		return true;
	}

	static bool Cmd_ObJson_JsonSetString_Execute(COMMAND_ARGS)
    {
		char filename[ARG_MAX_CHARS], key[ARG_MAX_CHARS], value[ARG_MAX_CHARS];
		*result = 0;

		if (ExtractArgs(PASS_EXTRACT_ARGS, &filename, &key, &value)) {
			ObJson::JsonManager::JsonObject JObject(filename);
			JObject.Set(key, std::string(value));
			JObject.WriteToDisk();
		}

		return true;
	}

	static bool Cmd_ObJson_JsonGetInt_Execute(COMMAND_ARGS)
    {
		char filename[ARG_MAX_CHARS], key[ARG_MAX_CHARS];
		UInt32 DefaultReturn;

		if (ExtractArgs(PASS_EXTRACT_ARGS, &filename, &key, &DefaultReturn)) {
			ObJson::JsonManager::JsonObject JObject(filename);
			JObject.Get(key, DefaultReturn);
			*result = DefaultReturn;
		} else {
			*result = 0;
		}

		return true;
	}

	static bool Cmd_ObJson_JsonSetInt_Execute(COMMAND_ARGS)
    {
		char filename[ARG_MAX_CHARS], key[ARG_MAX_CHARS];
		UInt32 value;
		*result = 0;

		if (ExtractArgs(PASS_EXTRACT_ARGS, &filename, &key, &value)) {
			ObJson::JsonManager::JsonObject JObject(filename);
			JObject.Set(key, value);
			JObject.WriteToDisk();
		}

		return true;
	}

	static bool Cmd_ObJson_JsonGetFloat_Execute(COMMAND_ARGS)
    {
		char filename[ARG_MAX_CHARS], key[ARG_MAX_CHARS];
		float default_return;

		if (ExtractArgs(PASS_EXTRACT_ARGS, &filename, &key, &default_return))
        {
			ObJson::JsonManager::JsonObject JObject(filename);
			JObject.Get(key, default_return);
        }

        *result = default_return;

		return true;
	}

	static bool Cmd_ObJson_JsonSetFloat_Execute(COMMAND_ARGS)
    {
		char filename[ARG_MAX_CHARS], key[ARG_MAX_CHARS];
		float value;
		*result = 0;

		if (ExtractArgs(PASS_EXTRACT_ARGS, &filename, &key, &value))
        {
			ObJson::JsonManager::JsonObject JObject(filename);
			JObject.Set(key, value);
			JObject.WriteToDisk();
		}

		return true;
	}

	static bool Cmd_ObJson_JsonGetForm_Execute(COMMAND_ARGS)
    {
		char filename[ARG_MAX_CHARS], key[ARG_MAX_CHARS];
		TESForm *default_return;
		UInt32 *ref_result = (UInt32 *)result;
		std::string temp_str("");

		if (ExtractArgsEx(PASS_EXTRACT_ARGS_EX, &filename, &key, &default_return))
        {
			*ref_result = default_return->refID;
			ObJson::JsonManager::JsonObject JObject(filename);
			JObject.Get(key, temp_str);
			ObJson::TESForms::StringToRefID(temp_str, ref_result);
		}
        else
        {
			*ref_result = 0;
		}

		return true;
	}

	static bool Cmd_ObJson_JsonSetForm_Execute(COMMAND_ARGS)
    {
		char filename[ARG_MAX_CHARS], key[ARG_MAX_CHARS];
		TESForm *value;
		*result = 0;

		if (ExtractArgsEx(PASS_EXTRACT_ARGS_EX, &filename, &key, &value))
        {
			ObJson::JsonManager::JsonObject JObject(filename);
			JObject.Set(key, ObJson::TESForms::FormToString(value));
			JObject.WriteToDisk();
		}

		return true;
	}

	static bool Cmd_ObJson_JsonEraseKey_Execute(COMMAND_ARGS)
    {
		char filename[ARG_MAX_CHARS], key[ARG_MAX_CHARS];
		*result = 0;

		if (ExtractArgs(PASS_EXTRACT_ARGS, &filename, &key))
        {
			ObJson::JsonManager::JsonObject JObject(filename);
			JObject.Erase(key);
			JObject.WriteToDisk();
		}

		return true;
	}

	static bool Cmd_ObJson_JsonListKeys_Execute(COMMAND_ARGS) {

		char filename[ARG_MAX_CHARS], key[ARG_MAX_CHARS];
		*result = 0;

		if (ExtractArgs(PASS_EXTRACT_ARGS, &filename, &key))
        {
			ObJson::JsonManager::JsonObject JObject(filename);
			std::vector<std::string> key_list;
			JObject.ListKeys(key, key_list);
			OBSEArray* obse_array = ObJson::Interfaces::ArrayFromStdVector(key_list, scriptObj);
			ObJson::Interfaces::kOBSEArrayVar->AssignCommandResult(obse_array, result);
		}

		return true;
	}

	ParamInfo kParams_JsonGetString[3] = {
		{ "Filename", kParamType_String, 0 },
		{ "Key", kParamType_String, 0 },
		{ "Default", kParamType_String, 0 }
	};

	ParamInfo kParams_JsonSetString[3] = {
		{ "Filename", kParamType_String, 0 },
		{ "Key", kParamType_String, 0 },
		{ "Value", kParamType_String, 0 }
	};

	ParamInfo kParams_JsonGetInt[3] = {
		{ "Filename", kParamType_String, 0 },
		{ "Key", kParamType_String, 0 },
		{ "Default", kParamType_Integer, 0 }
	};

	ParamInfo kParams_JsonSetInt[3] = {
		{ "Filename", kParamType_String, 0 },
		{ "Key", kParamType_String, 0 },
		{ "Value", kParamType_Integer, 0 }
	};

	ParamInfo kParams_JsonGetFloat[3] = {
		{ "Filename", kParamType_String, 0 },
		{ "Key", kParamType_String, 0 },
		{ "Default", kParamType_Float, 0 }
	};

	ParamInfo kParams_JsonSetFloat[3] = {
		{ "Filename", kParamType_String, 0 },
		{ "Key", kParamType_String, 0 },
		{ "Value", kParamType_Float, 0 }
	};

	ParamInfo kParams_JsonGetForm[3] = {
		{ "Filename", kParamType_String, 0 },
		{ "Key", kParamType_String, 0 },
		{ "Default", kOBSEParamType_Form, 0 }
	};

	ParamInfo kParams_JsonSetForm[3] = {
		{ "Filename", kParamType_String, 0 },
		{ "Key", kParamType_String, 0 },
		{ "Value", kOBSEParamType_Form, 0 }
	};

	ParamInfo kParams_JsonEraseKey[2] = {
		{ "Filename", kParamType_String, 0 },
		{ "Key", kParamType_String, 0 }
	};

	ParamInfo kParams_JsonListKeys[2] = {
		{ "Filename", kParamType_String, 0 },
		{ "KeyPath", kParamType_String, 0 }
	};

	CommandInfo kCommandInfo_JsonGetString = {
		"JsonGetString",
		"",
		0,
		"Read string value from JSON",
		0,
		3,
		kParams_JsonGetString,
		Cmd_objson_JsonGetString_Execute
	};

	CommandInfo kCommandInfo_JsonSetString = {
		"JsonSetString",
		"",
		0,
		"Write string value to JSON",
		0,
		3,
		kParams_JsonSetString,
		Cmd_objson_JsonSetString_Execute
	};

	CommandInfo kCommandInfo_JsonGetInt = {
		"JsonGetInt",
		"",
		0,
		"Read integer value from JSON",
		0,
		3,
		kParams_JsonGetInt,
		Cmd_objson_JsonGetInt_Execute
	};

	CommandInfo kCommandInfo_JsonSetInt = {
		"JsonSetInt",
		"",
		0,
		"Write integer value to JSON",
		0,
		3,
		kParams_JsonSetInt,
		Cmd_objson_JsonSetInt_Execute
	};

	CommandInfo kCommandInfo_JsonGetFloat = {
		"JsonGetFloat",
		"",
		0,
		"Read floating point value from JSON",
		0,
		3,
		kParams_JsonGetFloat,
		Cmd_objson_JsonGetFloat_Execute
	};

	CommandInfo kCommandInfo_JsonSetFloat = {
		"JsonSetFloat",
		"",
		0,
		"Write floating point value to JSON",
		0,
		3,
		kParams_JsonSetFloat,
		Cmd_objson_JsonSetFloat_Execute
	};

	CommandInfo kCommandInfo_JsonGetForm = {
		"JsonGetForm",
		"",
		0,
		"Read form value from JSON",
		0,
		3,
		kParams_JsonGetForm,
		Cmd_objson_JsonGetForm_Execute
	};

	CommandInfo kCommandInfo_JsonSetForm = {
		"JsonSetForm",
		"",
		0,
		"Write form value to JSON",
		0,
		3,
		kParams_JsonSetForm,
		Cmd_objson_JsonSetForm_Execute
	};

	CommandInfo kCommandInfo_JsonEraseKey = {
		"JsonEraseKey",
		"",
		0,
		"Erase key and value from JSON",
		0,
		2,
		kParams_JsonEraseKey,
		Cmd_objson_JsonEraseKey_Execute
	};

	CommandInfo kCommandInfo_JsonListKeys = {
		"JsonListKeys",
		"",
		0,
		"List all keys in a key path in JSON",
		0,
		2,
		kParams_JsonListKeys,
		Cmd_objson_JsonListKeys_Execute
	};

}
