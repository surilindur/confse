#pragma once

// file and exception includes
#include <fstream>
#include <exception>

// project includes
#include "Logging.hpp"
#include "MemoryCache.hpp"

namespace objson::JsonManager {

	// invalid characters for file path, borrowed from ConScribe by shademe
	static const std::string InvalidChars("/*:?\"<>;|");

	class JsonObject {

		private:

		nlohmann::json Data;
		std::string Filename;

		template<typename T>
		void SetDataValue(char *KeyPath, T &Value, nlohmann::json &JsonData) {
			char *Separator;
			try {
				if ((Separator = strchr(KeyPath, '.')) != NULL) {
					*Separator = '\0';
					SetDataValue(Separator + 1, Value, JsonData[KeyPath]);
				} else {
					try {
						JsonData[KeyPath] = Value;
					} catch (...) { // overwrite existing value with JSON object and then set the value
						JsonData = nlohmann::json();
						JsonData[KeyPath] = Value;
					}
				}
			} catch (std::exception &e) {
				Log_Print("JsonObject::SetDataValue, Exc (%s), Key (%s)", e.what(), KeyPath);
			} catch (...) {}
		}

		template<typename T>
		void GetDataValue(char *KeyPath, T &ReturnValue, nlohmann::json &JsonData) {
			char *Separator;
			try {
				if ((Separator = strchr(KeyPath, '.')) != NULL) {
					*Separator = '\0';
					if (JsonData.count(KeyPath) > 0) {
						GetDataValue(Separator + 1, ReturnValue, JsonData[KeyPath]);
					}
				} else {
					ReturnValue = JsonData[KeyPath];
				}
			} catch (std::exception &e) {
				Log_Print("JsonObject::GetDataValue, Exc (%s), Key (%s)", e.what(), KeyPath);
			} catch (...) {}
		}

		void EraseDataValue(char *KeyPath, nlohmann::json &JsonData) {
			char *Separator;
			try {
				if ((Separator = strchr(KeyPath, '.')) != NULL) {
					*Separator = '\0';
					if (JsonData.count(KeyPath) > 0) {
						EraseDataValue(Separator + 1, JsonData[KeyPath]);
					}
				} else {
					JsonData.erase(KeyPath);
				}
			} catch (std::exception &e) {
				Log_Print("JsonObject::EraseDataValue, Exc (%s), Key (%s)", e.what(), KeyPath);
			} catch (...) {}
		}

		void ListObjectKeys(char *KeyPath, std::vector<std::string> &ReturnValue, nlohmann::json &JsonData) {
			char *Separator;
			try {
				if ((Separator = strchr(KeyPath, '.')) != NULL) {
					*Separator = '\0';
					if (JsonData.count(KeyPath) > 0) {
						ListObjectKeys(Separator + 1, ReturnValue, JsonData[KeyPath]);
					}
				} else {
					ReturnValue.clear();
					for (nlohmann::json::iterator it = JsonData[KeyPath].begin(); it != JsonData[KeyPath].end(); ++it) {
						ReturnValue.push_back(it.key());
					}
				}
			} catch (std::exception &e) {
				Log_Print("JsonObject::ListKeys, Exc (%s), Key (%s)", e.what(), KeyPath);
			} catch (...) {}
		}

		public:

		JsonObject(char *FilenameFromUser) {
			Filename = std::string(FilenameFromUser);
			if (Filename.find("..") != std::string::npos || Filename.find_first_of(InvalidChars) != std::string::npos) {
				Log_Print("JsonObject::Constructor, InvalidFilepath (%s)", Filename);
				Filename.clear();
				return;
			}
			objson::MemoryCache::Find(Filename, Data);
			if (Data.size() > 0) {
				//Log_Print("JsonObject::Constructor, UseCached (%s)", Filename.c_str());
				return;
			}
			//Log_Print("JsonObject::Constructor, ReadFromDisk (%s)", Filename.c_str());
			try {
				std::ifstream Filehandle(objson::Interfaces::kOblivionDirectory + Filename);
				Filehandle >> Data;
				Filehandle.close();
				objson::MemoryCache::Push(Filename, Data);
			} catch (std::exception &e) {
				Log_Print("JsonObject::Constructor, Exc (%s), Filepath (%s)", e.what(), Filename.c_str());
			} catch (...) {}
		}

		void WriteToDisk() {
			if (Filename.size() < 1) {
				return;
			}
			try {
				objson::MemoryCache::Erase(Filename);
				std::ofstream Filehandle(objson::Interfaces::kOblivionDirectory + Filename);
				Filehandle << Data.dump(4) << std::endl;
				Filehandle.close();
			} catch (std::exception &e) {
				Log_Print("JsonObject::WriteToDisk, Exc (%s), Filepath (%s)", e.what(), Filename.c_str());
			} catch (...) {}
		}

		template <typename T>
		void Get(char *KeyPath, T &ReturnValue) {
			GetDataValue(KeyPath, ReturnValue, Data);
		}

		template <typename T>
		void Set(char *KeyPath, T &Value) {
			SetDataValue(KeyPath, Value, Data);
		}

		void Erase(char *KeyPath) {
			EraseDataValue(KeyPath, Data);
		}

		void ListKeys(char *KeyPath, std::vector<std::string> &ReturnValue) {
			ListObjectKeys(KeyPath, ReturnValue, Data);
		}

	};

}
