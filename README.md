ESP32 WiFi Soccer Robot

A competitive 4-wheel soccer robot controlled over WiFi via a browser-based game pad built with ESP32, dual L298N motor drivers, and BO gear motors. No app install required just connect to WiFi and open a browser.
Built for the CUJ Robotics Competition by our team.


Overview

This robot is a differential drive pusher-style soccer robot designed for robot soccer competitions. It is driven wireless through a custom game pad web interface that works on any phone or laptop browser. The controller is designed to feel like a real game pad, landscape full screen mode, split-screen layout, live tuning controls, and true multi-touch support.

Hardware

Component Details :-
    • Micro-controller - ESP32 Dev Module 
    • Motor Driver - 2× L298N H-Bridge 
    • Motors - 4× BO Gear Motors (paired per side)
    • Wheels - 4× Standard Robot Wheels
    • Power - 12V Battery → L298N → 5V onboard reg → ESP32 
    • Drive Type - Differential drive (tank style) 

Circuit Wiring


12V Battery (+)                             L298N #1 VIN
                                            L298N #2 VIN

12V Battery (-)                               L298N #1 GND
                                              L298N #2 GND
                                              ESP32 GND

L298N #1 5V OUT                       ESP32 5V/VIN
Make sure the 12V–5V jumper is ON on both L298N boards.

L298N #1                                     Left Motors (both wired in parallel)
L298N Pin                                    ESP32 GPIO

ENA                                          GPIO 14 (PWM) 
IN1                                          GPIO 26 
IN2                                          GPIO 27 
OUT1 & OUT2                                  Left Front + Rear Motors (parallel) 




L298N #2                                      Right Motors (both wired in parallel)
L298N Pin                                     ESP32 GPIO 

 ENB                                          GPIO 32 (PWM) 
 IN3                                          GPIO 25 
 IN4                                          GPIO 33 
 OUT1 & OUT2                                  Right Front + Rear Motors (parallel) 

4-Motor Parallel Wiring
L298N OUT1                                    Front Motor (+) and Rear Motor (+)
L298N OUT2                                    Front Motor (−) and Rear Motor (−)
⚠️ All GND pins (ESP32, L298N #1, L298N #2, Battery) must be connected together.

Software

Features :-
    • Browser-based WiFi game pad, no app needed
    • Full screen landscape mode with orientation lock
    • True multi-touch press Forward + Left/Right simultaneously
    • Smooth arc turns inner wheel slowed, not reversed
    • Diagonal movement:- FL, FR, BL, BR arc combos
    • 3 speed modes, High / Medium / Low
    • Live Turn Speed adjustment (4 – 60%, steps of 2)
    • Live Direction Balance trim (−20 to +20, steps of 2), fixes motor drift
    • Button lock prevention pointer tracking via pointer-id Map
    • Portrait mode block with rotate reminder
    • 240MHz CPU + WiFi power save disabled for minimum latency
      
Movement Commands :-
Command                                  Behavior

F                                        Both motors forward, full speed 
B                                        Both motors backward, full speed
L                                        Both motors forward, left inner slowed (smooth arc left) 
R                                        Both motors forward, right inner slowed (smooth arc right) 
FL                                       Arc forward-left (diagonal) 
FR                                       Arc forward-right (diagonal) 
BL                                       Arc backward-left (diagonal) 
BR                                       Arc backward-right (diagonal) 
S                                        Full stop  





Getting Started

1. Install Arduino IDE Setup 
2. Add ESP32 Board Support
	1. Open Arduino IDE --> File --> Preferences
	2. Paste in Additional Boards Manager URLs:
	https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
3. Go to Tools --> Board --> Boards Manager
4. Search esp32 --> Install esp32 by Espressif Systems
5. Select Tools --> Board -- > ESP32 Arduino --> ESP32 Dev Module

Upload the Code
1. Clone this repo or copy robot.ino into Arduino IDE
2. Connect ESP32 via USB
3. Select your port under Tools --> Port 
4. Click Upload
5. Open Serial Monitor at 115200 baud to confirm boot

Connect & Drive

1. Power on the robot
2. Connect your phone or laptop to WiFi: CUJ / password: 142482830 (You can change it)
3. Open browser → http://192.168.4.1
4. Tap TAP TO START to enter fullscreen landscape mode
5. Drive!

---

Live Tuning (from the controller)

Speed :-
    • High  255 (100%) 
    • Medium  185 (73%)
    • Low  115 (45%) 

Turn Speed (TURN knob)
Controls how much the inner wheel slows during a left or right turn.
	Low % (4–15%) = very tight sharp turn
	Medium % (16–30%) = balanced smooth arc (recommended)
	High % (40–60%) = very wide gradual arc

Direction Balance (BAL knob)
Corrects motor speed mismatch that causes the robot to drift sideways during straight movement.
	Negative value = slows left motors (fixes leftward drift)
	Positive value = slows right motors (fixes rightward drift)
	Start at 0, press − one step at a time, drive forward, repeat until straight

Always calibrate balance on the actual competition floor different surfaces change drift behavior.


Customisation

Change WiFi name and password
At the very top of the code:
	const char* ssid     = "CUJ";
	const char* password = "142482830";
Password must be at least 8 characters or the hotspot will not start.

Change motor GPIO pins

#define ENA 14    // Left motor PWM
#define IN1 26    // Left motor direction A
#define IN2 27    // Left motor direction B
#define ENB 32    // Right motor PWM
#define IN3 25    // Right motor direction A
#define IN4 33    // Right motor direction B


If motors spin the wrong direction
Swap the two wires on that motor at the L298N output terminals no code change needed.


Competition Tips

Test on the actual competition floor and re tune balance there
Use Medium speed for precise ball control, switch to High when pushing opponents
Set Turn to around 15–20% for the best balance of agility and control
If the robot drifts left, press BAL  two clicks at a time until straight
All 4 wheels must fully contact the floor, rock the robot to check for wobble


Project Structure


esp32-soccer-robot/
├── robot.ino          # Main Arduino sketch (ESP32 code + HTML controller)
└── README.md          # This file


Built With

[Arduino ESP32 Core](https://github.com/espressif/arduino-esp32) — Espressif v3.x
ESP32 Web-server library (built-in)
Vanilla HTML / CSS / JavaScript — no external dependencies
Pointer Events API — for true multi-touch game pad support


