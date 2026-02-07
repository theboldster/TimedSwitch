// Older Nano Boards need to have the processor set to ATmega328P (Old Bootloader)

// 1. Hook up RTC
// 2. Hook up I2C Multiplexer
// 3. Reattach RTC to MUX
// 4. Hook up Optocoupler
// 5. Hook up Speaker

// On Nano / Uno R3
// I2C SDA A4
// I2C SCL A5

// #define DS1302_ENABLE 0x8E                                                                    
// #define DS1302_TRICKLE 0x90   

#include <virtuabotixRTC.h>
// Creation of the Real Time Clock Object

// CONNECTIONS:
// DS1302 CLK/SCLK --> D6
// DS1302 DAT/IO --> D5
// DS1302 RST/CE --> D4
// DS1302 VCC --> 3.3v - 5v
// DS1302 GND --> GND

#define I2C_SDA 5 // Data
#define I2C_SCL 6 // Clock
#define I2C_RST 4 // RST / CE

// Debugflags
#define DEBUG_RTC TRUE
#define DEBUG_SC TRUE


#include <RtcDS1302.h>


ThreeWire myWire(5,6,4); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

#include "RTCRoutines.h"
#include "SunTime.h"

void setup()
{
  Serial.begin(9600);

#ifdef DEBUG_RTC
  Serial.print("compiled: ");
  Serial.print(__DATE__);
  Serial.println(__TIME__);
#endif

  // Wire.begin();
  Rtc.Begin();

#ifdef DEBUG_SC
// SunTimes fr den Tag iniiieren
SunTimes st = calc_suntimes(2026, 2, 7);
/*
Das ist das Ergebnis:
Daemmerung Beginn: 07:15:00
Sonnenaufgang: 07:48:19
Sonnenuntergang: 17:29:29
Daemmerung Ende: 18:02:48
*/
Serial.print("Daemmerung Beginn: "); print_time(st.dawn); Serial.print("\n");
Serial.print("Sonnenaufgang: "); print_time(st.sunrise); Serial.print("\n");
Serial.print("Sonnenuntergang: "); print_time(st.sunset); Serial.print("\n");
Serial.print("Daemmerung Ende: "); print_time(st.dusk); Serial.print("\n");
#endif

}



void loop () 
{
    RtcDateTime now = Rtc.GetDateTime();

    printDateTime(now);
    Serial.println();

    if (!now.IsValid())
    {
        // Common Causes:
        //    1) the battery on the device is low or even missing and the power line was disconnected
        Serial.println("RTC lost confidence in the DateTime!");
    }

    delay(500); // ten seconds
}


