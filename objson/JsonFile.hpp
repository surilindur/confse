#include <iostream>
#include <fstream>
#include <filesystem>
#include <regex>

#include "json/single_include/nlohmann/json.hpp"

namespace ObJson::JsonFile
{
    // recently accessed files will have their contents cached to avoid disk reads
    static const std::map<std::filesystem::path, nlohmann::json> file_cache;

    // the game directory should be the one below which all accessed files reside
    static const std::filesystem::path game_directory;

    class JsonFile
    {
        private:

        bool allow_disk_access;
        nlohmann::json filedata;
        std::filesystem::path filepath;

        std::pair<nlohmann::json*, std::string> find(std::string& key, bool create_on_access = false)
        {
            auto* temp_data = &filedata;
            auto key_fragments = split_key(key);
            auto final_fragment = key_fragments.back();

            key_fragments.pop_back();

            if (!key_fragments.empty())
            {
                for (const auto& key_fragment : key_fragments)
                {
                    if (create_on_access && !(*temp_data)[key_fragment].is_object())
                    {
                        (*temp_data)[key_fragment] = nlohmann::json::object();
                    }

                    temp_data = &temp_data->at(key_fragment);
                }
            }

            return std::pair<nlohmann::json*, std::string>(temp_data, final_fragment);
        }

        std::vector<std::string> split_key(std::string& key)
        {
            // the key being split can be either just strings:              some.key.here
            // or it can contain indexing, in case the value is an array:   some.key[0].here
            // these will all be matched by the ECMAScript regex:           /(\w+)(\[([0-9])\])?\.?/g
            // where the match groups will be:                              1 = string key, 3 = index

            // std::regex re("(\\w+)(\\[([0-9])\\])?\\.?", std::regex_constants::ECMAScript);

            // for now, ignore indexes since arrays are not supported, and only use the basic string keys

            std::regex re("(\\w+)\\.?", std::regex_constants::ECMAScript | std::regex_constants::optimize);

            auto key_fragments = std::vector<std::string>();

            for (std::smatch m; std::regex_search(key, m, re); key = m.suffix())
            {
                key_fragments.push_back(m[1]);
            }

            return key_fragments;
        }

        public:

        JsonFile(std::string& filename)
        {
            // normalisation and checking code borrowed from StackOverflow:
            // https://stackoverflow.com/questions/61123627/check-if-an-stdfilesystempath-is-inside-a-directory
            filedata = nlohmann::json::object();
            filepath = (game_directory / "data" / filename).lexically_normal();
            auto[game_directory_end, nothing] = std::mismatch(game_directory.begin(), game_directory.end(), filepath.begin());
            allow_disk_access = game_directory_end == game_directory.end();  // failing this check could be logged somehow

            if (allow_disk_access && std::filesystem::exists(filepath))
            {
                std::ifstream infile(filepath);
                infile >> filedata;  // this should automatically close at the end of the scope? really?
            }

            //std::cout << "Filepath is " << filepath << std::endl;
            //std::cout << "Data is " << filedata << std::endl;
        }

        template<typename T>
        void get_entry(std::string& key, T& value)
        {
            std::pair<nlohmann::json*, std::string> temp_data = find(key);

            if (!temp_data.first->empty())
            {
                temp_data.first->at(temp_data.second).get_to(value);
            }
        }

        template<typename T>
        void set_entry(std::string& key, T& value)
        {
            std::cout << "Set " << key << " to " << value << std::endl;
            std::pair<nlohmann::json*, std::string> temp_data = find(key, true);
            (*temp_data.first)[temp_data.second] = value;
        }

        void remove_entry(std::string& key)
        {
            std::pair<nlohmann::json*, std::string> temp_data = find(key);

            if (!temp_data.first->empty())
            {
                temp_data.first->erase(temp_data.second);
            }
        }

        void list_keys(std::string& key, std::vector<std::string>& keys)
        {
            std::pair<nlohmann::json*, std::string> temp_data = find(key);

            if (!temp_data.first->empty() && (*temp_data.first)[temp_data.second].is_object())
            {
                for (auto& [key, value] : (*temp_data.first)[temp_data.second].items())
                {
                    keys.push_back(key);
                }
            }
        }

        void flush_to_disk()
        {
            if (allow_disk_access)
            {
                std::ofstream file(filepath);
                file << std::setw(2) << filedata << std::endl;
            }
        }
    };
}
