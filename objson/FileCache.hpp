#include "Definitions.hpp"

namespace ObJson::FileCache {

    // memory cache to avoid excess disk I/O - for reading files only
    static std::map<std::string, nlohmann::json> filecache;

    void Get(std::string &filename, nlohmann::json &data) {
        std::map<std::string, nlohmann::json>::iterator it;
        if ((it = filecache.find(filename)) != filecache.end()) {
            data = it->second;
        }
    }

    void Set(std::string &filename, nlohmann::json &json) {
        if (filecache.size() > MEMCACHE_MAX_FILECOUNT) {
            filecache.clear();
        }
        filecache.insert(std::pair<std::string, nlohmann::json>(filename, json));
    }

}
