/* Example for using the accelerometer with the Sodaq_LSM303AGR library */
/* The example is tested on an Arduino M0 and a Sodaq Explorer with a Sodaq NB-IoT shield */
/* This code prints accelerometer readings every second and registers whether the board is flipped by interrupt */
#include <Arduino.h>
#include <Sodaq_LSM303AGR.h>

Sodaq_LSM303AGR AccMeter;

// Print the debug information on the SerialUSB 
#define USB SerialUSB

// The interrupt pin for the Accelerometer is attached to D4
#define ACC_INT_PIN 4

// Threshold for interrupt trigger
double threshold = -0.8;
int now = millis();

void setup() {
    USB.begin(9600);
    while ((!USB) && (millis() < 10000)) {
        // Wait 10 seconds for the Serial Monitor
    }

    // Start the I2C bug
    Wire.begin();

    AccMeter.rebootAccelerometer();
    delay(1000);

    // Enable the Accelerometer
    AccMeter.enableAccelerometer();

    // Attach interrupt event fot the Accelerometer
    pinMode(ACC_INT_PIN, INPUT);
    attachInterrupt(ACC_INT_PIN, interrupt_event, RISING);

    // Enable interrupts on the SAMD 
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(GCM_EIC) |
        GCLK_CLKCTRL_GEN_GCLK1 |
        GCLK_CLKCTRL_CLKEN;

    // If Z goes below threshold the interrupt is triggered
    AccMeter.enableInterrupt1(AccMeter.ZLow, threshold, 0, AccMeter.PositionRecognition);
}

void loop() {

    // Print sensor readings every second
    if ((now + 1000) < millis())
    {
        now = millis();
        read_AccMeter();
    }
}

void read_AccMeter()
{
    USB.print("x = ");
    USB.print(AccMeter.getX());
    USB.print(" (");
    USB.print(AccMeter.getXRaw());
    USB.print(" ");
    USB.print(AccMeter.getXRawUnsigned());
    USB.print(" )");

    USB.print(" y = ");
    USB.print(AccMeter.getY());
    USB.print(" (");
    USB.print(AccMeter.getYRaw());
    USB.print(" ");
    USB.print(AccMeter.getYRawUnsigned());    
    USB.print(" )");    

    USB.print(" z = ");
    USB.print(AccMeter.getZ());
    USB.print(" (");
    USB.print(AccMeter.getZRaw());
    USB.print(" ");
    USB.print(AccMeter.getZRawUnsigned());    
    USB.println(" )");
}

void interrupt_event()
{
    // Do not print in an interrupt event when sleep is enabled.
    USB.println("Board flipped");
}

