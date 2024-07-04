#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <WiFi.h>
#include <esp_now.h>

// TFT pins
#define TFT_CS     5
#define TFT_RST    4
#define TFT_DC     2
#define TFT_MOSI  23
#define TFT_SCLK  18
#define TFT_MISO   19

// Button pins
#define BUTTON_NEXT    32
#define BUTTON_SELECT  33
#define BUTTON_RETURN  25
#define BUTTON_OPTION  26

// LED pins
#define LED_TX  27
#define LED_RX  14

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Colors
unsigned short light = 0x8F1F;
unsigned short dark = 0x0169;
unsigned short darkest = 0x0062;
unsigned short selected = 0x701D;
unsigned short foreground;
unsigned short background;
unsigned short txt1 = 0xDEFB;

char letters[41] = "QWERTYUIOPASDFGHJKL<ZXCVBNM1234567890., ";
String myMsg = "";
int maxLength = 40;

int chosenX = 0;
int chosenY = 0;
int chosen = 0;

bool isMy[4] = {0};
bool isMyTMP[4] = {0};

String allMsg[4] = {"Welcome!",
                    "Use buttons to type.",
                    "Press OPTION to send.",
                    " "};

String allMsgTMP[4] = {"Welcome!",
                       "Use buttons to type.",
                       "Press OPTION to send.",
                       " "};

String success;
char msgSent[41];
char msgReceived[41];
esp_now_peer_info_t peerInfo;

// Flags
bool newMessageReceived = false;
bool showInstructions = true;
bool firstMessageSentOrReceived = false;
bool showStatus = false;

// Timer for status message
unsigned long statusMsgTime = 0;
const unsigned long statusMsgDuration = 2000; // 2 seconds

// Splash screen image (example: a simple 8x8 bitmap)
const uint16_t splashScreen[8][8] = {
  {ST77XX_BLUE, ST77XX_BLUE, ST77XX_BLUE, ST77XX_BLUE, ST77XX_BLUE, ST77XX_BLUE, ST77XX_BLUE, ST77XX_BLUE},
  {ST77XX_BLUE, ST77XX_WHITE, ST77XX_WHITE, ST77XX_WHITE, ST77XX_WHITE, ST77XX_WHITE, ST77XX_WHITE, ST77XX_BLUE},
  {ST77XX_BLUE, ST77XX_WHITE, ST77XX_WHITE, ST77XX_WHITE, ST77XX_WHITE, ST77XX_WHITE, ST77XX_WHITE, ST77XX_BLUE},
  {ST77XX_BLUE, ST77XX_WHITE, ST77XX_WHITE, ST77XX_WHITE, ST77XX_WHITE, ST77XX_WHITE, ST77XX_WHITE, ST77XX_BLUE},
  {ST77XX_BLUE, ST77XX_WHITE, ST77XX_WHITE, ST77XX_WHITE, ST77XX_WHITE, ST77XX_WHITE, ST77XX_WHITE, ST77XX_BLUE},
  {ST77XX_BLUE, ST77XX_WHITE, ST77XX_WHITE, ST77XX_WHITE, ST77XX_WHITE, ST77XX_WHITE, ST77XX_WHITE, ST77XX_BLUE},
  {ST77XX_BLUE, ST77XX_WHITE, ST77XX_WHITE, ST77XX_WHITE, ST77XX_WHITE, ST77XX_WHITE, ST77XX_WHITE, ST77XX_BLUE},
  {ST77XX_BLUE, ST77XX_BLUE, ST77XX_BLUE, ST77XX_BLUE, ST77XX_BLUE, ST77XX_BLUE, ST77XX_BLUE, ST77XX_BLUE}
};

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  digitalWrite(LED_TX, LOW); // Turn off TX LED
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status == ESP_NOW_SEND_SUCCESS) {
    success = "Delivery Success :)";
  } else {
    success = "Delivery Fail :(";
  }
  showStatus = true;
  statusMsgTime = millis();
}

// Callback when data is received
void OnDataRecv(const esp_now_recv_info *recv_info, const uint8_t *incomingData, int len) {
  digitalWrite(LED_RX, HIGH); // Turn on RX LED
  memcpy(&msgReceived, incomingData, sizeof(msgReceived));
  Serial.print("Bytes received: ");
  Serial.println(msgReceived);
  sort();
  allMsg[3] = String(msgReceived);
  isMy[3] = 0;
  newMessageReceived = true; // Set the flag to true
  showInstructions = false; // Hide instructions on receiving a message
  firstMessageSentOrReceived = true; // Set the flag to indicate the first message was sent or received
  digitalWrite(LED_RX, LOW); // Turn off RX LED after processing
}

