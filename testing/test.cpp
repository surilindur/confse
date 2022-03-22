#include "objson/JsonFile.hpp"

int main() {
    std::cout << "Testing..." << std::endl;

    std::string filename = "JsonFileTest.json";

    ObJson::JsonFile::JsonFile file = ObJson::JsonFile::JsonFile(filename);

    std::cout << "Set values..." << std::endl;

    std::string value = "bbbbb";

    std::string key = "aaaa";
    file.set_entry(key, value);

    float value_f = 1.23456;

    key = "cccccc.ddddd";
    file.set_entry(key, value_f);

    int value_i = 1987732;

    key = "cccccc.kkkk";
    file.set_entry(key, value_i);

    std::cout << "Get values..." << std::endl;

    value_i = 0;

    key = "cccccc.kkkk";
    file.get_entry(key, value_i);

    std::cout << value_i << std::endl;

    std::cout << "Get keys..." << std::endl;

    key = "cccccc";
    auto all_keys = std::vector<std::string>();

    file.list_keys(key, all_keys);

    for (const auto& k : all_keys)
    {
        std::cout << k << ",";
    }

    std::cout << std::endl;

    std::cout << "Flush to disk..." << std::endl;

    file.flush_to_disk();

    std::cout << "Finished!" << std::endl;
}
