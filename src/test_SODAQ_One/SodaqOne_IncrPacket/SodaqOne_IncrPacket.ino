/*
   Test program for the SODAQ One
   Sends a packet at regular interval with an incrementing counter inside and the GPS position

*/

#include "Arduino.h"
#include <Sodaq_UBlox_GPS.h>

#if defined(ARDUINO_AVR_SODAQ_MBILI)
#define debugSerial Serial
#define loraSerial Serial1

#elif defined(ARDUINO_SODAQ_AUTONOMO) || defined(ARDUINO_SODAQ_ONE) || defined(ARDUINO_SODAQ_ONE_BETA)
#define debugSerial SerialUSB
#define loraSerial Serial1

#elif defined(ARDUINO_SODAQ_EXPLORER)
#define debugSerial SerialUSB
#define loraSerial Serial2

#else
// please select a sodaq board
#endif

/* Application Configuration */
uint32_t cnt = 0;
unsigned long last_time = 0;
unsigned long last_fix = 0;
bool found_fix = false;

/* Perform GPS fix */
#define GPS_ENABLE_FIX 0

/* Timeout on GPS fix */
#define GPS_FIX_TIMEOUT (900 * 1000)

/* Timeout on GPS fix during main loop */
#define GPS_FIX_TIMEOUT_LOOP (4 * 1000)

/* Time between two GPS fix */
#define GPS_DELAY (10 * 1000)

/* Time between two packets in ms */
#define PKT_DELAY (15 * 1000)

/* Time to wait between two commands */
#define CMD_DELAY 15

/* Spreading Factor */
/* From sf7 to sf12 */
#define SPREADING_FACTOR "sf7"

/* Power Output */
/*
  Pwr dBm mA
  ----------------
  -3 -4.0 17.3
  -2 -2.9 18.0
  -1 -1.9 18.7
  0 -1.7 20.2
  1 -0.6 21.2
  2 0.4 22.3
  3 1.4 23.5
  4 2.5 24.7
  5 3.6 26.1
  6 4.7 27.5
  7 5.8 28.8
  8 6.9 30.0
  9 8.1 31.2
  10 9.3 32.4
  11 10.4 33.7
  12 11.6 35.1
  13 12.5 36.5
  14 13.5 38.0
  15 14.1 38.9 */
#define POWER_OUTPUT 1

/* while (loraSerial.available()) {
    char inChar = loraSerial.read();
    if (inChar != '\n') {
      ret += inChar;
    }
  }*/

String read_response() {
  String ret; 
  char inChar;
  do {
    if (loraSerial.available()) {
      inChar = loraSerial.read();
      if (inChar != '\n') {
        ret += inChar;
      }      
    }
  } while(inChar != '\n');
  return ret;
}

String radio_tx(String cmd) {
  set_cmd(cmd);
  return read_response();
}

String set_cmd(String cmd) {
  loraSerial.println(cmd);  
  return read_response();
}

String get_cmd(String cmd) {
  loraSerial.println(cmd);  
  return read_response();
}

void print_configuration() {
  debugSerial.println("RN2483 LoRa Configuration");
  debugSerial.println("-------------------------------------");
  debugSerial.println("Modulation: " + get_cmd("radio get mod"));
  debugSerial.println("Frequency: " + get_cmd("radio get freq"));
  debugSerial.println("Power: " + get_cmd("radio get pwr"));
  debugSerial.println("Spreading Factor: " + get_cmd("radio get sf"));
  debugSerial.println("Automatic Freq Correction: " + get_cmd("radio get afcbw"));
  debugSerial.println("Receive Bandwidth: " + get_cmd("radio get rxbw"));
  debugSerial.println("Frequency Deviation: " + get_cmd("radio get fdev"));
  debugSerial.println("Preambule Length: " + get_cmd("radio get prlen"));
  debugSerial.println("Use CRC: " + get_cmd("radio get crc"));
  debugSerial.println("IQ Inversion: " + get_cmd("radio get iqi"));
  debugSerial.println("Coding Rate: " + get_cmd("radio get cr"));
  debugSerial.println("Watchdog Timeout: " + get_cmd("radio get wdt"));
  debugSerial.println("Bandwidth: " + get_cmd("radio get bw"));
  debugSerial.println("-------------------------------------");
}

