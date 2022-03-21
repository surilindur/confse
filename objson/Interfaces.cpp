#include "Interfaces.hpp"

namespace objson::Interfaces {

	PluginHandle kPluginHandle = kPluginHandle_Invalid;
	OBSEStringVarInterface *kOBSEStringVar = NULL;
	OBSEArrayVarInterface *kOBSEArrayVar = NULL;
	OBSEScriptInterface *kOBSEScript = NULL;
	std::string kOblivionDirectory;

	OBSEArray *ArrayFromStdVector(const std::vector<std::string> &StdVector, Script *callingScript) {
		std::vector<OBSEElement> OBSEVector;
		for (auto &Element : StdVector) {
			OBSEVector.push_back(Element.c_str());
		}
		OBSEArray *OBSEArr = kOBSEArrayVar->CreateArray(&OBSEVector[0], OBSEVector.size(), callingScript);
		return OBSEArr;
	}

}
