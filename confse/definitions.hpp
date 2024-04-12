#pragma once

#include <filesystem>

// This is just a quick and hacky way to get IntelliSense to work with Wine headers on Linux.
// The types seem case-sensitive, and only full uppercase names are defined in the headers:
// <file:///usr/include/wine/windows/basetsd.h>
#ifdef WINE_WINTYPES_WORKAROUND
typedef CHAR UInt8;
typedef BYTE UChar;
typedef SHORT Int16;
typedef USHORT UInt16;
typedef LONG Int32;
typedef DWORD UInt32;
typedef LONGLONG Int64;
typedef ULONGLONG UInt64;
#endif

#include "common/IDebugLog.h"
#include "obse/ArrayVar.h"

#define OBSEPLUGIN_NAME "ConfSE"
#define OBSEPLUGIN_VERSION 1

#define OBSEPLUGIN_LOGFILE OBSEPLUGIN_NAME ".log" // The logfile ends up in the game directory
#define OBSEPLUGIN_LRU_CACHE_SIZE 10  // The JSON file LRU cache size used to avoid excessive reads from disk
#define OBSEPLUGIN_OPCODE_BASE 0x2890 // The plugin was assigned 0x2890 - 289F
#define OBSEPLUGIN_MOD_NAME_LENGTH 0x100
#define OBSEPLUGIN_REF_ID_LENGTH 0x20
#define OBSEPLUGIN_DYNAMIC_REF_PLUGIN_NAME "NONE" // The plugin name used for dynamic references

#define LOGMESSAGE(...) _MESSAGE(__VA_ARGS__)

IDebugLog gLog(OBSEPLUGIN_LOGFILE);

typedef OBSEArrayVarInterface::Array OBSEArray;
typedef OBSEArrayElement OBSEElement;

PluginHandle kPluginHandle = kPluginHandle_Invalid;
OBSEStringVarInterface *kOBSEStringVar = NULL;
OBSEArrayVarInterface *kOBSEArrayVar = NULL;
OBSEScriptInterface *kOBSEScript = NULL;

std::filesystem::path kOblivionDirectory;

#define ARG_MAX_CHARS 512
#define ExtractArgsEx(...) kOBSEScript->ExtractArgsEx(__VA_ARGS__)
#define PASS_EXTRACT_ARGS_EX paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList
