#pragma once
#include <charconv>
#include <string>

#include "obse/GameData.h"
#include "obse/ModTable.h"
#include "obse/ScriptUtils.h"

#include <nlohmann/json.hpp>

#include "definitions.hpp"

/**
 * Convert an std::vector into an OBSEArray.
 *
 * The std::vector entries are first added to another vector of OBSEElement
 * entries, after which that vector is converted into an OBSEArray and returned.
 *
 * @param std_vector Vector to convert into an OBSEArray.
 * @param calling_script Script from which the original OBSE function was
 * called.
 * @return OBSEArray containing the elements from the input vector.
 */
template <class T> OBSEArray *vectorToOBSEAarray(const std::vector<T> &std_vector, Script *calling_script)
{
    std::vector<OBSEElement> obse_vector;
    for (auto &element : std_vector)
    {
        obse_vector.push_back(element);
    }
    OBSEArray *obse_array = kOBSEArrayVar->CreateArray(&obse_vector[0], obse_vector.size(), calling_script);
    return obse_array;
}

nlohmann::json *obsearray_to_json(OBSEArray *obse_array)
{
    nlohmann::json data = nlohmann::json::object();
    UInt32 size = kOBSEArrayVar->GetArraySize(obse_array);

    if (size > 0)
    {
        OBSEElement *keys = (OBSEElement *)(sizeof(OBSEElement) * size);
        OBSEElement *elements = (OBSEElement *)(sizeof(OBSEElement) * size);
        if (kOBSEArrayVar->GetElements(obse_array, elements, keys))
        {
            for (int i = 0; i < size; i++)
            {
                switch (elements[i].GetType())
                {
                case kDataType_Numeric:
                }

                std::optional<std::string> key;
                switch (keys[i].GetType())
                {
                case kDataType_Numeric:
                    key = std::to_string(keys[i].Number());
                    break;
                case kDataType_String:
                    key = keys[i].String();
                    break;
                case kDataType_Form:
                    key = form_to_string(keys[i].Form());
                    break;
                default:
                    LOGMESSAGE("Invalid key type: %d", keys[i].GetType());
                    break;
                }

                if (key.has_value())
                {
                }
            }
        }
        free(keys);
        free(elements);
    }
}

/**
 * Convert a string representation of TESForm into its Form ID.
 *
 * Given a string in the form of "plugin.esp:00000000", this function will
 * attempt to retrieve the Form ID of the corresponding TESForm and return it,
 * or 0 if the form does not exist, such as when the plugin is not loaded or the
 * given Form ID within that plugin is invalid.
 *
 * Originally adapted from GetFormFromMod in Commands_Script.cpp from OBSE.
 *
 * @param form_string String representation of a form.
 * @return Form ID of the form.
 */
UInt32 string_to_form_id(std::string form_string)
{
    UInt32 result = 0;
    size_t colon_index;
    UInt8 mod_index;

    if ((colon_index = form_string.find(':')) != std::string::npos)
    {
        std::string mod_name = form_string.substr(0, colon_index);
        std::string form_id = form_string.substr(colon_index + 1);

        // The saved mod name in JSON is OBSEPLUGIN_DYNAMIC_REF_PLUGIN_NAME for
        // dynamic refs not declared in a mod Furthermore, when the mod is not
        // loaded, the mod index is 0xFF (255) Valid mod indexes are from 0 to 254
        if (mod_name.compare(OBSEPLUGIN_DYNAMIC_REF_PLUGIN_NAME) == 0 ||
            (mod_index = ModTable::Get().GetModIndex(mod_name)) != 0xFF)
        {
            UInt32 form_id_value;

            if (std::from_chars(form_id.data(), form_id.data() + form_id.size(), form_id_value).ec == std::errc{})
            {
                // The following two lines add the mod index to the form before sending
                // it for a lookup
                form_id_value &= 0x00FFFFFF;
                form_id_value |= (mod_index << 24);
                TESForm *form;
                if ((form = LookupFormByID(form_id_value)) != nullptr)
                {
                    result = form->refID;
                }
            }
        }
    }

    if (result == 0)
    {
        LOGMESSAGE("string_to_form: unresolved value \"%s\"", form_string.c_str());
    }

    return result;
}

/**
 * Convert a TESForm into its string representation.
 *
 * Given a TESForm, return the string representation in the form of
 * "plugin.esp:00000000". This should help with storing forms in a load
 * order-independent manner.
 *
 * Originally adapted from GetFormIDString in Commands_Script.cpp from OBSE.
 *
 * @param form TESForm to get a string representation for.
 * @return String representing the form.
 */
std::string form_to_string(TESForm *form)
{
    std::string form_string;

    char form_id_string[OBSEPLUGIN_REF_ID_LENGTH] = {0};
    const char *mod_name = (*g_dataHandler)->GetNthModName(form->GetModIndex());
    sprintf_s(form_id_string, sizeof(form_id_string), "%08X", form->refID);
    form_string = std::string(form_id_string) + ":" + std::string(mod_name);
    free(&mod_name);

    return form_string;
}
