# SAMP for Android GTASA 2.0
A plugin for the Android version of GTASA to allow players to join SAMP (PC) matches

### Project aims
- Complete replacement for build70-plus (Which has been deprecated)

### How to compile?
- Before starting, make sure that you have configured the 'env.json' file in the project folder

### Limitations of hook system
- Can't replace or emplace instructions that are a 'ret' or any type of branch
- Can't replace or save any instruction that changes or modifies the x8 or x30 registers (only the function prologue is allowed)

### Todo
```
- [ ] Handle vehicles and NPCs pools
- [ ] Get/Set player coordinates (*)
- [ ] Create single and multiplayer player objects
- [ ] Handle life and AFK
- [ ] Create SAMP splash screen
- [ ] Better handle entries systems
- [ ] Implement .txd/.dff dynamic loader (uncompress .txt for each GPU platform if needed)
```
