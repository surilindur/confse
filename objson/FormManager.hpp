#pragma once

#include "obse\ModTable.h"
#include "obse\ScriptUtils.h"
#include "obse\GameData.h"

#include "Logging.hpp"

#define MOD_NAME_LENGTH 0x100
#define REF_ID_LENGTH 0x20

namespace objson::FormManager {

	void StringToRefID(std::string, UInt32 *);
	std::string FormToString(TESForm *);

}
