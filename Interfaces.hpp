#pragma once

// obse includes
#include "dependencies/xOBSE/obse/obse/PluginAPI.h"

// definitions
typedef OBSEArrayVarInterface::Array OBSEArray;
typedef OBSEArrayVarInterface::Element OBSEElement;

namespace objson::Interfaces {

	extern PluginHandle kPluginHandle;
	extern OBSEScriptInterface *kOBSEScript;
	extern OBSEStringVarInterface *kOBSEStringVar;
	extern OBSEArrayVarInterface *kOBSEArrayVar;
	extern std::string kOblivionDirectory;

	OBSEArray *ArrayFromStdVector(const std::vector<std::string> &, Script *);

}
