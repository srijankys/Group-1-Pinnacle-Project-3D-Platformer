# Credits and licences

Everything outside our own code is listed here. Update the "Team to fill in" lines before submitting.

## Our own work
- Lightning realm: levels, mobs' behaviour scripts, Thunderwing boss, Storm Strike ability, effects drawn in code
  (`scripts/lightning/`), ESP32 firmware (`firmware/`), serial bridge (`tools/`).
- Sound effects `assets/lightning/sfx/*.wav` (thunder, charge, dash, blip): generated for the project.
- Thunderwing dragon: drawn for our prototype and animated in code (no sprite sheet).

## Art (Lightning realm) - all free, from itch.io
| Asset | Creator | Source | Licence |
|---|---|---|---|
| Tiles, trees, bushes, backgrounds | aamatniekss; Liz Cheong | itch.io free platformer packs | free to use (check each page) |
| Thunder cloud + bolt | Balloon Cart - Elemental Zone Asset Pack | https://balloon-cart.itch.io/elemental-zone | free personal/commercial use, modification allowed, no redistribution on their own |
| Volt bat, spark slime | Admurin - Enemy Galore I | https://admurin.itch.io/enemy-galore-1 | free for commercial use, credit appreciated |
| Storm eye | LuizMelo - Monsters Creatures Fantasy | https://luizmelo.itch.io/monsters-creatures-fantasy | CC0 |
| Storm hound | free creature pack sprite, downsampled and tinted | see `assets/lightning/CREDITS.md` | free (see page) |

Sprites were recoloured and converted to a low-resolution pixel look for the storm theme.

## Audio
- Dragon roar / hurt / death / fly / breath (`assets/lightning/sfx/*.ogg`): trimmed from
  "T.REX ROAR - Sound Design" by Pierre Mendola. **Licence not yet confirmed - check before any public release.**

## Base game (tutorial project)
- Knight player sprite, tutorial levels, movement script, tutorial boss, base sound effects and music:
  from the team's shared tutorial project. **Team to fill in:** original sources and licences for the knight sprite, music and sound effects.

## Software and libraries
| Tool | Use | Licence |
|---|---|---|
| Godot Engine 4.x | game engine | MIT |
| Arduino IDE + Espressif ESP32 core | firmware | LGPL-2.1 (core) |
| pyserial | read the controller's serial port | BSD-3-Clause |
| pynput | send keyboard key presses | LGPL-3.0 |

More detail on the Lightning art is in `assets/lightning/CREDITS.md`.
