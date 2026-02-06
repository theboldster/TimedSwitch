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


#include <RtcDS1302.h>

ThreeWire myWire(5,6,4); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);


void setup()
{
  Serial.begin(9600);

  Serial.print("compiled: ");
  Serial.print(__DATE__);
  Serial.println(__TIME__);

  // Wire.begin();
  Rtc.Begin();

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  printDateTime(compiled);
  Serial.println();

  if (!Rtc.IsDateTimeValid()) 
  {
      // Common Causes:
      //    1) first time you ran and the device wasn't running yet
      //    2) the battery on the device is low or even missing

      Serial.println("RTC lost confidence in the DateTime!");
      Rtc.SetDateTime(compiled);
  }

  if (Rtc.GetIsWriteProtected())
  {
      Serial.println("RTC was write protected, enabling writing now");
      Rtc.SetIsWriteProtected(false);
  }

  if (!Rtc.GetIsRunning())
  {
      Serial.println("RTC was not actively running, starting now");
      Rtc.SetIsRunning(true);
  }

  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled) 
  {
      Serial.println("RTC is older than compile time!  (Updating DateTime)");
      Rtc.SetDateTime(compiled);
  }
  else if (now > compiled) 
  {
      Serial.println("RTC is newer than compile time. (this is expected)");
  }
  else if (now == compiled) 
  {
      Serial.println("RTC is the same as compile time! (not expected but all is fine)");
  }
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

#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime& dt)
{
    char datestring[26];

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Month(),
            dt.Day(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    Serial.print(datestring);
}

