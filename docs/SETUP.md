# Setup and run instructions (Report Appendix E)

Game: Group 1 - Pinnacle Project platformer (Godot). Works with the keyboard alone,
or with the ESP32 breadboard controller.

## A. Run the game (keyboard, no hardware)
1. Install **Godot 4.6 or newer** (tested with 4.7) from https://godotengine.org/download
2. Download/clone this repository. The Godot project folder is the one containing `project.godot`.
3. Open Godot -> Import -> select `project.godot` -> Open. Wait for the first import to finish.
4. Press **F5** (Play). Main scene: `scenes/game.tscn`.

| Action | Keyboard | Controller button |
|---|---|---|
| Move left / right | A / D (or arrows) | LEFT / RIGHT |
| Jump | Space / W | JUMP |
| Shoot (auto-aim) | J (E or left mouse also work) | SHOOT |
| Ability 1 / 2 / 3 / 4 | K / L / X / S (Q F R C also work) | AB1 / AB2 / AB3 / AB4 |
| Pause | Esc | - |
| Cheat / level-skip menu | F1 | - |

## B. Run with the ESP32 controller
**Hardware:** ESP32 Dev Module + USB-C cable, 8 push buttons, breadboard, jumper wires.
Each button connects one GPIO pin to GND (the firmware uses the internal pull-ups, so no
resistors are needed). Pin map: see the top of `firmware/esp32_controller/esp32_controller.ino`.

1. Install the **Arduino IDE**, then add ESP32 support (Boards Manager -> "esp32" by Espressif).
2. Open `firmware/esp32_controller/esp32_controller.ino`.
3. Tools -> Board -> **ESP32 Dev Module**; Tools -> Port -> your COM port (e.g. COM3). Click Upload.
4. Open Serial Monitor at **115200 baud**: you should see `HELLO` every 2 s and, when a button is
   pressed, lines such as `JUMP:1` then `JUMP:0`. **Close the Serial Monitor before step 6** (only one program can hold the port).
5. Install Python 3 and the bridge libraries: `pip install -r tools/requirements.txt`
6. Start the bridge: `python tools/serial_bridge.py` (or `python tools/serial_bridge.py COM3`).
7. Click into the running game window and use the buttons. The bridge turns each message into the keyboard key in the table above.

**If the controller is unplugged** the bridge releases every key (nothing gets stuck) and keeps
retrying until the board is back. Contact bounce is filtered in firmware (15 ms debounce).

## C. Troubleshooting
- *Nothing happens in game:* the game window must be focused; check the bridge prints "Connected on ...".
- *Port busy / access denied:* close the Arduino Serial Monitor.
- *Port not found:* install the USB-serial driver for your ESP32 board (CP210x or CH340) and re-plug.
- *Game opens with errors on first import:* close Godot, reopen the project and let the import finish.

## D. Fresh lab machine checklist
- [ ] Godot installed and project opens
- [ ] Game starts with F5 and keyboard works
- [ ] (Controller) Python + `pip install -r tools/requirements.txt` done
- [ ] (Controller) Correct COM port; Serial Monitor closed
