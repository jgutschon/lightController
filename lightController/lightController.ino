#include <FastLED.h>

#define LED_PIN     7
#define NUM_LEDS    60
#define BRIGHTNESS  64
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

int8_t paletteIndex = 0;
int paletteSize = 8;
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

CRGBPalette16 palettes[] = {
    RainbowColors_p,
    RainbowStripeColors_p,
    OceanColors_p,
    CloudColors_p,
    LavaColors_p,
    ForestColors_p,
    PartyColors_p,
    myRedWhiteBluePalette_p,
};

TBlendType blends[] = {
    LINEARBLEND,
    NOBLEND,
    LINEARBLEND,
    LINEARBLEND,
    LINEARBLEND,
    LINEARBLEND,
    NOBLEND,
    LINEARBLEND,
};

void setup() {
    Serial.begin(9600);

    delay(3000); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);

    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
}

void loop() {
    if(Serial.available()) {
        char data = Serial.read();
        // Serial.println(data);
        switch(data) {
            case 'o': FastLED.setBrightness(BRIGHTNESS); break;   //lights on
            case 'f': FastLED.setBrightness(0); break;            //lights off
            case 'n': cyclePalette(data); break;                  //next palette
            case 'p': cyclePalette(data); break;                  //previous palette
        }
    }
    
    static uint8_t startIndex = 0;
    startIndex++; // motion speed
    
    FillLEDsFromPaletteColors(startIndex);
    
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void FillLEDsFromPaletteColors(uint8_t colorIndex) {
    uint8_t bright = 255;
    
    for(int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette(currentPalette, colorIndex, bright, currentBlending);
        colorIndex += 3;
    }
}

void cyclePalette(char data) {
    if (data == 'n')
        paletteIndex++;
    else if (data == 'p')
        paletteIndex--;
    
    // keep indexing in array bounds
    if (paletteIndex < 0)
        paletteIndex = paletteSize;
    if (paletteIndex > paletteSize - 1)
        paletteIndex = 0;
    
    currentPalette = palettes[paletteIndex];
    currentBlending = blends[paletteIndex];
}


// Custom Palettes
void blackAndWhiteStripes() {
    // black out all 16 palette entries
    fill_solid(currentPalette, 16, CRGB::Black);
    // set every fourth one to white
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
}

void SetupPurpleAndGreenPalette() {
    CRGB purple = CHSV(HUE_PURPLE, 255, 255);
    CRGB green  = CHSV(HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
        green,  green,  black,  black,
        purple, purple, black,  black,
        green,  green,  black,  black,
        purple, purple, black,  black );
}


// Example static color palette - 64 bytes of flash
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM = {
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
