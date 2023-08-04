# SAMobile
An experimental impl of MTA and SAMP for GTA SA v210 AArch64 (for more details look at OpenSA repo)

## Project aims
- Complete replacement of build69 (SAMP for ARMv7 from GTASA v1.8 what has deprecated)

## How to compile?
- Before start make sure that you have configured the `env.json` file in the project folder

## Todo
```
- [x] To load our 3 (three) texture databases at startup
- [x] To create two database directories that utilize the game's texture database system, providing users with the ability to load or replace game objects and textures
- [ ] To hook class methods by jump-to-trampoline method
- [ ] To fix CEntity_UpdateRwFrame hooking and restores from LP, making both LP and PC versions work
- [ ] To fix renderByEachGameSecond and ImGUI backend for RenderWare
```
