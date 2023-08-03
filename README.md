# SAMobile
An experimental impl of MTA and SAMP for GTA SA v210 AArch64 (for more details look at OpenSA repo)

## Project aims
- Complete replacement of build69 (SAMP for ARMv7 from GTASA v1.8 what has deprecated)

## How to compile?
- Before start make sure that you have configured the `env.json` file in the project folder:
```json
{
    "install_dir": "apkgen",
    "build_basedir": "build",
    "enable_debug": false,
    "baseapk_file": "GTASAv210.apk",
    "apktool_program": "apktool_2.8.1.jar",
    "android_keypath_env": "ANDROID_KEY_FILEPATH",
    "android_keyalias_env": "ANDROID_KEY_ALIAS",
    "android_keypass_env": "ANDROID_KEY_PASS"
}

```

## Todo
- [ ] To load our 3 (three) texture databases at startup
- [ ] To create two database directories that utilize the game's texture database system, providing users with the ability to load or replace game objects and textures
- [ ] To hook class methods by jump-to-trampoline method
- [ ] To fix CEntity_UpdateRwFrame hooking and restores from LP, making both LP and PC versions work
- [ ] To fix renderByEachGameSecond and ImGUI backend for RenderWare
