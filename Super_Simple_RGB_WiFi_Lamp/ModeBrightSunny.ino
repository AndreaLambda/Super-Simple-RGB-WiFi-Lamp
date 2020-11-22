class ModeBrightSunny : public ModeBase
{
private:
    const int WALL_START_IDX = NUM_LEDS - LIGHTS_PER_WALL[3]; // 185
    // Config
    int sunnySpeed        =  10;
    int sunnyBrightExtend =   0;
    int sunnyBrightBri    = 128;
    int sunnyBrightSat    = 255;
    int sunnyNormalBri    = 100;
    int sunnyNormalSat    = 255;

    // Derived from config
    int normalStart       =   0;
    int brightStart       = WALL_START_IDX;
    int normalBriRatio    = 255;
    
    // State
    float addedHue;

    void setStateFromConfig() {
        // Constrain the variables before using
        sunnySpeed = constrain(sunnySpeed, 0, 255);
        sunnyBrightExtend = constrain(sunnyBrightExtend, 0, NUM_LEDS);
        sunnyBrightBri = constrain(sunnyBrightBri, 0, 255);
        sunnyBrightSat = constrain(sunnyBrightSat, 0, 255);
        sunnyNormalBri = constrain(sunnyNormalBri, 0, 255);
        sunnyNormalSat = constrain(sunnyNormalSat, 0, 255);

        if (sunnyNormalBri > sunnyBrightBri) {
          int temp = sunnyNormalBri;
          sunnyNormalBri = sunnyBrightBri;
          sunnyBrightBri = temp;
        }

        normalStart = sunnyBrightExtend;
        brightStart = WALL_START_IDX - sunnyBrightExtend;

        // TODO: confirm assumption that using Value 255 and setting overall FastLED brightness is more energy-efficient?
        FastLED.setBrightness(sunnyBrightBri);
        normalBriRatio = (sunnyBrightBri > 0 ? (sunnyNormalBri * 255 / sunnyBrightBri) : 0);
    }

public:
    ModeBrightSunny() {}

    virtual void initialize() {
        addedHue = 0;
        setStateFromConfig();
    }

    virtual void render() {
        int startHue = 0;
        int speed = sunnySpeed;

        // Update the hue by 1 every 360th of the allocated time
        if (speed > 0) {
          float sunnyDeltaHue = (255 / ((float)speed * 1000)) * 50;
          EVERY_N_MILLISECONDS(50) {
            addedHue += sunnyDeltaHue;
            addedHue = (addedHue > 255) ? addedHue - 255 : addedHue;
          };

          startHue += (int)addedHue;
        }

        // Calculate the sunny so it lines up
        float deltaHue = (float)255/(float)NUM_LEDS;
        float currentHue = startHue;
        bool isBright;
        for (int i = 0; i < NUM_LEDS; i++) {
          isBright = (i < normalStart || i >= brightStart);
          currentHue = startHue + (float)(deltaHue*i);
          currentHue = (currentHue < 255) ? currentHue : currentHue - 255;
          if (isBright) {
            ledString[i] = CHSV(currentHue, sunnyBrightSat, 255);
          } else {
            ledString[i] = CHSV(currentHue, sunnyNormalSat, normalBriRatio);
          }
        }
    }

    virtual void applyConfig(JsonVariant& settings) {
        settings["Speed"] = sunnySpeed = settings["Speed"] | sunnySpeed;
        settings["BrightExtend"] = sunnyBrightExtend = settings["BrightExtend"] | sunnyBrightExtend;
        settings["BrightBrightness"] = sunnyBrightBri = settings["BrightBrightness"] | sunnyBrightBri;
        settings["BrightSaturation"] = sunnyBrightSat = settings["BrightSaturation"] | sunnyBrightSat;
        settings["NormalBrightness"] = sunnyNormalBri = settings["NormalBrightness"] | sunnyNormalBri;
        settings["NormalSaturation"] = sunnyNormalSat = settings["NormalSaturation"] | sunnyNormalSat;
        setStateFromConfig();
    }
};
