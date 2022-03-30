# Oblivion JSON Plugin

Experimental Oblivion Script Extender plugin to provide JSON file interface for Oblivion scripts, for reading and writing simple values. Emphasis should be placed on the word *experimental*.

**Note:** The project is unfinished at the moment, and the following will need to be done:

- [ ] Adding GitHub Actions for automated building
- [ ] Rewriting the code so it makes more sense and is less of a mess
- [ ] Cleaning up the project that is a huge mess
- [ ] Adding unit tests


## Dependencies

The project has the following dependencies:

* [llde/xOBSE](https://github.com/llde/xOBSE) for OBSE source code
* [nlohmann/json](https://github.com/nlohmann/json) for actual JSON interaction


## Added Commands

JsonGetString / JsonSetString - read and write string_var values

    (value:string_var) JsonGetString filename:string_var key:string_var defaultreturn:string_var
    (nothing)          JsonSetString filename:string_var key:string_var value:string_var

JsonGetForm / JsonSetForm - read and write ref values

    (value:ref) JsonGetForm filename:string_var key:string_var defaultreturn:ref
    (nothing)   JsonSetForm filename:string_var key:string_var value:ref

JsonGetInt / JsonSetInt - read and write integer values

    (value:int) JsonGetInt filename:string_var key:string_var defaultreturn:int
    (nothing)   JsonSetInt filename:string_var key:string_var value:int

JsonGetFloat / JsonSetFloa - read and write floating point values

    (value:float) JsonGetFloat filename:string_var key:string_var defaultreturn:float
    (nothing)     JsonSetFloat filename:string_var key:string_var value:float

JsonListKeys - fetch an array of keys as strings in a path in a file

    (value:array_var) JsonListKeys filename:string_var key:string_var


## Examples

Below is a small script to illustrate the usage of the plugin. It is just for reference! Do not take it too seriously. I have not tested that specific script, but it should give a general idea of how to use the plugin.

```
scriptname OBJSONTest

string_var JsonFile
string_var JsonKey
array_var TempArr

begin _Function {}

    PrintToConsole "objson: json test starting..."

    let JsonFile := "Data\ObjsonTester.json"
    let JsonKey := "TestSection"

    PrintToConsole "objson: string (valid) --> " + $( JsonGetString JsonFile JsonKey + ".StringHere" "default" )
    PrintToConsole "objson: string (invalid) --> " + $( JsonGetString JsonFile JsonKey + ".StringHereNot" "default" )

    PrintToConsole "objson: int (valid) --> %.0f" ( JsonGetInt JsonFile JsonKey + ".IntHere" -1234 )
    PrintToConsole "objson: int (invalid) --> %.0f" ( JsonGetInt JsonFile JsonKey + ".IntHereNot" -1234 )

    PrintToConsole "objson: float (valid) --> %.6f" ( JsonGetFloat JsonFile JsonKey + ".FloatHere" 9.87654 )
    PrintToConsole "objson: float (invalid) --> %.6f" ( JsonGetFloat JsonFile JsonKey + ".FloatHereNot" 9.87654 )

    PrintToConsole "objson: form (valid) --> %i" ( JsonGetForm JsonFile JsonKey + ".FormHere" PlayerRef )
    PrintToConsole "objson: form (invalid) --> %i" ( JsonGetForm JsonFile JsonKey + ".FormHereNot" PlayerRef )

    let JsonKey := "WriteTestSection"

    JsonSetString JsonFile JsonKey + ".StringOut" "this little piggie went out to JSON"
    JsonSetInt JsonFile JsonKey + ".IntOut" 1984
    JsonSetFloat JsonFile JsonKey + ".FloatOut" 3.14
    JsonSetForm JsonFile JsonKey + ".FormOut" PlayerRef

    let JsonKey := "KeyListTestSection"

    foreach TempArr <- ( JsonListKeys JsonFile JsonKey )
        PrintToConsole "objson: key --> " + $( JsonKey ) + "." + $( TempArr["value"] )
    loop

    PrintToConsole "objson: test finished!"

    sv_Destruct JsonFile JsonKey

end
```
