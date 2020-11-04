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