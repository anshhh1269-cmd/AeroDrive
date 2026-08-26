#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include <HardwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include "time.h"

// --- WIFI & API CONFIGURATION ---
const char* ssid       = "someone";
const char* password   = "rivaanretard2";

const char* ntpServer  = "pool.ntp.org";
const long  gmtOffset_sec = 25200; // GMT+7 (Bangkok)
const int   daylightOffset_sec = 0;

// Google Maps Distance Matrix API Configuration
const String GOOGLE_API_KEY = "YOUR_GOOGLE_MAPS_API_KEY";
const String ORIGIN         = "13.7563,100.5018"; // Start Coords (Lat,Lon)

// Saved Destinations Array for Touch Cycling
const String DESTINATIONS[3] = {"13.6900,100.7501", "13.7466,100.5349", "13.8000,100.5500"};
const String DEST_NAMES[3]   = {"AIRPORT", "CENTER", "NORTH HQ"};
int currentDestIdx = 0;

// Open-Meteo Weather Coordinates
const String LATITUDE  = "13.7563";
const String LONGITUDE = "100.5018";

// --- PIN DEFINITIONS (SCHEMATIC MATCH) ---
#define PIN_SW_TRA    32
#define PIN_SW_WI     33
#define PIN_SW_FOCUS  39

#define PIN_FAN       13
#define PIN_TFT_BL    12
#define TOUCH_CS      35 // T_CS on U8 header

#define IN1 14
#define IN2 27
#define IN3 26
#define IN4 25

// --- HARDWARE OBJECTS ---
TFT_eSPI tft = TFT_eSPI();
HardwareSerial dfSerial(1);
DFRobotDFPlayerMini dfplayer;

// --- STATE MANAGEMENT ---
enum OperatingMode { MODE_DEFAULT_CLOCK, MODE_TRAFFIC, MODE_WIND, MODE_POMODORO };
OperatingMode currentMode = MODE_DEFAULT_CLOCK;

// Stepper Motor Sequence (Half-Step)
const int stepPins[4] = {IN1, IN2, IN3, IN4};
const int stepMatrix[8][4] = {
  {1,0,0,0}, {1,1,0,0}, {0,1,0,0}, {0,1,1,0},
  {0,0,1,0}, {0,0,1,1}, {0,0,0,1}, {1,0,0,1}
};
int currentStep = 0;

// System Variables
bool isDimmed = false;
unsigned long pomodoroStart = 0;
bool isPomodoroPaused = false;
unsigned long pomodoroPauseOffset = 0;
const unsigned long POMODORO_DURATION = 25 * 60 * 1000;

unsigned long lastPressFocus = 0;
int tapCountFocus = 0;
unsigned long lastTimeUpdate = 0;
unsigned long lastTouchTime = 0;

// Theme Colors for Default Clock Touch Cycling
uint16_t clockThemeColors[4] = {TFT_CYAN, TFT_YELLOW, TFT_RED, TFT_GREEN};
int themeIdx = 0;

// Unit Switcher for Wind Mode (0: km/h, 1: m/s, 2: knots)
int windUnitMode = 0; 
unsigned long manualFanBoostEnd = 0; // Manual Fan Touch Timer

// Live API Data & Timers
int liveTravelTimeMin = 25;
int liveNormalTimeMin = 25;
float liveWindSpeedKmh = 0.0;
float liveWindGustKmh  = 0.0;

unsigned long lastTrafficFetch = 0;
unsigned long lastWindFetch    = 0;
const unsigned long API_INTERVAL = 300000; // 5-minute background refresh

