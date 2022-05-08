# NullHooks
**CS:GO cheat I made based on the famous base, and many other cheats I found.**

Based on: [Click me](REFERENCES.md)

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

#### Visuals - Others
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
- [ ] Add aim stuff
- [ ] Add chams
- [ ] Add bullet tracers :(
- [ ] Add config and all that
- [ ] Fix both spectator list and menu dragging when overlapped (See bottom of `menu.cpp`)
- [ ] Entity glow won't turn off on existing entities
- [ ] Reset bomb timer on new round
- [ ] Add "defusing" to bomb timer
- [ ] Nade projectiles outside screen?

## Screenshots
*Some screenshots might be a bit outdated...*  

![Screenshot 10](screenshots/screenshot10.png)
![Screenshot 8](screenshots/screenshot8.png)
![Screenshot 9](screenshots/screenshot9.png)
![Screenshot 11](screenshots/screenshot11.png)
