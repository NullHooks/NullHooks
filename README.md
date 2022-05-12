<div align="center">
    <h1>NullHooks</h1>
    <b>CS:GO cheat I made based on designer's base, and many other cheats I found. Mainly made for learning.</b>
    <div></div>
</div>

# Table of contents
1. [Description](#description)
2. [Features](#features)
    - [Visuals - ESP](#visuals---esp)
    - [Visuals - Misc](#visuals---misc)
    - [Misc](#misc)
3. [Todo](#todo)
4. [Screenshots](#screenshots)

## Description
Edgy name but not as cool as *PissHooks™* (RIP).

As I said I made this project to learn about game hacking and cpp in general, so expect some bad practises and errors, but feel free to improve them!

See [contributing.md](CONTRIBUTING.md) for contributing to the project and [references.md](REFERENCES.md) for a list of cheats that helped me learn while making the project.

## Features
#### Visuals - ESP
- Enable/disable team ESP
- Player glow
- Box ESP
- Player skeleton
- Name ESP
- Health ESP
- Line
- Player info (flashed, weapons, etc.)
    - Current weapon
    - `A` - Armor
    - `F` - Is flashed
    - `S` - Is scoped
    - `D` - Is defusing (soft)
    - `D` - Has defuser (hard)

#### Visuals - Misc
- C4 glow
- C4 timer and bar
- Dropped weapons and grenades glow
- Dropped weapons and grenades name
- Nade projectile ESP
- Nade prjectile duration
- Grenade prediction
- No flash
- No scope
- Custom crosshair
- Recoil crosshair

#### Misc:
- BunnyHop
- Spectator list
- Fov changer
- Stats watermark (*username, time, fps, speed...*)

## Todo:
- [X] Clean the code
- [X] Choose team not working (click)
- [X] Move *NullHooks-Lite* to *NullHooks* branches
- [X] Change spectator list style
- [X] Add recoil crosshair
- [X] Make spectator list movable (same as menu)
- [X] Add active nade ESP
	- [X] Fix smoke class id
- [X] Add nade prediction
- [X] Add player skeleton ESP
- [X] Bring menu tabs back (rip)
- [X] Make spectator list width dynamic depending on name lengths
- [X] Add line esp
- [X] Add glows 
	- [X] Add bomb glow
	- [X] Add player glow
	- [X] Add chicken glow
	- [X] Add weapon glow
- [X] Add bomb timer
- [X] Add weapon info about player
- [X] Fix entity names showing outside the map if they dont exist (See `entity_esp.cpp`)
- [X] Remove glow on nade projectiles
- [X] Remove esp on spectated player
- [X] Nade projectiles outside screen
- [X] Add option for disabling watermark and stats
- [ ] Make gui checkboxes enable by clicking the name of the feature
- [ ] Support wchar strings in renderer (Russian characers for example, see comment in renderer)
- [ ] Add "defusing" to bomb timer
- [ ] Reset bomb timer on new round
- [ ] Add aim stuff
- [ ] Add chams
- [ ] Add bullet tracers :(
- [ ] Add config and all that
- [ ] Entity glow won't turn off on weapons (will turn off if another glow is on)
- [ ] Fix both spectator list and menu dragging when overlapped (See bottom of `menu.cpp`)

## Screenshots
*Some screenshots might be a bit outdated...*  

![Screenshot 10](screenshots/screenshot10.png)
![Screenshot 8](screenshots/screenshot8.png)
![Screenshot 9](screenshots/screenshot9.png)
![Screenshot 11](screenshots/screenshot11.png)