void sort() {
  for (int i = 0; i < 4; i++) {
    allMsgTMP[i] = allMsg[i];
    isMyTMP[i] = isMy[i];
  }
  for (int i = 0; i < 3; i++) {
    allMsg[i] = allMsgTMP[i + 1];
    isMy[i] = isMyTMP[i + 1];
  }
}

void sendMsg() {
  digitalWrite(LED_TX, HIGH); // Turn on TX LED
  sort();
  allMsg[3] = myMsg;
  isMy[3] = 1;
  myMsg.toCharArray(msgSent, sizeof(msgSent));
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &msgSent, sizeof(msgSent));
  myMsg = "";
  showInstructions = false; // Hide instructions on sending a message
  firstMessageSentOrReceived = true; // Set the flag to indicate the first message was sent or received
}

void displaySplashScreen() {
  tft.fillScreen(ST77XX_BLACK);
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      tft.drawPixel(x + 60, y + 40, splashScreen[y][x]);
    }
  }
  delay(2000); // Display splash screen for 2 seconds
  tft.fillScreen(ST77XX_BLACK);
}

void setup() {
  Serial.begin(115200);
  delay(2000); // Added delay for stability
  Serial.println("Starting setup...");

  // Initialize TFT
  Serial.println("Initializing TFT...");
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(2);
  displaySplashScreen();
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(0, 0);

  // Initialize buttons
  Serial.println("Initializing buttons...");
  pinMode(BUTTON_NEXT, INPUT_PULLUP);
  pinMode(BUTTON_SELECT, INPUT_PULLUP);
  pinMode(BUTTON_RETURN, INPUT_PULLUP);
  pinMode(BUTTON_OPTION, INPUT_PULLUP);
  Serial.println("Buttons initialized.");

  // Initialize LEDs
  pinMode(LED_TX, OUTPUT);
  pinMode(LED_RX, OUTPUT);
  digitalWrite(LED_TX, LOW);
  digitalWrite(LED_RX, LOW);

  // Initialize WiFi
  Serial.println("Initializing WiFi...");
  WiFi.mode(WIFI_STA);
  Serial.println("WiFi initialized.");

  // Initialize ESP-NOW
  Serial.println("Initializing ESP-NOW...");
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  Serial.println("ESP-NOW initialized.");
  delay(1000); // Added delay to ensure stable initialization
}

void draw(bool fullUpdate = true) {
  if (fullUpdate) {
    tft.fillScreen(ST77XX_BLACK);
    tft.fillRect(2, 2, 124, 50, darkest);
    tft.fillRect(4, 70, 120, 14, dark);
    tft.fillRect(2, 52, 80, 14, darkest);
  }

  if (!firstMessageSentOrReceived) {
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.setCursor(0, 0);
    tft.print("Pichu Spark");
    tft.setTextColor(txt1, darkest);
    tft.setCursor(5, 6);
    tft.print(allMsg[0]);
    tft.setCursor(5, 18);
    tft.print(allMsg[1]);
    tft.setCursor(5, 30);
    tft.print(allMsg[2]);
  } else {
    // Clear only the message area (top part of the screen)
    tft.fillRect(0, 0, 128, 48, ST77XX_BLACK);
  }

  // Update typed message area
  tft.setTextColor(txt1, dark);
  tft.fillRect(5, 73, 118, 14, dark); // Clear the typed message area
  tft.setCursor(5, 73);
  tft.print(myMsg);

  // Redraw only the changed keys
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 10; i++) {
      if ((i == chosenX && j == chosenY) || (i == chosenX && j == chosenY && fullUpdate)) {
        // Clear previous selected key
        tft.fillRect(5 + (i * 12), 88 + (j * 14), 10, 12, ST77XX_BLACK);
      }
      if (i == chosenX && j == chosenY) {
        background = selected;
        foreground = ST77XX_WHITE;
      } else {
        background = light;
        foreground = ST77XX_BLACK;
      }
      tft.setTextColor(foreground, background);
      tft.fillRect(5 + (i * 12), 88 + (j * 14), 10, 12, background);
      tft.setCursor(5 + (i * 12) + 2, 88 + (j * 14) + 2);
      tft.print(String(letters[j * 10 + i]));
    }
  }

  // Update messages area
  for (int j = 0; j < 4; j++) {
    if (fullUpdate || allMsg[j] != allMsgTMP[j]) {
      tft.fillRect(0, 6 + (j * 12), 128, 12, ST77XX_BLACK); // Clear message area line by line
      if (isMy[j] == 1) {
        tft.setTextColor(txt1, darkest);
      } else {
        tft.setTextColor(light, darkest);
      }
      tft.setCursor(5, 6 + (j * 12));
      tft.print(allMsg[j]);
    }
  }

  // Update character count
  tft.setTextColor(ST77XX_ORANGE, ST77XX_BLACK);
  tft.fillRect(94, 57, 34, 8, ST77XX_BLACK); // Clear the previous character count
  tft.setCursor(94, 57);
  tft.print(String(myMsg.length()) + "/40");

  // Redraw the static parts
  tft.drawFastHLine(8, 54, 62, 0x52AA);
  tft.setTextColor(0x39E7, darkest);
  tft.setCursor(10, 57);
  tft.print("ESP NOW");

  tft.fillCircle(126, 2, 6, ST77XX_RED);

  // Display status message if needed
  if (showStatus) {
    tft.fillRect(0, 120, 128, 20, ST77XX_BLACK); // Clear status message area
    tft.setTextColor(success == "Delivery Success :)" ? ST77XX_GREEN : ST77XX_RED, ST77XX_BLACK);
    tft.setCursor(5, 125);
    tft.print(success);
  }
}

