#include "obse/ModTable.h"
#include "obse/ScriptUtils.h"
#include "obse/GameData.h"

namespace objson::tesforms
{
    // Converts a string input into a TESForm and outputs the Form ID as an UInt32
    // Originally adapted from GetFormFromMod in Commands_Script.cpp from OBSE
    UInt32 string_to_form(std::string& form_string)
    {
        UInt32 result = 0;

        if ((size_t colon_index = form_string.find(':')) != std::string::npos)
        {
            std::string mod_name = form_string.substr(0, colon_index);

            // The saved mod name in JSON is "NONE" for dynamic refs not declared in a mod
            // Furthermore, when the mod is not loaded, the mod index is 0xFF (255) where
            // valid mod indexes are from 0 to 254
            if (mod_name.compare("NONE") == 0 || (UInt8 mod_index = ModTable::Get().GetModIndex(mod_name)) != 0xFF)
            {
                // std::stoul converts the first parameter (string), starting from
                // the character after the second parameter (index), using the base
                // provided as the third parameter
                UInt32 ref_id_temp = std::stoul(form_string, colon_index, 16);

                if (ref_id_temp > 0)
                {
                    // The following two lines seem to just add the mod index to
                    // the form before sending it for a lookup
                    ref_id_temp &= 0x00FFFFFF;
                    ref_id_temp |= (mod_index << 24);
                    if ((TESForm *form = LookupFormByID(ref_id_temp)) != nullptr)
                    {
                        result = form->refID;
                    }
                }
            }
        }

        if (result == 0)
        {
            Log_Print("string_to_form: unresolved value \"%s\"", form_string.c_str());
        }

        return result;
    }

    // adapted from GetFormIDString in Commands_Script.cpp from OBSE
    std::string form_to_string(TESForm& form)
    {
        std::string form_string;

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
