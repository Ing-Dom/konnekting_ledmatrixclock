// ################################################
// ### DEBUG CONFIGURATION
// ################################################
#define KDEBUG // comment this line to disable DEBUG mode
#ifdef KDEBUG
#include <DebugUtil.h>
#endif

#define DBG_MAIN 1
#define DBG_TIMESLICE 2
#define DBG_KRSXL_COMMON 4
#define DBG_KNXCLOCK 8
#define DBG_DIMMER 16
#define DBG_DIMMCHANNEL 32
#define DBG_HWCHANNEL 64
#define DBG_FLASHSTORAGE 128

//#define DBGLVL  (DBG_MAIN | DBG_TIMESLICE | DBG_KNXCLOCK)
#define DBGLVL  0xffff

//
#define BETA 1
//define BETA 0



// ################################################
// ### KONNEKTING Configuration
// ################################################


#define DEBUGSERIAL Serial //(USB)
#define PROG_BUTTON_PIN 3
#define PROG_LED_PIN 13


// How many leds in your strip?
#define NUM_LEDS 110

// For led chips like WS2812, which have a data line, ground, && power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, && power), like the LPD8806 define both DATA_PIN && CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 5


#define COLOR CRGB::Red;