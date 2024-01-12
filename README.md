<p align="center">
    <img alt="logo" src="https://raw.githubusercontent.com/surilindur/confse/main/images/logo.svg" width="140">
</p>

<p align="center">
  <a href="https://github.com/surilindur/confse/actions/workflows/ci.yml"><img alt="CI" src=https://github.com/surilindur/confse/actions/workflows/ci.yml/badge.svg?branch=main"></a>
  <a href="https://isocpp.org/"><img alt="C++" src="https://img.shields.io/badge/%3C%2F%3E-C++-%23f34b7d.svg"></a>
  <a href="https://opensource.org/licenses/MIT"><img alt="License: MIT" src="https://img.shields.io/badge/License-MIT-blue.svg"></a>
</p>

**ConfSE** is an experimental [Oblivion Script Extender](/llde/xOBSE) (OBSE) plugin that aims to enable the loading and saving of JSON files from within Oblivion scripts. This is accomplished through the use of OBSE's `array_var` data type, that be either an array or map. The plugin uses [an existing JSON library for C++](https://github.com/nlohmann/json) together with some simple Oblivion-specific type conversions.

The following type conversions are performed.

| Oblivion                        | JSON                    | Notes                                                |
|:--------------------------------|:------------------------|:-----------------------------------------------------|
| `array_var` Array               | `[]`                    | Uninitialized saved as `null`                        |
| `array_var` Map                 | `{}`                    | Uninitialized saved as `null`                        |
| `string_var`                    | String                  | Uninitialized saved as `null`                        |
| `ref`                           | `"plugin.esp:00000000"` | Unbound is saved as `null`                           |
| `short`, `int`, `long`, `float` | Number                  | Each value gets loaded as the smallest possible type |

The design goal of the plugin is to offer a straightforward mapping from OBSE's `array_var`s, including nested ones, to a corresponding JSON strcture and vice versa.

**This is a work-in-progress thing and is NOT intended for use. At the moment, it does not even compile.**

This project depends on the following external projects and libraries:

* [**Oblivion Script Extender**](/llde/xOBSE) as the targeted OBSE version
* [**JSON for Modern C++**](/nlohmann/json) as the JSON library used to load and save files
* **MSBuild** for building the plugin project

The latest compiled DLL at a given time should be available from the GitHub Actions artifacts for the CI workflow.

## Development Setup

The repository has all its dependencies linked as Git submodules, so they need to be fetched when cloning, as well:

```bash
git clone --recurse-submodules https://github.com/surilindur/confse
```

Any changes can then be made, and building the plugin should be possible via

```bash
msbuild confse/confse.vcxproj -p:BuildInParallel=true
```

## Added Script Commands

The following new commands are added by the plugin to load JSON:

**JsonLoadFile** - attempts to load the contents of the specified file as JSON, returning the result as an `array_var`. When loading fails, returns either the `array_var` passed as `default` or an uninitialized value.

    (array_var) JsonLoadFile filename:string_var default:array_var

**JsonLoadString** - attempts to load the contents of the provided `string_var` as JSON, returning the result as an `array_var`. When loading fails, returns either the `array_var` passed as `default` or an uninitialized value.

    (array_var) JsonLoadString data:string_var default:array_var

**JsonSaveFile** - attempts to serialize the provided `array_var` to the specificed file as JSON, returning `0` after success or `-1` if an error occurred.

    (status:int) JsonSaveFile data:array_var filename:string_var

**JsonSaveString** - attempts to serialize the provided `array_var` as a string, returning a `string_var` that will either contain the resulting string or be uninitialized if an error occurred.

    (string_var) JsonSaveString data:array_var

## Issues

Please feel free to report any issues on the GitHub issue tracker.

## License

This code is copyrighted and released under the [MIT license](http://opensource.org/licenses/MIT).