void setup() {
  Serial.begin(115200);

  // Pin Modes
  pinMode(PIN_SW_TRA, INPUT_PULLUP);
  pinMode(PIN_SW_WI, INPUT_PULLUP);
  pinMode(PIN_SW_FOCUS, INPUT_PULLUP);
  
  pinMode(PIN_FAN, OUTPUT);
  digitalWrite(PIN_FAN, LOW);

  pinMode(PIN_TFT_BL, OUTPUT);
  digitalWrite(PIN_TFT_BL, HIGH);

  for(int i = 0; i < 4; i++) pinMode(stepPins[i], OUTPUT);

  // Initialize Display
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  // Wi-Fi Connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED && millis() < 10000) {
    delay(250);
  }

  // Time Sync
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // Initialize Audio Module
  dfSerial.begin(9600, SERIAL_8N1, 22, 21);
  if (dfplayer.begin(dfSerial)) {
    dfplayer.volume(20);
  }

  // Initial Data Fetch
  fetchLiveTrafficData();
  fetchLiveWindData();

  tft.fillScreen(TFT_BLACK);
}

void loop() {
  handleSwitchInputs();
  handleTouchInputs();

  // Background API Refresh
  if (millis() - lastTrafficFetch > API_INTERVAL) fetchLiveTrafficData();
  if (millis() - lastWindFetch > API_INTERVAL)    fetchLiveWindData();

  // Mode Handlers
  switch(currentMode) {
    case MODE_DEFAULT_CLOCK:
      updateDefaultClockUI();
      break;
    case MODE_TRAFFIC:
      runTrafficMode();
      break;
    case MODE_WIND:
      runWindMode();
      break;
    case MODE_POMODORO:
      runPomodoroMode();
      break;
  }
}

// --- TOUCHSCREEN CONTROL ENGINE ---
void handleTouchInputs() {
  uint16_t x = 0, y = 0;

  // Read Touch Controller (Threshold pressure ~600)
  if (tft.getTouch(&x, &y, 600)) {
    if (millis() - lastTouchTime < 350) return; // Debounce
    lastTouchTime = millis();

    // Universal Top Banner Touch: Tap to force API update or toggle modes
    if (y < 40) {
      if (currentMode == MODE_TRAFFIC) fetchLiveTrafficData();
      else if (currentMode == MODE_WIND) fetchLiveWindData();
      else {
        currentMode = static_cast<OperatingMode>((currentMode + 1) % 4);
        tft.fillScreen(TFT_BLACK);
      }
      return;
    }

    // DEFAULT MODE TOUCH: Tap clock to cycle themes
    if (currentMode == MODE_DEFAULT_CLOCK) {
      themeIdx = (themeIdx + 1) % 4;
      tft.fillScreen(TFT_BLACK);
    }
    
    // TRAFFIC MODE TOUCH: Tap destination label to switch route target
    else if (currentMode == MODE_TRAFFIC) {
      if (y > 150) {
        currentDestIdx = (currentDestIdx + 1) % 3;
        fetchLiveTrafficData(); // Immediate re-fetch for new coordinates
        tft.fillScreen(TFT_BLACK);
      }
    }

    // WIND MODE TOUCH: Tap center screen to activate 5s turbine boost / change units
    else if (currentMode == MODE_WIND) {
      if (y > 60 && y < 140) {
        windUnitMode = (windUnitMode + 1) % 3; // Toggle km/h -> m/s -> knots
      } else if (y >= 140) {
        manualFanBoostEnd = millis() + 5000; // 5-second manual turbine boost
      }
    }

    // POMODORO MODE TOUCH: Tap center timer to Pause/Resume
    else if (currentMode == MODE_POMODORO) {
      if (y > 60 && y < 180) {
        isPomodoroPaused = !isPomodoroPaused;
      }
    }
  }
}

// --- API FETCHERS ---
void fetchLiveTrafficData() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://maps.googleapis.com/maps/api/distancematrix/json?origins=" + ORIGIN + 
                 "&destinations=" + DESTINATIONS[currentDestIdx] + 
                 "&departure_time=now&key=" + GOOGLE_API_KEY;

    http.begin(url);
    if (http.GET() == HTTP_CODE_OK) {
      String payload = http.getString();
      DynamicJsonDocument doc(2048);
      deserializeJson(doc, payload);

      int trafficSec = doc["rows"][0]["elements"][0]["duration_in_traffic"]["value"];
      int normalSec  = doc["rows"][0]["elements"][0]["duration"]["value"];

      if (trafficSec > 0) liveTravelTimeMin = trafficSec / 60;
      if (normalSec > 0)  liveNormalTimeMin = normalSec / 60;
    }
    http.end();
  }
  lastTrafficFetch = millis();
}

