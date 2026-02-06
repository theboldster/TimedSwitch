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


#define I2C_SDA 4 // Data
#define I2C_SCL 5 // Clock
#define I2C_RST 2 // RST / CE

virtuabotixRTC myRTC(I2C_SCL, I2C_SDA, I2C_RST); // 

/*
void setup()
{
  Serial.begin(9600);
  Serial.print("compiled: ");
  Serial.print(__DATE__);
  Serial.print(" ");
  Serial.println(__TIME__);
  // myRTC.initRTC(I2C_SCL, I2C_SDA, I2C_RST);
// Set the current date, and time in the following format:
// seconds, minutes, hours, day of the week, day of the month, month, year
  myRTC.setDS1302Time(0, 10, 17, 5, 6, 2, 2026);
}

void loop () 
{
myRTC.updateTime();
 // This allows for the update of variables for time
// or accessing the individual elements.
Serial.print("Date / Time: ");
 // Start printing elements as individuals
Serial.print(myRTC.dayofmonth);
Serial.print("/");
Serial.print(myRTC.month);
Serial.print("/");
Serial.print(myRTC.year);
Serial.print(" ");
Serial.print(myRTC.hours);
Serial.print(":");
Serial.print(myRTC.minutes);
Serial.print(":");
Serial.println(myRTC.seconds);
delay(1000);   
}

*/