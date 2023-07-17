# SAMobile
An experimental impl of MTA and SAMP for GTA SA v210 AArch64 (for more details look at OpenSA repo)

## Project aims
- Complete replacement of build69 (SAMP for ARMv7 GTASA v1.8 that has deprecated)

## How to compile?
- Before start make sure that you have configured the `env.json` file in the project folder:
```json
{
    "install_dir": "factory",
    "build_basedir": "build",

    "enable_debug": false,

    "baseapk_file": "SharedContents/GTASAv210.apk",

    "apktool_program": "Tools/apktool_2.8.0.jar",

    "android_keypath_env": "ANDROID_KEY_FILEPATH",
    "android_keyalias_env": "ANDROID_KEY_ALIAS",
    "android_keypass_env": "ANDROID_KEY_PASS"
}

```

## Todo
- [ ] Load our 3 (three) texture databases at startup
- [ ] Produce two database directories (we need to take advantage of all game texture database system, given a facilities to the user to load or replace game objects and textures)
- [X] To hook InitializeRenderWare() (`On ARMv7 game version`) and to load our texture databases (for samp and mta) into the engine RenderWare memory space (The renderware will try to load all database from a relative path inside of .../files/texdb/`DATABASE_NAME`)
- [x] Hooking AArch64 functions

