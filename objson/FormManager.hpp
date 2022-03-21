#pragma once

#include "ModTable.h"
#include "ScriptUtils.h"
#include "GameData.h"

#include "Logging.hpp"

#define MOD_NAME_LENGTH 0x100
#define REF_ID_LENGTH 0x20

namespace objson::FormManager {

	void StringToRefID(std::string, UInt32 *);
	std::string FormToString(TESForm *);

}
