# AeroDrive

AeroDrive is a standalone, ESP32-powered smart dashboard that uses Wi-Fi to turn live traffic and weather data into physical animations (a spinning wheel and turbine) and screen updates across four custom modes

# Reason

Now, why did I choose to build AeroDrive over all the other amazing projects? First obvi I wanted to find something that would encompass both my passions for aerospace and automotive engineering into a single physical project. Second, I wanted to take my CAD and 3D printing skills to the next level by designing a custom chassis that feels like a cross between a cockpit instrument panel and a high-performance supercar dash—something functional, sleek, and genuinely cool to have on a desk.

# The Functionality

Firstly, hardware: AeroDrive contains an ESP32 microcontroller, a high-refresh display panel, and custom physical inputs—each mapped to simulate real vehicle and flight telemetry control;

Switch 1 (Traffic Mode): Revs an engine sound on startup, then continuously spins the car wheel fast for clear roads and slow for heavy traffic.

Switch 2 (Wind Mode): Displays live weather on the screen and briefly spools up the turbine fan whenever real outdoor wind gusts occur.

Switch 3 A 25-minute Pomodoro timer that mutes all motors for quiet studying, with double-tap display dimming and a long-press demo sweep.

The ESP32 device connects to your local Wi-Fi network and establishes a secure connection to fetch live environmental or telemetry data via background HTTP requests. It parses real-time JSON payloads containing parameters like speed, acceleration vectors, temperature metrics, and system status.

Then, the processed data is pushed directly to the display controller to update the custom retro-style dials, digital needles, and live graphs on the screen. Pressing any of the physical switches triggers immediate GPIO interrupts to execute custom routines, toggle display states, or send control commands back through the system.

# The Schematics and PCB

<img width="1098" height="719" alt="Screenshot 2026-08-04 234123" src="https://github.com/user-attachments/assets/d6cb46bd-2ea5-464e-a85f-00038c227bd0" />

<img width="840" height="621" alt="image" src="https://github.com/user-attachments/assets/99a703b1-87db-4385-b812-3969ec404cd7" />

<img width="798" height="692" alt="image" src="https://github.com/user-attachments/assets/646f7753-719e-4346-80fe-113ea137b7d5" />

<img width="852" height="735" alt="image" src="https://github.com/user-attachments/assets/8cf35683-dc27-422d-bac8-335a8603eedd" />

<img width="933" height="672" alt="image" src="https://github.com/user-attachments/assets/0d766b74-6a8f-4e51-8873-4ce63e875dd5" />

# 3D Model







