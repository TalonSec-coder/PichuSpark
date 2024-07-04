PichuSpark

Overview

PichuSpark is an ESP32 peer-to-peer text messaging system that uses a generic ST7735 TFT display and ESP-NOW for communication. 
This project is an adaptation designed to work with the Adafruit library for the ST7735 TFT displays.


Features
Peer-to-Peer Communication: Send and receive text messages between two ESP32 devices.
TFT Display: Utilize a generic ST7735 TFT display to show messages and interface.
ESP-NOW Protocol: Wireless communication without the need for a Wi-Fi network.
Vibration Notification: Added motor to buzz when a new message is received. (issue)


Hardware Requirements
ESP32 x2
ST7735 TFT Display x2
Buttons (Next, Select, Return, Option) x2 sets
Motor for vibration
Additional LEDs for TX and RX indicators



Wiring Diagram


ESP32 Connections
TFT Pin	ESP32 Pin

TFT_CS	5

TFT_RST	4

TFT_DC	2

TFT_MOSI	23

TFT_SCLK	18

TFT_MISO	19


Button	ESP32 Pin

BUTTON_NEXT	32

BUTTON_SELECT	33

BUTTON_RETURN	25

BUTTON_OPTION	26


LED	ESP32 Pin

LED_TX	27

LED_RX	14


Motor	ESP32 Pin

Motor	13


Installation

Clone the Repository

sh

Copy code

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

VolosR/Messenger / https://github.com/VolosR/Messenger/tree/main

M5Stack AtomS3 Dev Kit




License

This project is licensed under the MIT License - see the LICENSE file for details.

