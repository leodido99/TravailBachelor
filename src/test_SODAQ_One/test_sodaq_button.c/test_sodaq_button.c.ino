void setup() {
  // Start serial connection
  SerialUSB.begin(9600);
  // Configure the button as an input and enable the internal pull-up resistor
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(LED_GREEN, OUTPUT);
}

void loop() {
  // Read the button value into a variable
  int sensorVal = digitalRead(BUTTON);
  // Print out the value of the button
  SerialUSB.println(sensorVal);

  // Turn on the LED when the Button is pushed
  if (sensorVal == HIGH) {
    digitalWrite(LED_GREEN, HIGH);
  } else {
    digitalWrite(LED_GREEN, LOW);
  }
}

