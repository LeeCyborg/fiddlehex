
#include <limits.h>
#include <Arduino.h>

#include "ArdDmx.h"

// Globals
// -------

/** @brief Proximity sensor 1 trigger pin */
#define PROXIMITY1_TRIG_PIN 10
/** @brief Proximity sensor 1 echo pin */
#define PROXIMITY1_ECHO_PIN 13

/** @brief Proximity sensor 2 trigger pin */
#define PROXIMITY2_TRIG_PIN 9
/** @brief Proximity sensor 2 echo pin */
#define PROXIMITY2_ECHO_PIN 12

int hue1 = 110;
int hue2 = 200;
int hue1_range[2] = {100, 135};
int hue2_range[2] = {171, 215};
int incr = 3;
int thresh = 50;

int update_hue(int hue, const int range[], const bool increase){
    if (increase)  {
        hue += incr;
    } else {
        hue -= incr;
    }
    return constrain(hue, range[0], range[1]);
}

int proximity(int trigger, int echo){
    digitalWrite(trigger, LOW);
    delayMicroseconds(2);
    digitalWrite(trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger, LOW);

    float duration = pulseIn(echo, HIGH);
    float distance = (duration / 2) * 0.0344;

    delay(300);
    return distance;
}

/** @brief DMX shield master serial pin */
#define DMX_PIN 2
ArdDmx g_dmx;

void setup() {
    // Proximity
    // ---------

    pinMode(PROXIMITY1_TRIG_PIN, OUTPUT);
    pinMode(PROXIMITY1_ECHO_PIN, INPUT);
    pinMode(PROXIMITY2_TRIG_PIN, OUTPUT);
    pinMode(PROXIMITY2_ECHO_PIN, INPUT);

    // DMX
    // ---

    // Parameters
    ArdDmxParameters params;
    params.num_leds = 6;
    params.pin = DMX_PIN;
    params.color_order = kArdDmxRGB;
    params.dmx_channel_offset = 10;
    params.dmx_channels_per_light = 10;
    // initialize
    eArdDmxStatus status = g_dmx.Initialize(params);
    if (status != kArdDmxSuccess) {
        do
        {
            delay(1000);
        } while (true);
    }
}

// *** REPLACE FROM HERE ***
// ---> here we declare the effect function <---

void loop() {
    bool increase_hue1 = (proximity(PROXIMITY1_TRIG_PIN, PROXIMITY1_ECHO_PIN) >= thresh);
    hue1 = update_hue(hue1, hue1_range, increase_hue1);
    bool increase_hue2 = (proximity(PROXIMITY2_TRIG_PIN, PROXIMITY2_ECHO_PIN) >= thresh);
    hue2 = update_hue(hue2, hue2_range, increase_hue2);

    CRGB light1 = CRGB().setHSV(hue1, 255, 255);
    CRGB light2 = CRGB().setHSV(hue2, 255, 255);

    g_dmx.SetPixel(0, light1.r, light1.g, light1.b);
    g_dmx.SetPixel(1, light2.r, light2.g, light2.b);
    g_dmx.SetPixel(2, light1.r, light1.g, light1.b);
    g_dmx.SetPixel(3, light2.r, light2.g, light2.b);
    g_dmx.SetPixel(4, light1.r, light1.g, light1.b);
    g_dmx.SetPixel(5, light2.r, light2.g, light2.b);

    g_dmx.ShowStrip();
}

// ---> here we define the effect function <---
// *** REPLACE TO HERE ***
