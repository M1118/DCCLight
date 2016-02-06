#include <DCCLight.h>

DCCLight::DCCLight(int pin, unsigned int effect)
{
int	i;

  this->pin = pin;
  this->effect = effect;
  this->brightness = 0;
  if (LIGHT_EFFECT(effect) == LIGHT_EFFECT_SLOW_FLICKER)
    this->refresh = millis() + 1;
  else
    this->refresh = millis() + (this->period * 25);
  this->active = false;
  this->requestedActive = false;
  this->dim_cycle = 0;
  this->count = 0;

  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void DCCLight::loop()
{
  if (LIGHT_EFFECT(this->effect) == LIGHT_EFFECT_CONSTANT)
    return;

  if (! this->active)
    return;

  if (LIGHT_EFFECT(this->effect) == LIGHT_EFFECT_DIMABLE)
  {
    this->dim_cycle++;
    if (this->dim_cycle > 100)
    {
      this->dim_cycle = 0;
      digitalWrite(this->pin, HIGH);
    }
    if (this->dim_cycle == this->brightness)
    {
      digitalWrite(this->pin, LOW);
    }

    return;
  }

  if (LIGHT_EFFECT(this->effect) == LIGHT_EFFECT_DIMMED)
  {
    this->dim_cycle++;
    if (this->dim_cycle > 255)
    {
      this->dim_cycle = 0;
      digitalWrite(this->pin, HIGH);
    }
    if (this->dim_cycle == this->period)
    {
      digitalWrite(this->pin, LOW);
    }

    return;
  }

  if (LIGHT_EFFECT(this->effect) == LIGHT_EFFECT_FLICKER)
  {
    int rand = random(255);
    if (rand > 150)
      digitalWrite(this->pin, LOW);
    else
      digitalWrite(this->pin, HIGH);
    return;
  }
  
  if (this->refresh > millis())
    return;
  if (LIGHT_EFFECT(this->effect) == LIGHT_EFFECT_SLOW_FLICKER)
  {
    int rand = random(255);
    if (rand > 150)
      digitalWrite(this->pin, LOW);
    else
      digitalWrite(this->pin, HIGH);
    this->refresh = millis() + 1;
    return;
  }
  
  this->refresh = millis() + this->period * 25;
  this->count++;
  if (this->count >= 4)
    this->count = 0;
  switch (LIGHT_EFFECT(this->effect))
  {
  case LIGHT_EFFECT_FLASH_50:
    if (this->count < 2)
      digitalWrite(this->pin, HIGH);
    else
      digitalWrite(this->pin, LOW);
    break;
  case LIGHT_EFFECT_FLASH_25:
    if (this->count < 1)
      digitalWrite(this->pin, HIGH);
    else
      digitalWrite(this->pin, LOW);
    break;
  case LIGHT_EFFECT_FLASH_75:
    if (this->count < 3)
      digitalWrite(this->pin, HIGH);
    else
      digitalWrite(this->pin, LOW);
    break;
  }
 
}

void DCCLight::setDirection(boolean forwards)
{
  this->forwards = forwards;
  // Changing direction may turn the light on
  this->setActive(this->requestedActive);
}

void DCCLight::setBrightness(int percentage)
{
  this->brightness = percentage;
}

void DCCLight::setPeriod(int period)
{
  this->period = period;
}

void DCCLight::setActive(boolean active)
{
  this->requestedActive = active;
  switch (this->effect & LIGHT_DIRECTIONAL)
  {
  case 0:		// Both ways
  case (LIGHT_DIRECTION_FWD|LIGHT_DIRECTION_BCK):
    break;
  case LIGHT_DIRECTION_FWD:
    if (! this->forwards)
	active = false;
    break;
  case LIGHT_DIRECTION_BCK:
    if (this->forwards)
	active = false;
    break;
  }

  if ((active == true) && (this->active == false))
  {
    this->refresh = millis() + (this->period * 25);
  }
  this->active = active;
  if (! active)
  {
    digitalWrite(this->pin, LOW);
  }
  else if (LIGHT_EFFECT(this->effect) == LIGHT_EFFECT_CONSTANT)
  {
    digitalWrite(this->pin, HIGH);
  }
}
