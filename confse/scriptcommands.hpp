#include "obse/GameForms.h"
#include "obse/PluginAPI.h"
#include "obse/GameAPI.h"

#include "obseplugin.hpp"
#include "jsonfile.hpp"
#include "tesforms.hpp"

#define ARG_MAX_CHARS 512
#define ExtractArgsEx(...) objson::obseplugin::kOBSEScript->ExtractArgsEx(__VA_ARGS__)
#define PASS_EXTRACT_ARGS_EX paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList

namespace objson::scriptcommands
{
    static bool get_string(COMMAND_ARGS)
    {
        char filename[ARG_MAX_CHARS], key[ARG_MAX_CHARS], default_return[ARG_MAX_CHARS];

        if (ExtractArgs(PASS_EXTRACT_ARGS, &filename, &key, &default_return))
        {
            std::string temp_str(default_return);
            objson::jsonfile json_file(filename);
            json_file.get_entry(key, temp_str);
            objson::obseplugin::kOBSEStringVar->Assign(PASS_COMMAND_ARGS, temp_str.c_str());
            *result = 0;
            return true;
        }

        return false;
    }

    static bool set_string(COMMAND_ARGS)
    {
        char filename[ARG_MAX_CHARS], key[ARG_MAX_CHARS], value[ARG_MAX_CHARS];

        if (ExtractArgs(PASS_EXTRACT_ARGS, &filename, &key, &value))
        {
            objson::jsonfile json_file(filename);
            json_file.set_entry(key, value);
            json_file.flush_to_disk();
            *result = 0;
            return true;
        }

        return false;
    }

    static bool get_int(COMMAND_ARGS)
    {
        char filename[ARG_MAX_CHARS], key[ARG_MAX_CHARS];
        UInt32 default_return = 0;

        if (ExtractArgs(PASS_EXTRACT_ARGS, &filename, &key, &default_return))
        {
            objson::jsonfile json_file(filename);
            json_file.get_entry(key, default_return);
            *result = default_return;
            return true;
        }

        return false;
    }

    static bool set_int(COMMAND_ARGS)
    {
        char filename[ARG_MAX_CHARS], key[ARG_MAX_CHARS];
        UInt32 value;

        if (ExtractArgs(PASS_EXTRACT_ARGS, &filename, &key, &value))
        {
            objson::jsonfile json_file(filename);
            json_file.set_entry(key, value);
            json_file.flush_to_disk();
            *result = 0;
            return true;
        }

        return false;
    }

    static bool get_float(COMMAND_ARGS)
    {
        char filename[ARG_MAX_CHARS], key[ARG_MAX_CHARS];
        float default_return;

        if (ExtractArgs(PASS_EXTRACT_ARGS, &filename, &key, &default_return))
        {
            objson::jsonfile json_file(filename);
            json_file.get_entry(key, default_return);
            *result = default_return;
            return true;
        }

        return false;
    }

    static bool set_float(COMMAND_ARGS)
    {
        char filename[ARG_MAX_CHARS], key[ARG_MAX_CHARS];
        float value;

        if (ExtractArgs(PASS_EXTRACT_ARGS, &filename, &key, &value))
        {
            objson::jsonfile json_file(filename);
            json_file.set_entry(key, value);
            json_file.flush_to_disk();
            *result = 0;
            return true;
        }

        return false;
    }

    static bool get_form(COMMAND_ARGS)
    {
        char filename[ARG_MAX_CHARS], key[ARG_MAX_CHARS];
        TESForm *default_return;

        if (ExtractArgsEx(PASS_EXTRACT_ARGS_EX, &filename, &key, &default_return))
        {
            (UInt32 *)result = default_return->refID;
            std::string temp_str("");
            objson::jsonfile json_file(filename);
            json_file.get_entry(key, temp_str);
            objson::tesforms::string_to_ref_id(temp_str, (UInt32 *)result);
            return true;
        }

        return false;
    }

    static bool set_form(COMMAND_ARGS)
    {
        char filename[ARG_MAX_CHARS], key[ARG_MAX_CHARS];
        TESForm *value;

        if (ExtractArgsEx(PASS_EXTRACT_ARGS_EX, &filename, &key, &value))
        {
            objson::jsonfile json_file(filename);
            json_file.set_entry(key, objson::tesforms::FormToString(value));
            json_file.flush_to_disk();
            *result = 0;
            return true;
        }

        return false;
    }

