# MtMobile 

An experimental implementation of MTA and SAMP for GTA SA v210 aarch64 (for more details look at OpenSA repo)

## Project aims
- Complete replacement of build69 (SAMP for armv7a GTASA v1.8 !deprecated!)

## Todo
- [ ] Produce two database directories (we need to to some tools)
- [ ] To hook InitialiseRenderWare() (`On ARMv7a game version`) and to load our texture databases (for samp and mta) into the engine RenderWare memory space (The renderware will try to load all database from a relative path inside of .../files/texdb/`DATABASE_NAME`)