void setup() {
  // Enable LoRa module
#if defined(ARDUINO_SODAQ_AUTONOMO)
  pinMode(BEE_VCC, OUTPUT);
  digitalWrite(BEE_VCC, HIGH); //set input power BEE high
#elif defined(ARDUINO_AVR_SODAQ_MBILI)
  pinMode(20, OUTPUT);
  digitalWrite(20, HIGH); //set input power BEE high
#endif

  // Hard reset the RN module
#if defined(LORA_RESET)
  pinMode(LORA_RESET, OUTPUT);
  digitalWrite(LORA_RESET, LOW);
  delay(100);
  digitalWrite(LORA_RESET, HIGH);
  delay(100);
#endif

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  while ((!debugSerial) && (millis() < 10000)) {
    // wait 10 seconds for serial monitor
  }

  debugSerial.begin(57600);
  loraSerial.begin(57600);

  debugSerial.println("Configuring LoRa Radio");
  String cmd;
  /* Stop MAC layer */
  cmd = String("mac pause");
  debugSerial.println("Stop MAC Layer: " + cmd);
  debugSerial.println(set_cmd(cmd));

  /* Configure LoRa Spreading Factor */
  cmd = String("radio set sf ");
  cmd.concat(SPREADING_FACTOR);
  debugSerial.println("LoRa Spreading Factor: " + cmd);
  debugSerial.println(set_cmd(cmd));  

  /* Configure Power Output */
  cmd = String("radio set pwr ");
  cmd.concat(POWER_OUTPUT);
  debugSerial.println("Power Output: " + cmd);
  debugSerial.println(set_cmd(cmd));

  print_configuration();

  /* Setup GPS */
  sodaq_gps.init(GPS_ENABLE);

  /* Try to find GPS fix */
  found_fix = find_fix(GPS_FIX_TIMEOUT);
  
  debugSerial.println("Ready to receive commands!");
  last_time = millis() - PKT_DELAY;
  last_fix = millis() - GPS_DELAY;
}

bool find_fix(int timeout_ms) {
  bool fix;
  if (GPS_ENABLE_FIX) {
    debugSerial.println("GPS fix starting...");
    fix = sodaq_gps.scan(true, timeout_ms);
    if (fix) {
      debugSerial.println("GPS fix FOUND");
    } else {
      debugSerial.println("GPS fix TIMEOUT (" + String(timeout_ms) + String("ms") + String(")"));
    }   
  } else {
    fix = false;
  }
  return fix;
}

void RED() {
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, HIGH);
}

void GREEN() {
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, HIGH);
}

void BLUE() {
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, LOW);
}

void enable_led(int pin, bool enable) {
  if (enable) {
    digitalWrite(pin, LOW);
  } else {
    digitalWrite(pin, HIGH); 
  }
}

void flash_led(int pin) {
    for (size_t i = 0; i < 2; ++i) {
        delay(100);
        digitalWrite(pin, LOW);
        delay(100);
        digitalWrite(pin, HIGH);
    }
}

String convByteToHex(uint8_t val) {
  String valStr = String(val, HEX);
  if (valStr.length() == 1) {
    String newStr("0");
    newStr += valStr;
    valStr = newStr;
  }
  return valStr;
}

String convBytesToHex(uint8_t* val, int size) {
  String valStr;
  for(int i = (size-1); i >= 0; i--) {
      valStr += convByteToHex(val[i]);
  }
  return valStr;
}

String conv32BitToHex(uint32_t val) {
    String valStr;
    uint8_t* mybytes = (uint8_t*)&val;
    return convBytesToHex(mybytes, sizeof(val));
}

String conv64BitToHex(uint64_t val) {
    String valStr;
    uint8_t* mybytes = (uint8_t*)&val;
    return convBytesToHex(mybytes, sizeof(val));
}

String construct_pkt_str(uint32_t cnt_val, double lat, double lon) {
  String pkt_payload;
  uint64_t* p_dbl;
  /* Fixed values */
  pkt_payload += conv32BitToHex(0xFEEDDEAD);
  pkt_payload += conv32BitToHex(0xACABFACE);
  /* Latitude */
  p_dbl = (uint64_t*)&lat;
  pkt_payload += conv64BitToHex(*p_dbl);
  /* Longitude */
  p_dbl = (uint64_t*)&lon;
  pkt_payload += conv64BitToHex(*p_dbl);
  /* Add counter */  
  pkt_payload += conv32BitToHex(cnt_val);
  return pkt_payload;
}

void send_pkt(uint32_t cnt_val) {
  String cmd("radio tx ");
  /* Update GPS info */  
  double lat = sodaq_gps.getLat();
  double lon = sodaq_gps.getLon();
  /* Construct packet */
  cmd += construct_pkt_str(cnt_val, lat, lon);
  radio_tx(cmd);
  debugSerial.println(String("cnt=") + String(cnt_val) + String(" lat=") + String(lat) + String(" lon =") + String(lon) + String(" found_fix=") + String(found_fix));
}

void loop() {
  //send and receive data with serial
  if (debugSerial.available()) {
    //debugSerial.print("SEND:    ");
    while (debugSerial.available()) {
      uint8_t inChar = debugSerial.read();
      debugSerial.write(inChar);
      loraSerial.write(inChar);
    }
  }

  if (loraSerial.available()) {
    //debugSerial.print("RECEIVE: ");
    while (loraSerial.available()) {
      uint8_t inChar = loraSerial.read();
      debugSerial.write(inChar);
    }
  }

  /* Check if it is time to send the packet */
  if (millis() - last_time >= PKT_DELAY) {    
    send_pkt(cnt);
    cnt++;
    last_time = millis();
  }

  if (millis() - last_fix >= GPS_DELAY) {
    found_fix = find_fix(GPS_FIX_TIMEOUT_LOOP);
    last_fix = millis();
  }

  if (found_fix) {
    //flash_led(LED_GREEN);
    GREEN();
  } else {
    //flash_led(LED_RED);
    RED();
  }
}
