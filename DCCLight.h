#ifndef _DCCLIGHT_H
#define _DCCLIGHT_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define LIGHT_EFFECT_CONSTANT		0x00
#define LIGHT_EFFECT_DIMABLE		0x01
#define LIGHT_EFFECT_DIMMED		0x02
#define LIGHT_EFFECT_FLASH_50		0x03
#define LIGHT_EFFECT_FLASH_25		0x04
#define LIGHT_EFFECT_FLASH_75		0x05
#define LIGHT_EFFECT_FLICKER		0x06
#define LIGHT_EFFECT_SLOW_FLICKER	0x07
#define LIGHT_EFFECT_RANDOM_FLASH	0x08
#define LIGHT_EFFECT_FLOURESCENT	0x09
#define LIGHT_EFFECT_SLOW_START		0x0A
#define LIGHT_EFFECT_PULSATING		0x0B

#define LIGHT_DIRECTIONAL		0xc0
#define LIGHT_DIRECTION_FWD		0x80
#define LIGHT_DIRECTION_BCK		0x40

#define LIGHT_EFFECT(x)		((x) & (~LIGHT_DIRECTIONAL))

class DCCLight {
  private:
    int			pin;
    boolean  		active;     // Is the light on?
    int      		brightness; // Current brightness percentage
    unsigned long	refresh;    // Next refresh time mills()
    unsigned int	period;	    // The period of a flash cycle
    unsigned int       	effect;     // The lighting effect
    unsigned int	dim_cycle;
    int			count;
    boolean		forwards;
    boolean		requestedActive;
    int			seqIndex;
    unsigned int	*sequence;
    boolean		on;
    boolean             increasing;
  public:
    DCCLight(int, unsigned int);
    void loop();
    void setBrightness(int);
    void setPeriod(int);
    void setActive(boolean);
    void setDirection(boolean);
};

// On-off sequences of flashes for flourescent light startup
static unsigned int fsequence[] = { 500, 350, 800, 200, 150, 150, 0 };

#endif
