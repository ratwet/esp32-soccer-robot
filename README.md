# 🤖 ESP32 WiFi Soccer Robot

<p align="center">
  <img src="https://img.shields.io/badge/ESP32-E7352C?style=for-the-badge&logo=espressif&logoColor=white">
  <img src="https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=arduino&logoColor=white">
  <img src="https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white">
  <img src="https://img.shields.io/badge/WiFi-Controlled-success?style=for-the-badge">
  <img src="https://img.shields.io/badge/License-MIT-green?style=for-the-badge">
</p>

<p align="center">
  <strong>A high-performance ESP32-powered WiFi Soccer Robot with a browser-based controller, multi-touch support, smooth differential steering, and real-time tuning controls.</strong>
</p>

---

## 📖 Overview

This project is a competitive **4-wheel differential-drive soccer robot** built around the ESP32 microcontroller. The robot creates its own WiFi hotspot and hosts a responsive browser-based controller, allowing users to drive it from any smartphone, tablet, or laptop without installing additional applications.

Designed for robotics competitions, the system prioritizes:

* Low-latency control
* Reliable WiFi communication
* Smooth steering behavior
* Mobile-friendly operation
* Real-time driving adjustments

Simply power on the robot, connect to its WiFi network, open a browser, and start driving.

---

## ✨ Features

### 🎮 Browser-Based Controller

* No Android app required
* No iOS app required
* Works directly from any web browser
* Full-screen responsive interface

### 📱 Mobile Optimized

* Landscape-first controller layout
* Full-screen gameplay mode
* Touch-friendly controls
* Fast response on mobile devices

### 🤌 True Multi-Touch Support

Supports simultaneous button presses:

* Forward + Left
* Forward + Right
* Backward + Left
* Backward + Right

Allowing realistic diagonal and arc movements.

### 🏎️ Smooth Differential Steering

Instead of reversing one side while turning:

* Outer wheel remains at full speed
* Inner wheel slows down proportionally

Benefits:

* Better ball control
* Reduced wheel slip
* Smoother turning radius
* More predictable movement

### ⚙️ Real-Time Tuning

Adjust settings directly from the controller:

* Speed modes
* Steering sensitivity
* Motor balance compensation

No firmware upload required.

### ⚡ Competition Ready

* ESP32 running at 240MHz
* WiFi power-saving disabled
* Optimized command handling
* Low-latency wireless control

---

## 🏗️ System Architecture

```text
Smartphone / Laptop
          │
          ▼
Browser-Based Controller
          │
          ▼
      WiFi Network
          │
          ▼
        ESP32
          │
          ▼
   Dual L298N Drivers
          │
          ▼
      4 DC Motors
          │
          ▼
      Robot Drive
```

---

## 🛠️ Hardware Components

| Component          | Quantity  |
| ------------------ | --------- |
| ESP32 Dev Module   | 1         |
| L298N Motor Driver | 2         |
| BO Gear Motors     | 4         |
| Robot Wheels       | 4         |
| Robot Chassis      | 1         |
| 12V Battery        | 1         |
| Jumper Wires       | As Needed |

---

## 🔌 Circuit Connections

### ESP32 → Motor Drivers

| ESP32 GPIO | L298N Pin | Function          |
| ---------- | --------- | ----------------- |
| GPIO 14    | ENA       | Left Motor PWM    |
| GPIO 26    | IN1       | Left Direction A  |
| GPIO 27    | IN2       | Left Direction B  |
| GPIO 32    | ENB       | Right Motor PWM   |
| GPIO 25    | IN3       | Right Direction A |
| GPIO 33    | IN4       | Right Direction B |

---

### Power Connections

| Source          | Destination   |
| --------------- | ------------- |
| Battery (+)     | L298N #1 VIN  |
| Battery (+)     | L298N #2 VIN  |
| Battery (-)     | Common Ground |
| L298N 5V Output | ESP32 VIN     |

> ⚠️ Important: All grounds (ESP32, battery, and motor drivers) must be connected together.

---

## ⚙️ Motor Configuration

The robot uses a differential drive configuration.

### Left Side

* Front Left Motor
* Rear Left Motor

Connected in parallel.

### Right Side

* Front Right Motor
* Rear Right Motor

Connected in parallel.

If any motor rotates in the wrong direction, swap its two motor wires.