    static bool erase_key(COMMAND_ARGS)
    {
        char filename[ARG_MAX_CHARS], key[ARG_MAX_CHARS];

        if (ExtractArgs(PASS_EXTRACT_ARGS, &filename, &key))
        {
            objson::jsonfile json_file(filename);
            json_file.remove_entry(key);
            json_file.flush_to_disk();
            *result = 0;
            return true;
        }

        return false;
    }

    static bool list_keys(COMMAND_ARGS)
    {
        char filename[ARG_MAX_CHARS], key[ARG_MAX_CHARS];

        if (ExtractArgs(PASS_EXTRACT_ARGS, &filename, &key))
        {
            objson::jsonfile json_file(filename);
            std::vector<std::string> key_list;
            json_file.list_keys(key, key_list);
            OBSEArray *obse_array = objson::obseplugin::ArrayFromStdVector(key_list, scriptObj);
            objson::obseplugin::kOBSEArrayVar->AssignCommandResult(obse_array, result);
            *result = 0;
            return true;
        }

        return false;
    }

    ParamInfo params_get_string[3] = {
        {"Filename", kParamType_String, 0},
        {"Key", kParamType_String, 0},
        {"Default", kParamType_String, 0}};

    ParamInfo params_set_string[3] = {
        {"Filename", kParamType_String, 0},
        {"Key", kParamType_String, 0},
        {"Value", kParamType_String, 0}};

    ParamInfo params_get_int[3] = {
        {"Filename", kParamType_String, 0},
        {"Key", kParamType_String, 0},
        {"Default", kParamType_Integer, 0}};

    ParamInfo params_set_int[3] = {
        {"Filename", kParamType_String, 0},
        {"Key", kParamType_String, 0},
        {"Value", kParamType_Integer, 0}};

    ParamInfo params_get_float[3] = {
        {"Filename", kParamType_String, 0},
        {"Key", kParamType_String, 0},
        {"Default", kParamType_Float, 0}};

    ParamInfo params_set_float[3] = {
        {"Filename", kParamType_String, 0},
        {"Key", kParamType_String, 0},
        {"Value", kParamType_Float, 0}};

    ParamInfo params_get_form[3] = {
        {"Filename", kParamType_String, 0},
        {"Key", kParamType_String, 0},
        {"Default", kOBSEParamType_Form, 0}};

    ParamInfo params_set_form[3] = {
        {"Filename", kParamType_String, 0},
        {"Key", kParamType_String, 0},
        {"Value", kOBSEParamType_Form, 0}};

    ParamInfo params_erase_key[2] = {
        {"Filename", kParamType_String, 0},
        {"Key", kParamType_String, 0}};

    ParamInfo params_list_keys[2] = {
        {"Filename", kParamType_String, 0},
        {"KeyPath", kParamType_String, 0}};

    CommandInfo commandinfo_get_string = {
        "JsonGetString",
        "",
        0,
        "Read string value from JSON",
        0,
        3,
        params_get_string,
        get_string};

    CommandInfo commandinfo_set_string = {
        "JsonSetString",
        "",
        0,
        "Write string value to JSON",
        0,
        3,
        params_set_string,
        set_string};

    CommandInfo commandinfo_get_int = {
        "JsonGetInt",
        "",
        0,
        "Read integer value from JSON",
        0,
        3,
        params_get_int,
        get_int};

    CommandInfo commandinfo_set_int = {
        "JsonSetInt",
        "",
        0,
        "Write integer value to JSON",
        0,
        3,
        params_set_int,
        set_int};

    CommandInfo commandinfo_get_float = {
        "JsonGetFloat",
        "",
        0,
        "Read floating point value from JSON",
        0,
        3,
        params_get_float,
        get_float};

    CommandInfo commandinfo_set_float = {
        "JsonSetFloat",
        "",
        0,
        "Write floating point value to JSON",
        0,
        3,
        params_set_float,
        set_float};

    CommandInfo commandinfo_get_form = {
        "JsonGetForm",
        "",
        0,
        "Read form value from JSON",
        0,
        3,
        params_get_form,
        get_form};

    CommandInfo commandinfo_set_form = {
        "JsonSetForm",
        "",
        0,
        "Write form value to JSON",
        0,
        3,
        params_set_form,
        set_form};

    CommandInfo commandinfo_erase_key = {
        "JsonEraseKey",
        "",
        0,
        "Erase key and value from JSON",
        0,
        2,
        params_erase_key,
        erase_key};

    CommandInfo commandinfo_list_keys = {
        "JsonListKeys",
        "",
        0,
        "List all keys in a key path in JSON",
        0,
        2,
        params_list_keys,
        list_keys};

}
