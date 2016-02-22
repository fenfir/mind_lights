#include <DmxSimple.h>
#include <Brain.h>
#include <SoftwareSerial.h>

void HSBToRGB(
    unsigned int inHue, unsigned int inSaturation, unsigned int inBrightness,
    unsigned int *oR, unsigned int *oG, unsigned int *oB );
    
SoftwareSerial softSerial(10, 11);
Brain brain(softSerial);

void setup() {
  DmxSimple.usePin(13);
  DmxSimple.maxChannel(7);

  softSerial.begin(9600);
  Serial.begin(9600);  
}


  int saturation = 255;
  int brightness = 255;
void loop() {
  int hue;
  
  unsigned int r,g,b;

    if (brain.update()) {
        Serial.println(brain.readErrors());
        Serial.println(brain.readCSV());
        brightness = 0;
    }
    
  int attention = brain.readAttention();
  hue = map(attention, 0, 90, 0, 255);
  
    HSBToRGB(hue, saturation, brightness, &r, &g, &b);

    brightness += 2;
    if (brightness > 255)
        brightness = 0;
    //*
    
    DmxSimple.write(1, 255);    
    /**/
    DmxSimple.write(3, r); 
    DmxSimple.write(4, b);
    DmxSimple.write(5, g);
    DmxSimple.write(6, 90);
    //*/
    /* Small delay to slow down the ramping */
    delay(10);
}

void HSBToRGB(
    unsigned int inHue, unsigned int inSaturation, unsigned int inBrightness,
    unsigned int *oR, unsigned int *oG, unsigned int *oB )
{
    if (inSaturation == 0)
    {
        // achromatic (grey)
        *oR = *oG = *oB = inBrightness;
    }
    else
    {
        unsigned int scaledHue = (inHue * 6);
        unsigned int sector = scaledHue >> 8; // sector 0 to 5 around the color wheel
        unsigned int offsetInSector = scaledHue - (sector << 8); // position within the sector         
        unsigned int p = (inBrightness * ( 255 - inSaturation )) >> 8;
        unsigned int q = (inBrightness * ( 255 - ((inSaturation * offsetInSector) >> 8) )) >> 8;
        unsigned int t = (inBrightness * ( 255 - ((inSaturation * ( 255 - offsetInSector )) >> 8) )) >> 8;

        switch( sector ) {
        case 0:
            *oR = inBrightness;
            *oG = t;
            *oB = p;
            break;
        case 1:
            *oR = q;
            *oG = inBrightness;
            *oB = p;
            break;
        case 2:
            *oR = p;
            *oG = inBrightness;
            *oB = t;
            break;
        case 3:
            *oR = p;
            *oG = q;
            *oB = inBrightness;
            break;
        case 4:
            *oR = t;
            *oG = p;
            *oB = inBrightness;
            break;
        default:    // case 5:
            *oR = inBrightness;
            *oG = p;
            *oB = q;
            break;
        }
    }
}

