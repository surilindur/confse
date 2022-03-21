#pragma once

#include "json\single_include\nlohmann\json.hpp"

// memory cache size in number of files to cache (file size not inspected)
#define MEMCACHE_MAX_FILECOUNT 10

namespace objson::MemoryCache {

	void Erase(std::string &);
	void Find(std::string &, nlohmann::json &);
	void Push(std::string &, nlohmann::json &);

}
