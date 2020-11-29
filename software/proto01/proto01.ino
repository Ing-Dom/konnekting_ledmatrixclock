/*
VIS-L;C.01 LED Matrix Clock
V0.0.0
*/

// Developed/Tested with
//
// Library Versions:
//
// KONNEKTING_Device_Library: 1.0.0-BETA4b_mod
//
// Other Versions:
// Arduino IDE: 1.8.8

// external libs
#include <KonnektingDevice.h>


// own classes
#include "timeslice.h"
#include "KNXClock.h"


// include device related configuration code, created by "KONNEKTING CodeGenerator"
#include "kdevice_VIS-LMC.01.h"

//configuration
#include "config.h"


// ################################################
// ### Global variables, sketch related
// ################################################
KNXCLock knxclock(COMOBJ_gen_datetime);


//ErrorCode
uint32_t g_ErrorCode = 0;

const int ledPin =  LED_BUILTIN;// the number of the LED pin
int ledState = LOW;             // ledState used to set the LED
unsigned long previousMillis = 0;        // will store last time LED was updated
long interval = 200;           // interval at which to blink (milliseconds)




// ################################################
// ### set ProgLED status
// ################################################
//this function is used to indicate programming mode.
//you can use LED, LCD display or what ever you want...
void progLed(bool state)
{
  if(state)
    interval = 50;
  else
    interval = 200;
}


// ################################################
// ### KNX EVENT CALLBACK
// ################################################
void knxEvents(byte index)
{
  knxclock.KnxEvent(index);
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

  //while (!DEBUGSERIAL)

  // make debug serial port known to debug class
  // Means: KONNEKTING will sue the same serial port for console debugging
  Debug.setPrintStream(&DEBUGSERIAL);
  #endif

  pinMode(PROG_LED_PIN, OUTPUT);
  pinMode(PROG_BUTTON_PIN, INPUT_PULLUP);
  if(!digitalRead(PROG_BUTTON_PIN))
  {
    Debug.println(F("Jumper set to ProgMode. Del EEPROM and Starting programming mode..."));

      for (int i = 0 ; i < 256 ; i++) {
    EEPROM.write(i, 0xFF);
  }

    Konnekting.setProgState(true);
  }
  else
  {
     Debug.println(F("Jumper set to normal"));
  }

  // Initialize KNX enabled Arduino Board
  Konnekting.init(KNX_SERIAL,
                    &progLed,
                    MANUFACTURER_ID,
                    DEVICE_ID,
                    REVISION);

  
  

  // If device has been parametrized with KONNEKTING Suite, read params from EEPROM
  // Otherwise continue with global default values from sketch
  if (!Konnekting.isFactorySetting())
  {
      knxclock.Setup();
  }
  else
  {
    Konnekting.setProgState(true);
  }
  timeslice_setup();

  if(!digitalRead(PROG_BUTTON_PIN))
  {
    Debug.println(F("Jumper set to ProgMode. Starting programming mode..."));

    Konnekting.setProgState(true);
  }


  #if (DBGLVL & DBG_MAIN) != 0
  Debug.println(F("Setup is ready. go to loop..."));
  #endif
}

// ################################################
// ### LOOP
// ################################################
void loop()
{
  if(true) // blink
  {
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
      //Debug.print(F("DEBUG! free ram: %d bytes \n"), Debug.freeRam());
    }
  }

    Knx.task();
    // only do application stuff if not in programming mode
    if (Konnekting.isReadyForApplication())
    {
      timeslice_scheduler();
    }
}

void T1() // 1ms
{
}

void T2() // 5ms
{
}

void T3() // 25ms
{
}

void T4() // 500ms
{
}

void T5() // 10000ms
{
  #if (DBGLVL & DBG_MAIN) != 0
  Debug.println(F("T5"));
  #endif

  if(g_ErrorCode || BETA) // send only when != 0 OR when BETA
  {
    Knx.write(COMOBJ_error_code, g_ErrorCode);
  }
}


// ################################################
// ### HELPER FUNCTIONS
// ################################################


void SetErrorCode(uint32_t ErrorBits, uint32_t Mask)
{
  g_ErrorCode &= ~Mask; // Bits löschen
  g_ErrorCode |= ErrorBits;
}