void loop() {
  static unsigned long lastDebounceTime = 0;
  static int lastButtonStateNext = HIGH;
  static int lastButtonStateSelect = HIGH;
  static int lastButtonStateReturn = HIGH;
  static int lastButtonStateOption = HIGH;
  bool redraw = false;

  int buttonStateNext = digitalRead(BUTTON_NEXT);
  int buttonStateSelect = digitalRead(BUTTON_SELECT);
  int buttonStateReturn = digitalRead(BUTTON_RETURN);
  int buttonStateOption = digitalRead(BUTTON_OPTION);

  unsigned long currentTime = millis();

  if (buttonStateNext == LOW && lastButtonStateNext == HIGH && currentTime - lastDebounceTime > 200) {
    chosenX++;
    if (chosenX > 9) chosenX = 0;
    lastDebounceTime = currentTime;
    redraw = true;
    showInstructions = false; // Hide instructions on button press
  }

  if (buttonStateSelect == LOW && lastButtonStateSelect == HIGH && currentTime - lastDebounceTime > 200) {
    chosenY++;
    if (chosenY > 3) chosenY = 0; // Adjusted to handle 4 rows
    lastDebounceTime = currentTime;
    redraw = true;
    showInstructions = false; // Hide instructions on button press
  }

  if (buttonStateReturn == LOW && lastButtonStateReturn == HIGH && currentTime - lastDebounceTime > 200) {
    if (letters[chosenY * 10 + chosenX] == '<' && myMsg.length() > 0) {
      myMsg = myMsg.substring(0, myMsg.length() - 1);
    } else if (myMsg.length() < maxLength) {
      myMsg += letters[chosenY * 10 + chosenX];
    }
    lastDebounceTime = currentTime;
    redraw = true;
    showInstructions = false; // Hide instructions on button press
  }

  if (buttonStateOption == LOW && lastButtonStateOption == HIGH && currentTime - lastDebounceTime > 200) {
    sendMsg();
    lastDebounceTime = currentTime;
    redraw = true;
    showInstructions = false; // Hide instructions on button press
  }

  lastButtonStateNext = buttonStateNext;
  lastButtonStateSelect = buttonStateSelect;
  lastButtonStateReturn = buttonStateReturn;
  lastButtonStateOption = buttonStateOption;

  if (redraw || newMessageReceived) { // Redraw if a new message is received or status needs to be shown
    draw(false);
    newMessageReceived = false; // Reset the flag
  }

  // Check if the status message needs to be hidden
  if (showStatus && millis() - statusMsgTime > statusMsgDuration) {
    showStatus = false; // Hide status message after duration
    draw(false); // Update the display to clear the status message
  }

  delay(100); // Loop delay to prevent watchdog reset
}
