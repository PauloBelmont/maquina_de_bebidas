# Contributing Guide

Thank you for your interest in contributing to the **Coffee Machine (ESP32 + Nextion)** project! This repository contains the artifacts developed for an embedded systems project, combining firmware, HMI (Nextion), and documentation.

Follow the steps below to contribute efficiently and safely—especially when changes affect real hardware.

## What’s in this repository?

- `maquina_cafe_tela/`
  - **Firmware**: ESP32 source code (sensors, pumps, relays, control logic).
  - **HMI**: Nextion project files used to build the touchscreen interface.
- `docs/`
  - Project documentation (requirements, diagrams, manuals, reports).

## Requirements

Before contributing, make sure you have:

- **Software**
  - Arduino IDE: https://www.arduino.cc/en/software
  - Nextion Editor (LTS): https://nextion.tech/download/nextion-setup-vLTS.zip
  - Git: https://git-scm.com/
- **Hardware (recommended for real testing)**
  - ESP32 Devkit V1, Nextion display, sensors and actuators listed in the README.

## How to contribute

1. **Fork the repository** (GitHub UI).
2. **Clone your fork**
   ```bash
   git clone https://github.com/PauloBelmont/maquina_de_bebidas.git
   cd maquina_de_bebidas
   ```
3. **Create a new branch**
   Use a descriptive name and prefix:
   ```bash
   git checkout -b firmware/fix-sensor-reading
   # or
   git checkout -b hmi/improve-main-screen
   # or
   git checkout -b docs/update-pinout
   ```
4. **Make your changes**
   - Keep firmware and HMI changes organized.
   - If you change wiring/pins, update documentation immediately to avoid hardware damage.
5. **Commit your changes**
   Write clear commit messages:
   ```bash
   git add .
   git commit -m "Fix relay activation logic to prevent pump from running indefinitely"
   ```
6. **Push your branch**
   ```bash
   git push origin firmware/fix-sensor-reading
   ```
7. **Open a Pull Request**
   - Describe what changed and why.
   - Include how you tested (simulation vs real hardware).
   - If you changed the Nextion UI, add screenshots/photos.

## Best practices (embedded + hardware)

- **Pin safety:** Do not change ESP32 pins without checking conflicts (boot pins, input-only pins, etc.).
- **Fail-safe behavior:** Make sure pumps/relays cannot stay ON due to loops, crashes, or missing state transitions.
- **Input validation:** Validate data coming from the Nextion serial interface before triggering actions.
- **Repository hygiene:** Do not commit generated/temporary files or IDE-specific folders.

## Good contribution ideas

- Improve the firmware state machine (dispense flow, error states).
- Calibrate HC-SR04 detection logic for more reliable cup sensing.
- Improve Nextion UI/UX (feedback screens, progress states, error prompts).
- Complete and maintain pinout documentation in `docs/` and README.

## Questions / Support

Please open an issue for bugs, questions, or suggestions:
https://github.com/PauloBelmont/maquina_de_bebidas/issues
