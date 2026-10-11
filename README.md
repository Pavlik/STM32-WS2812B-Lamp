# STM32 WS2812B Smart Lamp

A custom battery-powered RGB lamp based on the **STM32G030K6T6** microcontroller and WS2812B addressable LEDs.

This project covers the complete development process, including embedded firmware, custom PCB design in Altium Designer, and a 3D-printed enclosure designed in Autodesk Inventor.

The repository contains the source files needed to study, modify, and reproduce the device.

## Overview

The lamp provides multiple lighting modes, button-based control, rechargeable battery operation, and low-power standby functionality.

The firmware was developed in STM32CubeIDE. The initial project configuration was generated using STM32CubeMX, while the remaining peripheral configuration and application logic were implemented manually.

## Features

- STM32G030K6T6 microcontroller
- WS2812B individually addressable RGB LEDs
- Multiple lighting modes, including static color and animated effects
- Button-based user interface with short and long press detection
- Timer and DMA-based LED data transmission
- Rechargeable Li-ion battery power supply
- Battery charging and power management
- Low-power standby mode
- Persistent settings stored in internal Flash using EEPROM emulation
- Custom schematic and PCB designed in Altium Designer
- Custom 3D-printed enclosure designed in Autodesk Inventor

## Hardware

The hardware was designed specifically for this project.

| Component | Purpose |
|---|---|
| STM32G030K6T6 | Main microcontroller |
| WS2812B | Addressable RGB lighting |
| BQ24070 | Li-ion battery charging and power-path management |
| Li-ion battery | Portable power source |
| Boost converter | Provides the LED supply voltage |
| Custom PCB | Integrates the electronics |

The exact component values, connections, and design files are provided in the `Hardware/` directory.

## Firmware Architecture

The firmware is organized into separate modules to keep application logic, LED effects, user input, and power management independent.

| Module | Responsibility |
|---|---|
| `engine` | Main application logic and coordination |
| `myPixel` | LED-related operations |
| `pixelEffects` | Lighting effects and animations |
| `HSV_to_RGB` | HSV-to-RGB color conversion |
| `my_button` | Button input handling |
| `powerLogic` | Power management logic |
| `bq24070` | Charging-related functionality |
| `adcMeasure` | ADC measurements |
| `eeprom` | Persistent settings using Flash-based EEPROM emulation |

### LED Control

LED data is transmitted using a hardware timer and DMA.

This approach allows the timer to generate the timing-sensitive signal required by WS2812B LEDs while DMA transfers data to the timer peripheral, reducing the need for CPU-driven bit-by-bit output.

### Persistent Settings

The firmware uses the STM32 internal Flash memory to emulate EEPROM functionality.

This allows selected settings, such as the saved color, to persist across power cycles without a separate EEPROM chip.

Flash erase granularity, write alignment, and endurance must be considered when implementing persistent storage.

### Power Management

The firmware includes a low-power standby mode and logic for managing the device's power states.

The power-management implementation is designed around the lamp's battery-powered operation and user interaction.

## Hardware Design

The hardware was developed in Altium Designer.

The `Hardware/` directory contains the editable design files and relevant manufacturing exports.

Where available, the repository includes:

- Schematic source files
- PCB source files
- Component libraries and related design resources
- Gerber and NC Drill files
- Bill of Materials (BOM)
- Schematic and PCB documentation

## Enclosure Design

The enclosure was designed in **Autodesk Inventor** and manufactured using 3D printing.

The `Enclosure/` directory contains editable CAD files and, where available, exported models for 3D printing.

## Repository Structure

```text
STM32-WS2812B-Lamp/
├── README.md
├── Firmware/
│   ├── Core/
│   ├── Drivers/
│   ├── Neo_lamp.ioc
│   └── ...
├── Hardware/
│   ├── Schematic/
│   ├── PCB/
│   ├── Libraries/
│   └── Manufacturing/
├── Enclosure/
│   ├── CAD/
│   └── 3D-Print/
├── docs/
│   └── images/
├── LICENSE
└── LICENSE-HARDWARE
```

The actual directory structure may vary depending on the original project files.

## Building the Firmware

**Requirements**

- STM32CubeIDE
- STM32CubeG0 firmware package and dependencies required by the project
- ST-LINK or another compatible programming/debugging interface

**Instructions**

1. Clone or download this repository.
2. Open STM32CubeIDE.
3. Import the `Firmware/` project into the workspace.
4. Check the build configuration and required dependencies.
5. Build the project.
6. Connect the target board through the SWD interface and flash the firmware.

The included `.ioc` file documents the initial STM32CubeMX configuration. Since some peripheral configuration was implemented manually, regenerating the project from this file may not reproduce the complete firmware configuration.

## Reproducing the Device

To reproduce the lamp, use the schematic, PCB design files, BOM, enclosure models, and firmware provided in this repository.

Verify component values, battery compatibility, power-supply requirements, and board revisions before manufacturing or assembling the device.

## Project Status

The project is a completed hardware and firmware development project.

Documentation, manufacturing exports, and additional reproduction instructions may be refined over time.

## License

See `LICENSE` for firmware licensing terms and `LICENSE-HARDWARE` for hardware design licensing terms.

Third-party components, libraries, and tools remain subject to their respective licenses.
