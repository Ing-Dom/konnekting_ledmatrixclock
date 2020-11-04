/*
VIS-LMC.01 LED Matrix Clock
V0.0.0
*/

// Developed/Tested with
//
// Library Versions:
//
// KONNEKTING_Device_Library: develop_2020-10-31_18_00
// Adafriut_SPIFlash: 1.0.8 / 1.1.0
//
// Other Versions:
// Arduino IDE: 1.8.8
// Boardmanager Arduino SAMD Boards: 1.8.3
// Boardmanager Adafruit SAMD Boards: 1.5.3

// external libs
#include <KonnektingDevice.h>


// own classes
//#include "timeslice.h"
//#include "KNXClock.h"
#include "FileAsEEPROM.h"


// include device related configuration code, created by "KONNEKTING CodeGenerator"
#include "kdevice_VIS-LMC.01.h"

//configuration
#include "config.h"




// ################################################
// ### KONNEKTING Configuration
// ################################################

#define KNX_SERIAL Serial1 // Leonardo/Micro etc. use Serial1


//FlashStorage
byte readMemory(int index)
{
    Debug.println(F("FLASH read on index %d"),index);
    return FileEEPROM.read(index);
}
void writeMemory(int index, byte val)
{
    Debug.println(F("FLASH write value %d on index %d"),val, index);
    FileEEPROM.write(index, val);
}
void updateMemory(int index, byte val)
{
    Debug.println(F("FLASH update"));
    if (FileEEPROM.read(index) != val) {
        FileEEPROM.write(index, val);
    }
}
void commitMemory()
{
    Debug.println(F("FLASH commit"));
    FileEEPROM.commit();
}


// ################################################
// ### set ProgLED status
// ################################################
//this function is used to indicate programming mode.
//you can use LED, LCD display or what ever you want...
void progLed(bool state)
{
  //digitalWrite(PROG_LED_PIN, state);
}


// ################################################
// ### KNX EVENT CALLBACK
// ################################################
void knxEvents(byte index)
{
  //knxclock.KnxEvent(index);
}


// ################################################
// ### SETUP
// ################################################

void setup()
{

  // debug related stuff
  #ifdef KDEBUG

  // Start debug serial with 115200 bauds
  DEBUGSERIAL.begin(115200);

  while (!DEBUGSERIAL)

  // make debug serial port known to debug class
  // Means: KONNEKTING will sue the same serial port for console debugging
  Debug.setPrintStream(&DEBUGSERIAL);
  #endif

    Konnekting.setMemoryReadFunc(&readMemory);
  Konnekting.setMemoryWriteFunc(&writeMemory);
  Konnekting.setMemoryUpdateFunc(&updateMemory);
  Konnekting.setMemoryCommitFunc(&commitMemory);

  // Initialize KNX enabled Arduino Board
  Konnekting.init(KNX_SERIAL,
                    &progLed,
                    MANUFACTURER_ID,
                    DEVICE_ID,
                    REVISION);

   pinMode(PROG_BUTTON_PIN, INPUT_PULLUP);
  if(!digitalRead(PROG_BUTTON_PIN))
  {
    Debug.println(F("Jumper set to ProgMode. Starting programming mode..."));
    Konnekting.setProgState(true);
  }

  // If device has been parametrized with KONNEKTING Suite, read params from EEPROM
  // Otherwise continue with global default values from sketch
  if (!Konnekting.isFactorySetting())
  {
      //knxclock.Setup();
  }
  else
  {
    Konnekting.setProgState(true);
  }
  //timeslice_setup();


  #if (DBGLVL & DBG_MAIN) != 0
  Debug.println(F("Setup is ready. go to loop..."));
  #endif
}



const int ledPin =  LED_BUILTIN;// the number of the LED pin
int ledState = LOW;             // ledState used to set the LED
unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 200;           // interval at which to blink (milliseconds)

// ################################################
// ### LOOP
// ################################################
void loop()
{
    Knx.task();
    
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
    Debug.print(F("DEBUG! free ram: %d bytes \n"), Debug.freeRam());
  }



    //timeslice_scheduler();
    // only do application stuff if not in programming mode
    if (Konnekting.isReadyForApplication())
    {
      
    }
}