"""
Serial bridge: ESP32 controller -> keyboard keys for the Godot game.

Godot has no built-in serial port, so this small program reads the lines the
ESP32 sends ("JUMP:1", "JUMP:0", "HELLO") and presses/releases the matching
keyboard key. The game already maps these keys to its actions, so no game
change is needed (see project.godot: A/D move, Space jump, J shoot, K/L/X/S abilities).

Usage:   python serial_bridge.py            (auto-detects the ESP32 port)
         python serial_bridge.py COM3       (give the port yourself)
Install: pip install pyserial pynput
Failure handling: if the cable is unplugged, every key is released at once and
the bridge keeps retrying until the board is back.
"""
import sys
import time

import serial                      # pyserial
from serial.tools import list_ports
from pynput.keyboard import Controller, Key

BAUD = 115200
TIMEOUT_S = 5.0                    # no line (not even HELLO) for this long = lost link

KEYMAP = {                         # message name -> keyboard key (matches Godot input map)
    "LEFT": "a", "RIGHT": "d", "JUMP": Key.space, "SHOOT": "j",
    "AB1": "k", "AB2": "l", "AB3": "x", "AB4": "s",
}

kb = Controller()
held = set()


def parse(line):
    """'JUMP:1' -> ('JUMP', True). Returns None for HELLO or noise."""
    if ":" not in line:
        return None
    name, _, val = line.partition(":")
    if name not in KEYMAP or val not in ("0", "1"):
        return None                # ignore corrupted / unknown lines
    return name, val == "1"


def set_key(name, pressed):
    key = KEYMAP[name]
    if pressed and name not in held:
        kb.press(key)
        held.add(name)
    elif not pressed and name in held:
        kb.release(key)
        held.discard(name)


def release_all():
    for name in list(held):
        set_key(name, False)


def find_port():
    for p in list_ports.comports():
        text = (p.description or "") + (p.manufacturer or "")
        if any(w in text for w in ("CP210", "CH340", "USB Serial", "Silicon Labs", "UART")):
            return p.device
    ports = list_ports.comports()
    return ports[0].device if ports else None


def run(port):
    with serial.Serial(port, BAUD, timeout=0.5) as ser:
        print(f"Connected on {port}. Press Ctrl+C to stop.")
        last_seen = time.time()
        while True:
            raw = ser.readline().decode("ascii", errors="ignore").strip()
            if raw:
                last_seen = time.time()
                msg = parse(raw)
                if msg:
                    set_key(*msg)
            elif time.time() - last_seen > TIMEOUT_S:
                release_all()
                raise serial.SerialException("no data from controller")


def main():
    port = sys.argv[1] if len(sys.argv) > 1 else None
    try:
        while True:
            try:
                run(port or find_port() or "")
            except (serial.SerialException, OSError) as e:
                release_all()      # never leave a key stuck down
                print(f"Controller lost ({e}); retrying in 2 s...")
                time.sleep(2)
    except KeyboardInterrupt:
        release_all()
        print("Stopped.")


if __name__ == "__main__":
    main()
