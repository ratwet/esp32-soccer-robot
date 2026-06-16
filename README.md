<h1 align="center">🤖 ESP32 WiFi Soccer Robot</h1>

<p align="center">
  <img src="https://img.shields.io/badge/ESP32-E7352C?style=for-the-badge&logo=espressif&logoColor=white" alt="ESP32">
  <img src="https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" alt="C++">
  <img src="https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=arduino&logoColor=white" alt="Arduino">
  <img src="https://img.shields.io/badge/HTML5-E34F26?style=for-the-badge&logo=html5&logoColor=white" alt="HTML5">
  <img src="https://img.shields.io/badge/JavaScript-F7DF1E?style=for-the-badge&logo=javascript&logoColor=black" alt="JavaScript">
</p>

> A competitive 4-wheel differential-drive soccer robot controlled over WiFi via a custom browser-based gamepad. Built for the **CUJ Robotics Competition**, this robot requires no app installation—just connect to its WiFi hotspot and open your browser!

---

## 📖 Overview

This pusher-style soccer robot is engineered for competition. It features a differential drive system (tank-style) driven wirelessly through a web interface that works on any smartphone or laptop. The controller is optimized for high-performance driving, featuring true multi-touch support, live tuning controls, a split-screen landscape layout, and a responsive gamepad feel.

## ✨ Features

- 🎮 **Browser-Based Gamepad:** No app needed. Accessible via any web browser.
- 📱 **Mobile Optimized:** Full-screen landscape mode with orientation lock and portrait-mode block.
- 🤌 **True Multi-Touch:** Press Forward + Left/Right simultaneously for fluid movement.
- 🏎️ **Smooth Arc Turns:** Inner wheel is slowed (not reversed) for precise cornering.
- 🔀 **Diagonal Movement:** Supports FL, FR, BL, and BR arc combinations.
- ⚙️ **Live Tuning:**
  - **3 Speed Modes:** High (100%), Medium (73%), Low (45%).
  - **Turn Speed Control:** Adjustable inner wheel speed (4% – 60%).
  - **Direction Balance Trim:** Fixes motor drift live (-20 to +20).
- ⚡ **Ultra-Low Latency:** 240MHz CPU with WiFi power-save disabled.
- 🔒 **Button Lock Prevention:** Reliable pointer tracking via Pointer-ID maps.

---

## 🛠️ Hardware Components

- **Microcontroller:** ESP32 Dev Module
- **Motor Drivers:** 2 × L298N H-Bridge
- **Motors:** 4 × BO Gear Motors (paired per side)
- **Wheels:** 4 × Standard Robot Wheels
- **Power Delivery:** 12V Battery → L298N → 5V onboard regulator → ESP32

---

## 🔌 Circuit & Wiring Guide

### Power Routing
> ⚠️ **IMPORTANT:** All GND pins (ESP32, L298N #1, L298N #2, Battery) **must** be connected together. Make sure the 12V–5V jumper is **ON** on both L298N boards.

| Component | Terminal / Pin | Connection |
| :--- | :--- | :--- |
| **12V Battery** | `(+) Positive` | L298N #1 `VIN` **&** L298N #2 `VIN` |
| **12V Battery** | `(-) Negative` | L298N #1 `GND`, L298N #2 `GND`, **&** ESP32 `GND` |
| **L298N #1** | `5V OUT` | ESP32 `5V / VIN` |

### Signal Wiring (ESP32 to L298N)

| ESP32 GPIO | L298N Pin | Module | Function |
| :--- | :--- | :--- | :--- |
| `GPIO 14` | `ENA` | L298N #1 | Left Motors PWM (Speed) |
| `GPIO 26` | `IN1` | L298N #1 | Left Motors Direction A |
| `GPIO 27` | `IN2` | L298N #1 | Left Motors Direction B |
| `GPIO 32` | `ENB` | L298N #2 | Right Motors PWM (Speed) |
| `GPIO 25` | `IN3` | L298N #2 | Right Motors Direction A |
| `GPIO 33` | `IN4` | L298N #2 | Right Motors Direction B |

### Motor Wiring (Parallel Configuration)
Wire the front and rear motors on each side together in parallel.
- **L298N `OUT1` / `OUT3`:** Front Motor (+) and Rear Motor (+)
- **L298N `OUT2` / `OUT4`:** Front Motor (-) and Rear Motor (-)
*(If motors spin in the wrong direction, simply swap the two wires on that motor at the L298N output terminals—no code change needed).*

---

## 🕹️ Movement Commands

| Command | Action | Behavior |
| :---: | :--- | :--- |
| **F** | Forward | Both motors forward, full speed |
| **B** | Backward | Both motors backward, full speed |
| **L** | Left Turn | Both motors forward, left inner slowed (smooth arc) |
| **R** | Right Turn | Both motors forward, right inner slowed (smooth arc) |
| **FL / FR** | Diagonal Forward | Arc forward-left / forward-right |
| **BL / BR** | Diagonal Backward | Arc backward-left / backward-right |
| **S** | Stop | Full stop |

---

## 🚀 Getting Started

### 1. Arduino IDE Setup
1. Open Arduino IDE → `File` → `Preferences`.
2. Paste the following into **Additional Boards Manager URLs**:
   `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
3. Go to `Tools` → `Board` → `Boards Manager`.
4. Search for **esp32** and install the package by *Espressif Systems*.
5. Select `Tools` → `Board` → `ESP32 Arduino` → **ESP32 Dev Module**.

### 2. Upload the Code
1. Clone this repository or copy `robot.ino` into the Arduino IDE.
2. Connect your ESP32 via USB.
3. Select your COM port under `Tools` → `Port`.
4. Click **Upload**.
5. Open the Serial Monitor at `115200` baud to confirm a successful boot.

### 3. Connect & Drive
1. Power on the robot.
2. Connect your smartphone or laptop to the robot's WiFi network:
   - **SSID:** `CUJ`
   - **Password:** `142482830`
3. Open your web browser and navigate to: `http://192.168.4.1`
4. Tap **TAP TO START** to enter fullscreen mode.
5. Start driving!

---

## ⚙️ Live Tuning & Customization

### Tuning via the Web Controller
*   **TURN Knob (Turn Speed):** Controls how much the inner wheel slows during turns. 
    *   *4–15%: Very tight sharp turn*
    *   *16–30%: Balanced smooth arc (Recommended)*
    *   *40–60%: Very wide gradual arc*
*   **BAL Knob (Direction Balance):** Corrects sideways drift during straight movement.
    *   *Negative value (-): Fixes leftward drift.*
    *   *Positive value (+): Fixes rightward drift.*
    *   *(Start at 0, press `-` or `+` one step at a time and test until straight).*

### Customizing the Code
You can easily change your credentials and pin mappings at the top of the `robot.ino` sketch:

```cpp
// 1. Change WiFi Credentials (Password must be 8+ chars)
const char* ssid     = "CUJ";
const char* password = "142482830";

// 2. Change Motor GPIO Pins
#define ENA 14    // Left motor PWM
#define IN1 26    // Left motor direction A
#define IN2 27    // Left motor direction B
#define ENB 32    // Right motor PWM
#define IN3 25    // Right motor direction A
#define IN4 33    // Right motor direction B