void fetchLiveWindData() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://api.open-meteo.com/v1/forecast?latitude=" + LATITUDE + 
                 "&longitude=" + LONGITUDE + 
                 "&current=wind_speed_10m,wind_gusts_10m";

    http.begin(url);
    if (http.GET() == HTTP_CODE_OK) {
      String payload = http.getString();
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);

      liveWindSpeedKmh = doc["current"]["wind_speed_10m"];
      liveWindGustKmh  = doc["current"]["wind_gusts_10m"];
    }
    http.end();
  }
  lastWindFetch = millis();
}

// --- SWITCH INTERRUPT & GESTURE LOGIC ---
void handleSwitchInputs() {
  if (digitalRead(PIN_SW_TRA) == LOW) {
    if (currentMode != MODE_TRAFFIC) {
      currentMode = MODE_TRAFFIC;
      initTrafficMode();
    }
    delay(200);
  } 
  else if (digitalRead(PIN_SW_WI) == LOW) {
    if (currentMode != MODE_WIND) {
      currentMode = MODE_WIND;
      initWindMode();
    }
    delay(200);
  }
  else if (digitalRead(PIN_SW_FOCUS) == LOW) {
    unsigned long pressTime = millis();
    while(digitalRead(PIN_SW_FOCUS) == LOW) {
      if (millis() - pressTime > 1500) {
        runDemoSweep();
        return;
      }
    }
    
    // Double Tap Backlight Dimming
    tapCountFocus++;
    if (tapCountFocus == 1) lastPressFocus = millis();
    if (tapCountFocus == 2 && (millis() - lastPressFocus < 400)) {
      isDimmed = !isDimmed;
      analogWrite(PIN_TFT_BL, isDimmed ? 30 : 255);
      tapCountFocus = 0;
    }

    if (currentMode != MODE_POMODORO) {
      currentMode = MODE_POMODORO;
      initPomodoroMode();
    }
  }

  if (millis() - lastPressFocus > 400) tapCountFocus = 0;
}

// --- LIVE TIME & DATE RENDERER ---
void drawTimeHeader(bool fullScreenClock) {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)) return;

  char timeStr[10];
  char dateStr[20];
  strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &timeinfo);
  strftime(dateStr, sizeof(dateStr), "%a, %b %d %Y", &timeinfo);

  if (fullScreenClock) {
    tft.setTextColor(clockThemeColors[themeIdx], TFT_BLACK);
    tft.drawString(timeStr, 20, 60, 7);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString(dateStr, 40, 150, 4);
    tft.drawString("SYSTEM READY // AERODRIVE", 30, 200, 2);
  } else {
    tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
    tft.drawString(String(timeStr) + " | " + String(dateStr), 10, 5, 2);
  }
}

// --- MODE 0: DEFAULT CLOCK ---
void updateDefaultClockUI() {
  if (millis() - lastTimeUpdate > 500) {
    drawTimeHeader(true);
    lastTimeUpdate = millis();
  }
  digitalWrite(PIN_FAN, LOW);
  stopMotor();
}

// --- MODE 1: TRAFFIC MODE ---
void initTrafficMode() {
  tft.fillScreen(TFT_BLACK);
  dfplayer.play(1);
  delay(1200);
}

