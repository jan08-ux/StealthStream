# StealthStream: Ascon-Based Encryption for IoT Edge Systems

![StealthStream Header](assets/header.png)

[![NIST LWC](https://img.shields.io/badge/NIST-LWC_Winner-blueviolet?style=for-the-badge)](https://csrc.nist.gov/projects/lightweight-cryptography)
[![Platform: ESP32](https://img.shields.io/badge/Platform-ESP32-E7352C?style=for-the-badge&logo=espressif)](https://www.espressif.com/en/products/socs/esp32)
[![Algorithm: Ascon-128](https://img.shields.io/badge/Algorithm-Ascon--128-00599C?style=for-the-badge&logo=cplusplus)](https://ascon.iaik.tugraz.at/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg?style=for-the-badge)](https://opensource.org/licenses/MIT)

**StealthStream** is a high-performance, ultra-lightweight security framework designed for resource-constrained IoT devices. By leveraging the **ASCON-128** algorithm—the winner of the NIST Lightweight Cryptography competition—StealthStream provides robust **Authenticated Encryption with Associated Data (AEAD)** for telemetry pipelines over MQTT.

---

## Key Features

- **NIST Standard Security**: Uses the finalized NIST LWC standard (ASCON) optimized for microcontrollers.
- **End-to-End Encryption**: Secure data flow from ESP32 edge nodes to Python-based monitoring clusters.
- **AEAD Protection**: Ensures both **Confidentiality** (encryption) and **Integrity** (authentication tag).
- **MQTT Integration**: Seamlessly operates with standard MQTT brokers (e.g., HiveMQ).
- **Stateful Communication**: Implements nonce-synchronization to prevent replay attacks.

---


## 📂 Project Structure

- `lightweight_ascon.ino`: Core ESP32 sketch handling WiFi, MQTT, and encryption loops.
- `aead.c / ascon.h`: C-implementation of the ASCON AEAD core.
- `rece/recevier.py`: Python-based subscriber that decrypts incoming hexadecimal streams.
- `rece/ascon.py`: Python implementation of ASCON for cross-platform compatibility.

---

## 🛠️ Getting Started

### 1. Hardware Requirements
- ESP32 Development Board (e.g., ESP32-S3, DevKitV1).
- Stable WiFi connection.

### 2. Microcontroller Setup (ESP32)
1. Open `lightweight_ascon.ino` in the Arduino IDE.
2. Update your WiFi credentials:
   ```cpp
   const char* ssid = "YOUR_SSID";
   const char* password = "YOUR_PASSWORD";
   ```
3. Ensure the `key` and `nonce` match your receiver's configuration.
4. Upload to your device.

### 3. Receiver Setup (Python)
1. Install dependencies:
   ```bash
   pip install paho-mqtt
   ```
2. Run the receiver:
   ```bash
   python rece/recevier.py
   ```

---

## 🔒 Security Specifications

| Specification | Value |
| :--- | :--- |
| **Algorithm** | Ascon-128 (v1.2) |
| **Key Size** | 128-bit |
| **Nonce Size** | 128-bit |
| **Tag Size** | 128-bit |
| **Mode** | AEAD (Authenticated Encryption) |

> [!CAUTION]
> **Key Management**: The current implementation uses a hardcoded key for demonstration. For production environments, use a secure vault or hardware security module (HSM) to manage your 128-bit keys.

---

## 📜 License
This project is licensed under the MIT License - see the LICENSE file for details.
