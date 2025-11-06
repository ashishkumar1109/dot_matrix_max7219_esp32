# DotMatrix LED Display Library for ESP32 + MAX7219

A lightweight, easy-to-use **C++ library** for driving **cascaded 8x8 LED matrix displays** using the **MAX7219 driver** on **ESP32** via hardware SPI.

Perfect for scrolling text, static messages, and custom animations on 1–4 chained 8x8 dot matrix modules.

---

## Features

- **Smooth left-to-right text scrolling**
- **Static centered text display**
- **Hardware SPI** (fast & reliable)
- **Supports 1 to 4 cascaded modules**
- **5x7 uppercase font** (A–Z, 0–9, symbols)
- **Adjustable scroll speed & brightness**
- **Simple API** – just a few function calls
- **No external dependencies** beyond Arduino core

---

## Hardware Requirements

| Component         | Details                             |
|-------------------|-------------------------------------|
| ESP32 Dev Board   | Any (e.g., DevKit, NodeMCU)         |
| MAX7219 Module(s) | 8x8 LED matrix with MAX7219         |
| Connections       | 5-wire SPI (VCC, GND, DIN, CLK, CS) |

---

## Wiring (ESP32 to MAX7219)

| MAX7219 Pin   | ESP32 Pin  | Description      |
|---------------|------------|------------------|
| VCC           | 3.3V or 5V | Power            |
| GND           | GND        | Ground           |
| DIN (Data In) | GPIO 23    | MOSI             |
| CLK           | GPIO 18    | SCK              |
| CS (Load)     | GPIO 5     | SS / Chip Select |

> You can change pins in code if needed.

---

## Usage Example

```cpp
#include "dot_matrix.h"

//DATA, CLK, CS, Module
DotMatrix matrix ( 23, 18, 5, 4 );

void setup ( ) {
    matrix.begin ( );
    matrix.setIntensity ( 8 );
}

void loop ( ) {
    matrix.scrollText ( "HELLO #ESP32     ", 70 );
}
```

---

## API Reference

### Constructor
```cpp
DotMatrix matrix ( dataPin, clkPin, csPin, numDevices );
```
- `numDevices`: 1 to 4 (default: 4)

### Methods

| Function                       | Description                 |
|--------------------------------|-----------------------------|
| `begin ( )`                    | Initializes SPI and MAX7219 |
| `clear ( )`                    | Clears display              |
| `setIntensity ( level )`       | 0–15 brightness             |
| `scrollText ( "text", delay )` | Scrolls text with spacing   |
| `displayStatic ( "text" )`     | Shows centered static text  |

---

## Font

- **5x7 pixels** per character
- **Uppercase A–Z**, **0–9**, and common symbols
- LSB = top row in font data

---

## File Structure

```
DotMatrix/
├── dot_matrix.h
├── dot_matrix.cpp
└── examples/
    └── ScrollDemo/
        └── ScrollDemo.ino
```

---

## Example: Static Text

```cpp
matrix.displayStatic ( "ESP32" );
delay ( 2000 );
matrix.displayStatic ( "READY" );
```

---

## Tips

- Add extra spaces in `scrollText ( )` to control gap between loops.
- Use `setIntensity ( )` to adjust visibility in different lighting.
- Chain up to **4 modules** (32x8 pixels total).

---

## License

**MIT License** – Free to use, modify, and distribute.

---

## Author

Built with **ESP32** and **MAX7219** in mind.  
Clean, efficient, and beginner-friendly.

---

> **Happy Scrolling!**  
> *Now make your LED matrix talk!*