void runTrafficMode() {
  drawTimeHeader(false);
  
  uint16_t statusColor = TFT_GREEN;
  int motorSpeedDelay = 3;

  if (liveTravelTimeMin > liveNormalTimeMin + 15) {
    statusColor = TFT_RED;
    motorSpeedDelay = 12;
  } else if (liveTravelTimeMin > liveNormalTimeMin + 5) {
    statusColor = TFT_YELLOW;
    motorSpeedDelay = 7;
  }

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("LIVE COMMUTE ETA", 20, 30, 4);
  tft.setTextColor(statusColor, TFT_BLACK);
  tft.drawString(String(liveTravelTimeMin) + " MINS", 20, 70, 7);
  
  tft.drawSmoothArc(240, 160, 50, 40, 45, 315, statusColor, TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("TARGET: " + DEST_NAMES[currentDestIdx] + " [TAP]", 20, 160, 2);

  stepMotor(motorSpeedDelay);
}

// --- MODE 2: WIND MODE ---
void initWindMode() {
  tft.fillScreen(TFT_BLACK);
}

void runWindMode() {
  drawTimeHeader(false);

  bool isGusting = liveWindGustKmh > 15.0 || liveWindSpeedKmh > 15.0;
  bool isManualBoost = millis() < manualFanBoostEnd;

  // Unit Conversions
  float displayVal = liveWindSpeedKmh;
  String unitStr = " km/h";
  if (windUnitMode == 1) { displayVal *= 0.277778; unitStr = " m/s"; }
  else if (windUnitMode == 2) { displayVal *= 0.539957; unitStr = " knots"; }

  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.drawString("WIND TELEMETRY", 20, 30, 4);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("SPEED: " + String(displayVal, 1) + unitStr, 20, 80, 4);

  if (isGusting || isManualBoost) {
    tft.setTextColor(TFT_BLUE, TFT_BLACK);
    tft.drawString(isManualBoost ? "[ MANUAL BOOST ]" : "[ TURBINE SPOOLING ]", 20, 140, 4);
    digitalWrite(PIN_FAN, HIGH);
  } else {
    digitalWrite(PIN_FAN, LOW);
  }
}

// --- MODE 3: POMODORO FOCUS MODE ---
void initPomodoroMode() {
  pomodoroStart = millis();
  isPomodoroPaused = false;
  tft.fillScreen(TFT_BLACK);
  digitalWrite(PIN_FAN, LOW);
  stopMotor();
  dfplayer.stop();
}

void runPomodoroMode() {
  drawTimeHeader(false);

  unsigned long elapsed = isPomodoroPaused ? pomodoroPauseOffset : (millis() - pomodoroStart);
  if (elapsed > POMODORO_DURATION) elapsed = POMODORO_DURATION;

  unsigned long remaining = (POMODORO_DURATION - elapsed) / 1000;
  int mins = remaining / 60;
  int secs = remaining % 60;

  char timeBuffer[10];
  sprintf(timeBuffer, "%02d:%02d", mins, secs);

  tft.setTextColor(isPomodoroPaused ? TFT_YELLOW : TFT_ORANGE, TFT_BLACK);
  tft.drawString(isPomodoroPaused ? "PAUSED [TAP TO RESUME]" : "FOCUS TIMER [TAP PAUSE]", 20, 30, 4);
  tft.drawString(timeBuffer, 20, 80, 8);
}

// --- DEMO SWEEP ---
void runDemoSweep() {
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.drawString("SYSTEM SWEEP DEMO", 20, 100, 4);

  digitalWrite(PIN_FAN, HIGH);
  for(int i = 0; i < 400; i++) stepMotor(2);
  digitalWrite(PIN_FAN, LOW);
  
  tft.fillScreen(TFT_BLACK);
}

// --- STEPPER MOTOR DRIVER ---
void stepMotor(int speedDelayMs) {
  for (int pin = 0; pin < 4; pin++) {
    digitalWrite(stepPins[pin], stepMatrix[currentStep][pin]);
  }
  currentStep = (currentStep + 1) % 8;
  delay(speedDelayMs);
}

void stopMotor() {
  for (int pin = 0; pin < 4; pin++) digitalWrite(stepPins[pin], LOW);
}