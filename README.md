<h1 align="center">🤖 ESP32 WiFi Soccer Robot</h1>

<p align="center">
  <em>A competitive 4-wheel differential-drive soccer robot controlled entirely over WiFi via a browser-based gamepad. Built for the CUJ Robotics Competition.</em>[cite: 1]
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Hardware-ESP32-E7352C?style=for-the-badge&logo=espressif&logoColor=white" alt="ESP32">
  <img src="https://img.shields.io/badge/Language-C++-%2300599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" alt="C++">
  <img src="https://img.shields.io/badge/Frontend-HTML/CSS/JS-E34F26?style=for-the-badge&logo=html5&logoColor=white" alt="HTML/CSS/JS">
</p>

---

## 📖 Overview

This is a pusher-style, differential-drive (tank-style) soccer robot engineered specifically for robot soccer competitions.[cite: 1] It is driven wirelessly through a custom web interface that runs natively on any phone or laptop browser—**no app installation required**. 

The web controller is designed to mimic a real hardware gamepad, featuring a landscape full-screen mode, split-screen layout, live tuning controls, and true multi-touch support for complex maneuvering.

---

## 🛠️ Hardware & Components

*   **Microcontroller:** ESP32 Dev Module (running at 240 MHz for minimum latency)[cite: 1]
*   **Motor Driver:** 2× L298N H-Bridge[cite: 1]
*   **Motors:** 4× BO Gear Motors (paired per side)
*   **Wheels:** 4× Standard Robot Wheels
*   **Power Supply:** 12V Battery → L298N → 5V onboard regulator → ESP32

### 🔌 Circuit Wiring & Pinout

**Power Distribution**
> ⚠️ **CRITICAL:** All `GND` pins (ESP32, L298N #1, L298N #2, and Battery) MUST be connected together to ensure a common ground. Make sure the 12V–5V jumper is **ON** on both L298N boards.

| Source | Destination |
| :--- | :--- |
| **12V Battery (+)** | L298N #1 `VIN` & L298N #2 `VIN` |
| **12V Battery (-)** | L298N #1 `GND`, L298N #2 `GND`, & ESP32 `GND` |
| **L298N #1 (5V OUT)** | ESP32 `5V/VIN` |

**Motor Driver Data Connections**

| ESP32 GPIO | L298N #1 (Left Motors) | ESP32 GPIO | L298N #2 (Right Motors) |
| :---: | :--- | :---: | :--- |
| `GPIO 14` (PWM) | `ENA` | `GPIO 32` (PWM) | `ENB` |
| `GPIO 26` | `IN1` | `GPIO 25` | `IN3` |
| `GPIO 27` | `IN2` | `GPIO 33` | `IN4` |

**Motor Wiring (4-Motor Parallel)**
*   **L298N OUT1:** Front Motor (+) and Rear Motor (+)
*   **L298N OUT2:** Front Motor (−) and Rear Motor (−)

---

## 💻 Software Features

*   🌐 **Browser-Based Gamepad:** Works instantly over WiFi.
*   📱 **Responsive UI:** Full-screen landscape mode with orientation lock and portrait mode blocking.
*   👆 **True Multi-Touch:** Press Forward + Left/Right simultaneously. Prevents button locks via pointer-id tracking map.
*   🔄 **Smooth Arc Turns:** Inner wheels are slowed down rather than reversed for fluid turning. Includes diagonal arcs (FL, FR, BL, BR).
*   ⚙️ **Live Tuning:** Adjust Turn Speed and Direction Balance on-the-fly directly from the UI.
*   ⚡ **Low Latency:** ESP32 CPU set to 240MHz with WiFi power-save disabled for instantaneous response times.

### 🕹️ Movement Commands

| Command | Action | Behavior Details |
| :---: | :--- | :--- |
| **F** | Forward | Both motors forward, full speed. |
| **B** | Backward | Both motors backward, full speed. |
| **L** | Left Turn | Both motors forward, left inner slowed *(smooth arc left)*. |
| **R** | Right Turn | Both motors forward, right inner slowed *(smooth arc right)*. |
| **FL/FR** | Diagonal Forward | Arc forward-left / Arc forward-right. |
| **BL/BR** | Diagonal Backward | Arc backward-left / Arc backward-right. |
| **S** | Stop | Full stop. |

---

## 🚀 Getting Started

### 1. Arduino IDE Setup
1. Open Arduino IDE and navigate to **File → Preferences**.
2. Paste the following into *Additional Boards Manager URLs*:
```text
   [https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json](https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json)
