#include <FastLED.h>
#include <ArduinoSTL.h>

#define LED_PIN     7
#define NUM_LEDS    30
#define BRIGHTNESS  64
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

int8_t effectIndex = 0;
CRGBPalette16 currPalette;
TBlendType    currBlend;

extern CRGBPalette16 redWhiteBluePalette;
extern const TProgmemPalette16 redWhiteBluePalette_p PROGMEM;

std::vector<std::pair<CRGBPalette16, TBlendType>> effects {
    {RainbowColors_p,           LINEARBLEND},
    {RainbowStripeColors_p,     NOBLEND},
    {OceanColors_p,             LINEARBLEND},
    {CloudColors_p,             LINEARBLEND},
    {LavaColors_p,              LINEARBLEND},
    {ForestColors_p,            LINEARBLEND},
    {PartyColors_p,             NOBLEND},
    {redWhiteBluePalette_p,     LINEARBLEND}
};

void setup() {
    Serial.begin(9600);

    delay(3000); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);

    currPalette = effects[effectIndex].first;
    currBlend = effects[effectIndex].second;
}

void loop() {
    if(Serial.available()) {
        char data = Serial.read();
        // Serial.println(data);
        switch(data) {
            case 'o': FastLED.setBrightness(BRIGHTNESS); break;   //lights on
            case 'f': FastLED.setBrightness(0); break;            //lights off
            case 'n': cycleEffect(data); break;                  //next palette
            case 'p': cycleEffect(data); break;                  //previous palette
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
        leds[i] = ColorFromPalette(currPalette, colorIndex, bright, currBlend);
        colorIndex += 3;
    }
}

void cycleEffect(char data) {
    if (data == 'n')
        effectIndex++;
    else if (data == 'p')
        effectIndex--;
    
    // keep indexing in array bounds
    if (effectIndex < 0)
        effectIndex = effects.size();
    else if (effectIndex > effects.size() - 1)
        effectIndex = 0;
    
    currPalette = effects[effectIndex].first;
    currBlend = effects[effectIndex].second;
}


// Custom Palettes
void blackAndWhiteStripes() {
    // black out all 16 palette entries
    fill_solid(currPalette, 16, CRGB::Black);
    // set every fourth one to white
    currPalette[0] = CRGB::White;
    currPalette[4] = CRGB::White;
    currPalette[8] = CRGB::White;
    currPalette[12] = CRGB::White;
}

void SetupPurpleAndGreenPalette() {
    CRGB purple = CHSV(HUE_PURPLE, 255, 255);
    CRGB green  = CHSV(HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currPalette = CRGBPalette16(
        green,  green,  black,  black,
        purple, purple, black,  black,
        green,  green,  black,  black,
        purple, purple, black,  black );
}


// static color palette - 64 bytes of flash
const TProgmemPalette16 redWhiteBluePalette_p PROGMEM = {
    CRGB::Red,
    CRGB::Gray,
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
