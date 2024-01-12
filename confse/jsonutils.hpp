#include <iostream>
#include <fstream>
#include <optional>

#include <nlohmann/json.hpp>

#include "definitions.hpp"

std::optional<std::filesystem::path> resolve_filepath(std::string filepath)
{
    auto const path = std::filesystem::canonical(filepath);
    auto it = std::search(path.begin(), path.end(), oblivion_directory.begin(), oblivion_directory.end());
    if (it == path.begin())
    {
        return path;
    }
}

std::optional<nlohmann::json> json_load_file(std::string filepath)
{
    auto path = resolve_filepath(filepath);
    if (path.has_value())
    {
        auto data = nlohmann::json::object();
        std::ifstream infile(path.value());
        try
        {
            infile >> data;
            return data;
        }
        catch (const nlohmann::json::exception &ex)
        {
            LOGMESSAGE("Loading from \"%s\" failed: %d \"%s\"", path.value(), ex.id, ex.what());
        }
        catch (...)
        {
            LOGMESSAGE("Loading from \"%s\" failed", path.value());
        }
    }
}

std::optional<nlohmann::json> json_load_string(std::string input)
{
    try
    {
        return nlohmann::json::parse(input);
    }
    catch (const nlohmann::json::exception &ex)
    {
        LOGMESSAGE("Parsing failed: %d \"%s\"\n%s", ex.id, ex.what(), input);
    }
    catch (...)
    {
        LOGMESSAGE("Parsing failed:\n%s", input);
    }
}

boolean json_save_file(std::string filepath, nlohmann::json data)
{
    auto path = resolve_filepath(filepath);
    if (path.has_value())
    {
        std::ofstream outfile(path.value());
        try
        {
            outfile << std::setw(2) << data << std::endl;
            return true;
        }
        catch (const nlohmann::json::exception &ex)
        {
            LOGMESSAGE("Saving \"%s\" failed: %d \"%s\"", path.value(), ex.id, ex.what());
        }
        catch (...)
        {
            LOGMESSAGE("Saving \"%s\" failed", path.value());
        }
    }
    return false;
}

std::optional<std::string> json_save_string(nlohmann::json data)
{
    try
    {
        return data.dump(2);
    }
    catch (const nlohmann::json::exception &ex)
    {
        LOGMESSAGE("Serializing failed: %d \"%s\"", ex.id, ex.what());
    }
    catch (...)
    {
        LOGMESSAGE("Serializing failed");
    }
}
