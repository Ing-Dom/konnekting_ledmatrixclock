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
#include <FastLED.h>


// own classes
#include "timeslice.h"
#include "KNXClock.h"
#include "FileAsEEPROM.h"


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

// Define the array of leds
CRGB leds[NUM_LEDS];
bool myLEDs[NUM_LEDS];
bool myLEDs2D[10][11];

int mytime = 0;

byte color[3] = {0xFF, 0xFF, 0xFF};
byte backcolor[3] = {0x00, 0x00, 0x00};




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

  if(index == COMOBJ_color)
  {
      byte rgb[3];
      Knx.read(index, rgb);
      SetColor(rgb);
  }
  else if(index == COMOBJ_backcolor)
  {
      byte rgb[3];
      Knx.read(index, rgb);
      SetBackColor(rgb);
  }
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

  // while (!DEBUGSERIAL)

  // make debug serial port known to debug class
  // Means: KONNEKTING will use the same serial port for console debugging
  Debug.setPrintStream(&DEBUGSERIAL);
  #endif

  SetupLEDMatrix();


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
   pinMode(PROG_LED_PIN, OUTPUT);
  if(!digitalRead(PROG_BUTTON_PIN))
  {
    Debug.println(F("Jumper set to ProgMode. Starting programming mode..."));
    Konnekting.setProgState(true);
  }

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


  #if (DBGLVL & DBG_MAIN) != 0
  Debug.println(F("Setup is ready. go to loop..."));
  #endif
}


// ################################################
// ### LOOP
// ################################################
void loop()
{
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
  int minute = mytime % 60;
  int hour = (mytime / 60) % 12;

  //SetClock(hour, minute);
  //SetLED();


  mytime++;
}

void T5() // 10000ms
{
  #if (DBGLVL & DBG_MAIN) != 0
  Debug.println(F("T5"));
  #endif

  time_t currtimestamp = knxclock.Time();
  struct tm *my_time;
  my_time = gmtime(&currtimestamp);
  SetClock(my_time->tm_hour, my_time->tm_min);
  SetLED();


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

void SetupLEDMatrix()
{
  FastLED.setMaxPowerInMilliWatts(20000);

  
  for(int i = 0;i<NUM_LEDS;i++)
  {
    myLEDs[i] = false;
  }
  
    // Uncomment/edit one of the following lines for your leds arrangement.
    // ## Clockless types ##
     FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical


    SetLED();
}

void SetLED()
{

  
  for(int i = 0;i<NUM_LEDS;i++)
  {
    int col = i%11;
    int row = i/11;
    int j;
    if(col % 2 == 0)
      j = col * 10 + 9 - row;
    else
      j = col * 10 + row;
    
    if(myLEDs[i])
    {
      leds[j] = CRGB(color[0],color[1],color[2]);
    }
    else
    {
      leds[j] = CRGB(backcolor[0],backcolor[1],backcolor[2]);
    }
  }
  FastLED.show();
}

void SetArray(int s, int e)
{
  for(int i = s;i<=e;i++)
    myLEDs[i] = true;
}

void SetClock(int hour, int minute)
{
  for(int i = 0;i<NUM_LEDS;i++)
    myLEDs[i] = false;
  
  
  // set "ES"
  SetArray(0,1);
  
  // set "IST"
  SetArray(3,5);
  
  if(minute >= 0 && minute <= 4)
  {
    //set "UHR"
    SetArray(107,109);
  }
    
  else if (minute >= 5 && minute <= 9)
  {
     SetArray(7,10); // FÜNF
     SetArray(40,43); // NACH
  }
    
  else if (minute >= 10 && minute <= 14)
  {
     SetArray(11,14); // ZEHN
     SetArray(40,43); // NACH
  }

  else if (minute >= 15 && minute <= 19)
  {
     SetArray(26,32); // VIERTEL
     SetArray(40,43); // NACH
  }
  else if (minute >= 20 && minute <= 24)
  {
     SetArray(15,21); // ZWANZIG
     SetArray(40,43); // NACH
  }
  else if (minute >= 25 && minute <= 29)
  {
     SetArray(7,10); // FÜNF
     SetArray(33,35); // VOR
     SetArray(44,47); // HALB
  }
  else if (minute >= 30 && minute <= 34)
  {
     SetArray(44,47); // HALB
  }
  else if (minute >= 35 && minute <= 39)
  {
     SetArray(7,10); // FÜNF
     SetArray(40,43); // NACH
     SetArray(44,47); // HALB
  }
   else if (minute >= 40 && minute <= 44)
   {
     SetArray(11,14); // ZEHN
     SetArray(40,43); // NACH
     SetArray(44,47); // HALB
  }
  else if (minute >= 45 && minute <= 49)
  {
     SetArray(22,32); // DREIVIERTEL
  }
  else if (minute >= 50 && minute <= 54)
  {
     SetArray(11,14); // ZEHN
     SetArray(33,35); // VOR
  }
    else if (minute >= 55 && minute <= 59)
  {
     SetArray(7,10); // FÜNF
     SetArray(33,35); // VOR
  }

  int hour_disp = hour; 
  if(minute >= 25 && minute <= 59)
    hour_disp++;
  hour_disp %= 12;
   
    switch(hour_disp)
    {
      case 0:
        SetArray(94,98); // ZWÖLF
        break;
      case 1:
        if((minute >= 0 && minute <= 4))
          SetArray(55,57); // EIN
        else
          SetArray(55,58); // EINS
        break;
      case 2:
        SetArray(62,65); // ZWEI
        break;
      case 3:
        SetArray(66,69); // DREI
        break;
      case 4:
        SetArray(73,76); // VIER
        break;
      case 5:
        SetArray(51,54); // FÜNF
        break;
      case 6:
        SetArray(77,81); // SECHS
        break;
      case 7:
        SetArray(88,93); // SIEBEN
        break;
      case 8:
        SetArray(84,87); // ACHT
        break;
      case 9:
        SetArray(102,105); // NEUN
        break;
      case 10:
        SetArray(99,102); // ZEHN
        break;
      case 11:
        SetArray(49,51); // ELF
        break;
    }

    if((minute >= 0 && minute <= 4))
          SetArray(107,109); // UHR
}

void SetColor (byte rgb[3])
{
  color[0] = rgb[0];
  color[1] = rgb[1];
  color[2] = rgb[2];
  SetLED();
}

void SetBackColor (byte rgb[3])
{
  backcolor[0] = rgb[0];
  backcolor[1] = rgb[1];
  backcolor[2] = rgb[2];
  SetLED();
}


