#define FASTLED_ALLOW_INTERRUPTS 0

#include <FastLED.h>

#include <String.h>
#include <stdio.h>

String cmd = "";

#define DEBUG 0
#define DATA 5
#define LAMP 9

#define deviceID 1

#define NUM_LEDS    144
#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

#define REFRESH_TIME 500

CRGB leds[NUM_LEDS];

int STATE = 0;
uint8_t brightness = 255;

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

void SetupPurpleAndGreenPalette(void);
void FillLEDsFromPaletteColors(uint8_t colorIndex);
void ChangePalettePeriodically(void);
void SetupTotallyRandomPalette(void);
void SetupBlackAndWhiteStripedPalette(void);
void SetupBlackPalette(void);
void SetupWhitePalette(void);

bool isSame(char *command, char *format, int length_f);


void setup() {
    Serial.begin(115200);

    pinMode(LAMP, OUTPUT);

    digitalWrite(LAMP, LOW);

    FastLED.addLeds<LED_TYPE, DATA, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalSMD5050 );
    FastLED.setBrightness(  BRIGHTNESS );

    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
}

static uint8_t startIndex = 0;

int timer_1 = millis();
int timer_2 = millis();

void loop() {

    if (Serial.available() > 0) {
        char c = Serial.read();

        if (c != '\n') {
            cmd += c;
        } else {

            int buffer_l = cmd.length() + 1;
            char buffer[buffer_l];

            cmd.toCharArray(buffer, buffer_l);
            
            char commd[50] = {0};
            int device;

            sscanf(buffer, "NewGetRequest:deviceID=%i&command=%s\0", &device, commd);

            if (device == deviceID) {
                if (isSame(commd, "LAMP_ON", 7)) {
                    digitalWrite(LAMP, HIGH);
                } else if (isSame(commd, "LAMP_OFF", 8)) {
                    digitalWrite(LAMP, LOW);
                } else if (isSame(commd, "LED_ON", 6)) {
                    fadeOut();
                    currentPalette = RainbowColors_p;
                    currentBlending = LINEARBLEND;
                    fadeIn();
                } else if (isSame(commd, "LED_OFF", 7)) {
                    fadeOut();
                    SetupBlackPalette();
                    fadeIn();
                } else if (isSame(commd, "LED_0", 5)) {
                    fadeOut();
                    ChangePalettePeriodically();
                    fadeIn();
                } else if (isSame(commd, "LED_1", 5)) {
                    fadeOut();
                    SetupBlackPalette();
                    fadeIn();
                } else if (isSame(commd, "LED_2", 5)) {
                    fadeOut();
                    currentPalette = RainbowColors_p;
                    currentBlending = LINEARBLEND;
                    fadeIn();
                } else if (isSame(commd, "LED_3", 5)) {
                    fadeOut();
                    currentPalette = RainbowStripeColors_p;
                    currentBlending = LINEARBLEND;
                    fadeIn();
                } else if (isSame(commd, "LED_4", 5)) {
                    fadeOut();
                    SetupPurpleAndGreenPalette();
                    currentBlending = LINEARBLEND;
                    fadeIn();
                } else if (isSame(commd, "LED_5", 5)) {
                    fadeOut();
                    SetupTotallyRandomPalette();
                    currentBlending = LINEARBLEND;
                    fadeIn();
                } else if (isSame(commd, "LED_6", 5)) {
                    fadeOut();
                    SetupBlackAndWhiteStripedPalette();
                    currentBlending = LINEARBLEND;
                    fadeIn();
                } else if (isSame(commd, "LED_7", 5)) {
                    fadeOut();
                    currentPalette = CloudColors_p;
                    currentBlending = LINEARBLEND;
                    fadeIn();
                } else if (isSame(commd, "LED_8", 5)) {
                    fadeOut();
                    currentPalette = PartyColors_p;
                    currentBlending = LINEARBLEND;
                    fadeIn();
                } else if (isSame(commd, "LED_9", 5)) {
                    fadeOut();
                    SetupWhitePalette();
                    fadeIn();
                } else if (isSame(commd, "ALL_OFF", 7)) {
                    fadeOut();
                    digitalWrite(LAMP, LOW);
                    SetupBlackPalette();
                    fadeIn();
                } else if (isSame(commd, "AMBIANT_LIGHTING", 16)) {
                    fadeOut();
                    currentPalette = RainbowColors_p;
                    currentBlending = LINEARBLEND;
                    digitalWrite(LAMP, LOW);
                    fadeIn();
                } else if (isSame(commd, "NORMAL_LIGHTING", 15)) {
                    fadeOut();
                    SetupWhitePalette();
                    brightness = 255;
                    fadeIn();
                } else if (isSame(commd, "LOWER_LIGHT", 11)) {
                    if (brightness - 15 > 0) {
                        brightness -= 15;
                    } else {
                        brightness = 0;
                    }
                } else if (isSame(commd, "HIGHER_LIGHT", 12)) {
                    if (brightness + 15 < 255) {
                        brightness += 15;
                    } else {
                        brightness = 255;
                    }
                }
            }

            cmd = "";
        }
    }

    timer_2 = millis();

    if (timer_2 - timer_1 > REFRESH_TIME) {

        startIndex = startIndex + 1; /* motion speed */

        FillLEDsFromPaletteColors(startIndex);

        FastLED.show();

        timer_1 = millis();
    }

}

void FillLEDsFromPaletteColors(uint8_t colorIndex)
{
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;

    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
        if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
        if( secondHand == 20)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
        if( secondHand == 25)  { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
        if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
        if( secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 45)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 55)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
    }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;

}

void fadeIn() {
    FillLEDsFromPaletteColors(startIndex);
    FastLED.show();
    while (brightness != 255) {
        brightness++;
        FillLEDsFromPaletteColors(startIndex);
        FastLED.show();
        delay(10);
    }
}

void fadeOut() {
    while (brightness != 0) {
        brightness--;
        FillLEDsFromPaletteColors(startIndex);
        FastLED.show();
        delay(10);
    }
}

void SetupBlackPalette() {
    fill_solid(currentPalette, 16, CRGB::Black);
}

void SetupWarmWhitePalette() {

    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i].red   = 255;
        leds[i].green = 193;
        leds[i].blue  = 7; // rgb(255, 193, 7)
    }
}

void SetupWhitePalette() {
    fill_solid(currentPalette, 16, CRGB::White);
}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;

    currentPalette = CRGBPalette16(
            green,  green,  black,  black,
            purple, purple, black,  black,
            green,  green,  black,  black,
            purple, purple, black,  black );
}


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
        {
                CRGB::Red,
                CRGB::Gray, // 'white' is too bright compared to red and blue
                CRGB::Blue,
                CRGB::Black,

                CRGB::Red,
                CRGB::Gray,
                CRGB::Blue,
                CRGB::Black,

                CRGB::Red,
                CRGB::Red,
                CRGB::Gray,
                CRGB::Gray,
                CRGB::Blue,
                CRGB::Blue,
                CRGB::Black,
                CRGB::Black
        };

bool isSame(char *command, char *format, int length_f) {
    for(int i = 0; i < length_f; i++) {
        if (command[i] != format[i]) {
            return false;
        }
    }

    return true;
    
}