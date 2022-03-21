#include "FormManager.hpp"

namespace objson::FormManager {

	// adapted from GetFormFromMod in Commands_Script.cpp from OBSE
	void StringToRefID(std::string StringValue, UInt32 *ReturnValue) {

		char ModName[MOD_NAME_LENGTH], RefIDString[REF_ID_LENGTH];
		size_t Separator = StringValue.find(':');
		TESForm *Form;
		UInt8 ModIndex = 0xFF;
		UInt32 RefID;

		if (Separator == std::string::npos) {
			Log_Print("FormManager::StringToRefID, InvalidString (%s)", StringValue.c_str());
			return;
		}

		strcpy_s(RefIDString, StringValue.substr(0, Separator).c_str());
		strcpy_s(ModName, StringValue.substr(Separator + 1).c_str());

		if (_stricmp(ModName, "NONE") != 0) { // "NONE" for dynamic refs
			ModIndex = ModTable::Get().GetModIndex(ModName);
			if (ModIndex == 0xFF) {
				return; // mod is not loaded (255?)
			}
		}

		if (sscanf_s(RefIDString, "%x", &RefID) == 1) {
			RefID &= 0x00FFFFFF;
			RefID |= (ModIndex << 24);
			if ((Form = LookupFormByID(RefID)) != NULL) {
				*ReturnValue = (UInt32)RefID;
			}
		}

		return;
	}

	// adapted from GetFormIDString in Commands_Script.cpp from OBSE
	std::string FormToString(TESForm *Form) {

		char FormIDString[REF_ID_LENGTH] = { 0 };
		const char *ModName = NULL;
		std::string FormString("");

		if (Form) {
			ModName = (*g_dataHandler)->GetNthModName(Form->GetModIndex());
			sprintf_s(FormIDString, sizeof(FormIDString), "%08X", Form->refID);
			FormString = std::string(FormIDString) + ":" + std::string(ModName);
			free(&ModName);
		}

		return FormString;
	}

}
