# PichuSpark

## Overview

PichuSpark is an ESP32 peer-to-peer text messaging system that uses a generic ST7735 TFT display and ESP-NOW for communication. This project is an adaptation designed to work with the Adafruit library for the ST7735 TFT displays.

## Features
- **Peer-to-Peer Communication:** Send and receive text messages between two ESP32 devices.
- **TFT Display:** Utilize a generic ST7735 TFT display to show messages and interface.
- **ESP-NOW Protocol:** Wireless communication without the need for a Wi-Fi network.
- **Vibration Notification:** Added motor to buzz when a new message is received. (issue)

## Hardware Requirements
- ESP32 x2
- ST7735 TFT Display x2
- Buttons (Next, Select, Return, Option) x2 sets
- Motor for vibration
- Additional LEDs for TX and RX indicators

## Wiring Diagram

### ESP32 Connections
| TFT Pin  | ESP32 Pin |
|----------|-----------|
| TFT_CS   | 5         |
| TFT_RST  | 4         |
| TFT_DC   | 2         |
| TFT_MOSI | 23        |
| TFT_SCLK | 18        |
| TFT_MISO | 19        |

### Button Connections
| Button        | ESP32 Pin |
|---------------|-----------|
| BUTTON_NEXT   | 32        |
| BUTTON_SELECT | 33        |
| BUTTON_RETURN | 25        |
| BUTTON_OPTION | 26        |

### LED Connections
| LED    | ESP32 Pin |
|--------|-----------|
| LED_TX | 27        |
| LED_RX | 14        |

### Motor Connections
| Motor  | ESP32 Pin |
|--------|-----------|
| Motor  | 13        |

## Installation

### Clone the Repository
```sh
git clone https://github.com/TalonSec-coder/PichuSpark.git
cd PichuSpark
Install Required Libraries
Adafruit GFX Library
Adafruit ST7735 and ST7789 Library
Upload the Code
Open the project in the Arduino IDE.
Select the appropriate ESP32 board from Tools > Board.
Upload the code to both ESP32 devices.
Usage
Power Up the Devices
Connect and power up both ESP32 devices.

Sending Messages
Use the buttons to navigate and type messages.
Press the option button to send the message.
The motor will buzz on the receiving device when a new message is received.
Original Credits
VolosR/Messenger
M5Stack AtomS3 Dev Kit
ESP32 SMS Encrypted Messenger!!!
Encryption Overview
The project uses a simple XOR cipher for encryption. While XOR encryption is easy to implement and can obscure data to some extent, it is not secure against more sophisticated attacks. XOR encryption can be broken easily if the key or part of the key is known or guessed.

For a more secure encryption solution, consider using an established cryptographic library that provides stronger algorithms. Here are a few suggestions:

AES (Advanced Encryption Standard): A widely used encryption standard that is secure and efficient. The ESP32 has hardware support for AES encryption.
ChaCha20: A stream cipher designed to be fast and secure, particularly suitable for embedded systems.
ESP32 Arduino Crypto Library: This library provides various cryptographic functions, including AES, RSA, SHA, and more.
Features
Encrypted Messaging: Messages are encrypted using a basic XOR cipher before transmission.
ESP-NOW Communication: Utilizes ESP-NOW for efficient peer-to-peer communication.
User Interface: The display shows messages, typing area, and a virtual keyboard for message composition.
Visual Indicators: LEDs indicate message transmission and reception.
How to Use
Setup: Connect the ESP32 to the TFT display and buttons as per the pin configuration.
Upload Code: Use the Arduino IDE to upload the code to your ESP32.
Send and Receive Messages: Use the buttons to compose and send messages. Received messages will be displayed on the screen.
Future Enhancements
Improve Encryption: Upgrade the encryption method to AES or ChaCha20 for better security.
License
This project is licensed under the MIT License - see the LICENSE file for details.