---

## 🕹️ Movement Commands

| Command | Action         |
| ------- | -------------- |
| F       | Forward        |
| B       | Backward       |
| L       | Left Arc       |
| R       | Right Arc      |
| FL      | Forward Left   |
| FR      | Forward Right  |
| BL      | Backward Left  |
| BR      | Backward Right |
| S       | Stop           |

---

## 🚀 Installation

### 1. Install Arduino IDE

Download:

https://www.arduino.cc/en/software

---

### 2. Install ESP32 Board Package

Open:

File → Preferences

Add the following URL:

```text
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

Open:

```text
Tools → Board Manager
```

Search:

```text
ESP32
```

Install:

```text
Espressif Systems ESP32 Package
```

---

### 3. Clone Repository

```bash
git clone https://github.com/yourusername/esp32-wifi-soccer-robot.git
```

---

### 4. Open Project

Open:

```text
code.ino
```

in Arduino IDE.

---

### 5. Select Board

```text
Tools → Board → ESP32 Dev Module
```

---

### 6. Upload Firmware

1. Connect ESP32 via USB
2. Select correct COM Port
3. Click Upload

---

## 📡 Connecting to the Robot

After powering the robot:

### WiFi Credentials

```text
SSID: CUJ
Password: 142482830
```

Connect your device to the WiFi network.

Open:

```text
http://192.168.4.1
```

Press:

```text
TAP TO START
```

and begin driving.

---

## ⚙️ Live Tuning Controls

### Speed Modes

| Mode   | Output |
| ------ | ------ |
| High   | 100%   |
| Medium | 73%    |
| Low    | 45%    |

---

### Turn Control

Adjusts how much the inner wheel slows during turns.

| Value  | Behavior          |
| ------ | ----------------- |
| 4–15%  | Very Sharp Turn   |
| 16–30% | Balanced Steering |
| 40–60% | Wide Arc Turn     |

Recommended:

```text
16% – 30%
```

---

### Balance Adjustment

Used to correct drift.

| Value    | Effect              |
| -------- | ------------------- |
| Negative | Correct Left Drift  |
| Positive | Correct Right Drift |

---

## 🔧 Customization

### Change WiFi Credentials

```cpp
const char* ssid = "YourSSID";
const char* password = "YourPassword";
```

---

### Change Motor Pins

```cpp
#define ENA 14
#define IN1 26
#define IN2 27

#define ENB 32
#define IN3 25
#define IN4 33
```

---

## 📂 Repository Structure

```text
ESP32-WiFi-Soccer-Robot/
│
├── code.ino
├── README.md
└── LICENSE

```

---

## 📈 Performance Features

This project includes several optimizations:

* ESP32 CPU at 240MHz
* WiFi sleep disabled
* Efficient motor command processing
* Responsive browser interface
* Multi-touch pointer tracking
* Smooth differential steering

These improvements reduce input lag and improve handling during matches.

---

## 🔍 Troubleshooting

### Cannot Connect to WiFi

* Verify robot is powered
* Restart ESP32
* Check SSID and password
* Confirm antenna is unobstructed

---

### Web Controller Not Opening

Verify:

```text
http://192.168.4.1
```

and ensure you are connected to the robot's WiFi network.

---

### Motors Spin Backwards

Swap motor polarity on the corresponding motor.

No code changes required.

---

### Robot Pulls to One Side

Use the Balance Adjustment control until the robot drives straight.

---

## 🛡️ Safety Notes

* Disconnect power before modifying wiring.
* Avoid short circuits.
* Use appropriate battery protection.
* Verify polarity before powering on.
* Keep hands clear of moving wheels.

---

## 🚀 Future Improvements

Potential upgrades:

* ESP32-CAM integration
* FPV video streaming
* Autonomous ball tracking
* Line detection
* OLED status display
* PID motor control
* OTA firmware updates
* Wireless gamepad support
* Telemetry dashboard

---

## 🤝 Contributing

Contributions are welcome.

1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Submit a pull request

---

## 📜 License

This project is licensed under the MIT License.

See the `LICENSE` file for more information.

---

## ⭐ Support

If you found this project useful:

* ⭐ Star the repository
* 🍴 Fork the project
* 📢 Share it with the robotics community

Happy Building! 🤖⚽
