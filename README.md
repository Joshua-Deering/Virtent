## Vertent (*Virt*ual-Det*ent*): <br><small>A BLDC-actuated haptic interface for high-resolution system control</small>
A high-resolution, haptic-feedback scroll peripheral built on the ESP32 platform. Unlike mechanical scroll wheels, this device uses a Brushless DC (BLDC) motor to generate tactile "virtual detents," allowing for dynamic resistance, infinitely variable scrolling resolutions, and haptic alerts.

### Key Features
**Virtual Haptic Detents**: Utilizes a BLDC motor for software-defined tactile feedback, allowing for "notched" or "smooth" scrolling modes.

**High-Resolution Input**: Fine-grained rotational tracking far exceeding standard mechanical encoders.

**Dual-Mode Connectivity**: Supports both USB-C and Bluetooth Low Energy (BLE) for versatile workstation integration.

**Power Efficient**: Custom power management logic designed to maximize runtime on a portable Li-ion cell.

**Custom Integrated Electronics**: 100% custom PCB designed for the ESP32-WROOM-1 form factor.

### Tech Stack
**Microcontroller**: ESP32-WROOM-1 (Dual-core for real-time haptic processing).

**PCB Design**: KiCad (Multi-layer design, ground-up schematic, and routing).

**Firmware**: C++ / ESP-IDF.

**Mechanical**: 3D Printed PLA housing.

### Engineering Challenges & Implementation
**Real-Time Haptics**
The core of this project is the low-latency feedback loop required to simulate physical detents. The system must process motor position and adjust magnetic resistance in real-time to ensure the "clicks" feel crisp and responsive to the user's touch.

**Power Optimization**
As an aspiring systems engineer, I prioritized energy profiling. The firmware implements deep-sleep states and optimized BLE advertising intervals to ensure the high-draw motor doesn't compromise portability.

### Project Status: In-Progress
This project is currently in the active prototyping phase.

#### Current Focus:

**Hardware**: Finalizing preparation for reflow soldering of PCB and SMD components.

**Mechanical**: Iterating on the 3D-printed enclosure to better accommodate battery constraints and ergonomic mounting.

**Firmware**: Energy use profiling and optimization.

### Planned Roadmap:

[ ] Complete reflow-soldered board.

[ ] Implement software-defined "profiles" (e.g., high-resistance for volume, low-resistance for long documents).

[ ] Finalize enclosure, and print with Resin for better surface finish

### Development Gallery
