#include "MemoryCache.hpp"

namespace objson::MemoryCache {

	// memory cache to avoid excess disk I/O - for reading files only
	static std::map<std::string, nlohmann::json> MemoryCache;

	void Erase(std::string &Filename) {
		MemoryCache.erase(Filename);
	}

	void Find(std::string &Filename, nlohmann::json &Data) {
		std::map<std::string, nlohmann::json>::iterator it;
		if ((it = MemoryCache.find(Filename)) != MemoryCache.end()) {
			Data = it->second;
		}
	}

	void Push(std::string &Filename, nlohmann::json &JsonData) {
		if (MemoryCache.size() > MEMCACHE_MAX_FILECOUNT) {
			MemoryCache.clear();
		}
		MemoryCache.insert(std::pair<std::string, nlohmann::json>(Filename, JsonData));
	}

}

