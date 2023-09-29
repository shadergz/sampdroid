# saonline-mobile
Plugin for GTA:SA v210 to enable players to join SAMP/MTA matches 

#### Project aims
- Complete replacement for build69 (SAMP for ARMv7 from GTASA v1.8 what has deprecated)

#### How to compile?
- Before start make sure that you have configured the `env.json` file in the project folder

#### Limitations of hook system
- Can't to replace or emplace instructions that are a 'ret' or any type of branch
- Can't to replace or save any instruction that changes or modifies the x8 or x30 registers (only the function prologue is allowed)

#### Todo
```
- [ ] Handle vehicles and NPCs pools
- [ ] Get/Set player coordinates (*)
- [ ] Create single and multiplayer player objects
- [ ] Handle life and AFK
- [ ] Create SAMP splash screen
- [ ] Better handle entries systems
- [x] Update internal libraries and fix the code base inconsistency (*)
- [ ] Implement .txd/.dff dynamic loader (uncompress .txt for each GPU platform if needed)
- [x] Add RakNet to the vendor directory
```

#### Screenshots (work-in-progress), thorough testing, and additional fixes are still needed
![Image](screens/Screenshot_20230928-154643_GTA_SA.png)
