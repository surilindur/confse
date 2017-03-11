#pragma once

// obse includes
#include "dependencies/xOBSE/obse/obse/ModTable.h"
#include "dependencies/xOBSE/obse/obse/ScriptUtils.h"
#include "dependencies/xOBSE/obse/obse/GameData.h"

// project includes
#include "Logging.hpp"

// definitions
#define MOD_NAME_LENGTH 0x100
#define REF_ID_LENGTH 0x20

namespace objson::FormManager {

	void StringToRefID(std::string, UInt32 *);
	std::string FormToString(TESForm *);

}
