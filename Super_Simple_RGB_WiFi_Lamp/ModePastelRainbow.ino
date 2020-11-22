class ModePastelRainbow : public ModeBase
{
private:
    // Config
    int rainbowStartHue     = 0;
    int rainbowSpeed        = 10;
    int rainbowBri          = 100;
    int rainbowSaturation   = 255;

    // State
    float rainbowAddedHue;

public:
    ModePastelRainbow() {}

    virtual void initialize() {
        rainbowAddedHue = 0;
    }

    virtual void render() {
        int startHue = rainbowStartHue;
        int speed = rainbowSpeed;
        int brightness = rainbowBri;
        int saturation = rainbowSaturation;

        // Constrain the variables before using
        startHue = constrain(startHue, 0, 255);
        speed = speed > 0 ? speed : 0;
        brightness = constrain(brightness, 0, 255);
        saturation = constrain(saturation, 0, 255);

        // Update the hue by 1 every 360th of the allocated time
        if (speed > 0) {
          float rainbowDeltaHue = (255 / ((float)speed * 1000)) * 50;
          EVERY_N_MILLISECONDS(50) {
            rainbowAddedHue += rainbowDeltaHue;
            rainbowAddedHue = (rainbowAddedHue > 255) ? rainbowAddedHue - 255 : rainbowAddedHue;
          };

          startHue += (int)rainbowAddedHue;
        }

        // Calculate the rainbow so it lines up
        float deltaHue = (float)255/(float)NUM_LEDS;
        float currentHue = startHue;
        for (int i = 0; i < NUM_LEDS; i++) {
          currentHue = startHue + (float)(deltaHue*i);
          currentHue = (currentHue < 255) ? currentHue : currentHue - 255;
          ledString[i] = CHSV(currentHue, saturation, 255);
        }

        FastLED.setBrightness(brightness);
    }

    virtual void applyConfig(JsonVariant& settings) {
        settings["Hue"] = rainbowStartHue = settings["Hue"] | rainbowStartHue;
        settings["Speed"] = rainbowSpeed = settings["Speed"] | rainbowSpeed;
        settings["Brightness"] = rainbowBri = settings["Brightness"] | rainbowBri;
        settings["Saturation"] = rainbowSaturation = settings["Saturation"] | rainbowSaturation;
    }
};
