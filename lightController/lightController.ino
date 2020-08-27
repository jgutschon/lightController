#include <FastLED.h>

#define LED_PIN     7
#define NUM_LEDS    30
#define BRIGHTNESS  64
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

uint8_t paletteIndex = 0;
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

CRGBPalette16 palettes[8] = {
    RainbowColors_p,
    RainbowStripeColors_p,
    OceanColors_p,
    CloudColors_p,
    LavaColors_p,
    ForestColors_p,
    PartyColors_p,
    myRedWhiteBluePalette_p
};


void setup() {
    Serial.begin(9600);

    delay(3000); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);

    currentPalette = RainbowColors_p;
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

    currentBlending = LINEARBLEND;
    
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
    
    if (paletteIndex < 0)
        paletteIndex = 7;
    if (paletteIndex > 7)
        paletteIndex = 0;
    
    currentPalette = palettes[paletteIndex];
}


// FastLED preset palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.

// Set up a palette of black and white stripes
void SetupBlackAndWhiteStripedPalette() {
    // 'black out' all 16 palette entries...
    fill_solid(currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
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


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
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



// Additionl notes on FastLED compact palettes:
//
// Normally, in computer graphics, the palette (or "color lookup table")
// has 256 entries, each containing a specific 24-bit RGB color.  You can then
// index into the color palette using a simple 8-bit (one byte) value.
// A 256-entry color palette takes up 768 bytes of RAM, which on Arduino
// is quite possibly "too many" bytes.
//
// FastLED does offer traditional 256-element palettes, for setups that
// can afford the 768-byte cost in RAM.
//
// However, FastLED also offers a compact alternative.  FastLED offers
// palettes that store 16 distinct entries, but can be accessed AS IF
// they actually have 256 entries; this is accomplished by interpolating
// between the 16 explicit entries to create fifteen intermediate palette
// entries between each pair.
//
// So for example, if you set the first two explicit entries of a compact 
// palette to Green (0,255,0) and Blue (0,0,255), and then retrieved 
// the first sixteen entries from the virtual palette (of 256), you'd get
// Green, followed by a smooth gradient from green-to-blue, and then Blue.