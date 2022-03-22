#include "obse/ModTable.h"
#include "obse/ScriptUtils.h"
#include "obse/GameData.h"

#include "Definitions.hpp"

namespace ObJson::TESForms
{
    // adapted from GetFormFromMod in Commands_Script.cpp from OBSE
    void StringToRefID(std::string ref_id_input, UInt32 *ref_id_integer)
    {
        if ((size_t separator = ref_id_input.find(':')) == std::string::npos) {
            Log_Print("StringToRefID: Invalid string \"%s\"", ref_id_string.c_str());
            return;
        }

        char mod_name[OBJSON_MOD_NAME_LENGTH];
        char ref_id_string[OBJSON_REF_ID_LENGTH];

        strcpy_s(ref_id_string, ref_id_input.substr(0, separator).c_str());
        strcpy_s(mod_name, ref_id_input.substr(separator + 1).c_str());

        UInt8 mod_index = 0xFF;

        if (_stricmp(mod_name, "NONE") != 0) { // "NONE" for dynamic refs
            if ((mod_index = ModTable::Get().GetModIndex(mod_name)) == 0xFF) {
                return; // mod is not loaded (255, where valid ones are 0 to 254)
            }
        }

        UInt32 ref_id;
        TESForm *form;

        if (sscanf_s(ref_id_string, "%x", &ref_id) == 1) {
            ref_id &= 0x00FFFFFF;
            ref_id |= (mod_index << 24);
            if ((form = LookupFormByID(ref_id)) != NULL) {
                *ref_id_integer = (UInt32)ref_id;
            }
        }
    }

    // adapted from GetFormIDString in Commands_Script.cpp from OBSE
    std::string FormToString(TESForm *form)
    {
        std::string form_string("");

        if (form)
        {
            char form_id_string[OBJSON_REF_ID_LENGTH] = { 0 };
            const char *mod_name = (*g_dataHandler)->GetNthModName(form->GetModIndex());
            sprintf_s(form_id_string, sizeof(form_id_string), "%08X", form->refID);
            form_string = std::string(form_id_string) + ":" + std::string(mod_name);
            free(&mod_name);
        }

        return form_string;
    }

}
