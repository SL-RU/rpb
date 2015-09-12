#include "hw.hpp"

GButton::GButton(int Gpin, bool buttonPressedLevel):
  MinClickDur(70),
  MinPressDur(400),
  pin(Gpin),
  pressLevel(buttonPressedLevel),
  pressStartTime(0),
  clickEvent(0),
  pressEvent(0)
{
  pinMode(Gpin, INPUT);
  if(buttonPressedLevel)
    {
      pullUpDnControl(Gpin, PUD_DOWN);
    }
  else
    {
      pullUpDnControl(Gpin, PUD_UP);
    }
  id = Add_HardwareObject(this);
}

void GButton::Update(unsigned int time)
{
  int r = digitalRead(pin);
  if(r == pressLevel) //if button down
    {
      if(pressStartTime == 0)
	{
	  pressStartTime = time;
	}
      if(time - pressStartTime >= MinPressDur) 
	{
	  IndicateClick();
	}  
    }
  else //if button up
    {
      if(pressStartTime != 0)
	{
	  if(time - pressStartTime >= MinClickDur && time - pressStartTime < MinPressDur)
	    {
	      //Execute click function
	      IndicateClick();
	      if(clickEvent != 0)
		{
		  (*clickEvent)(pin);
		}
	    }
	  else if(time - pressStartTime >= MinClickDur)
	    {
	      //Execute press function
	      if(pressEvent != 0)
		{
		  (*pressEvent)(pin);
		}
	    }
	  pressStartTime = 0;
	}
      else
	{
	}
    }
}

void GButton::setOnClick(void (*func)(int))
{
  clickEvent = func;
}
void GButton::setOnPress(void (*func)(int))
{
  pressEvent = func;
}


GLed::GLed(int Gpin, bool ledLightsLevel):
  pin(Gpin),
  lightsLevel(ledLightsLevel),
  curState(0),
  blinkDuration(-1),
  blinkStartTime(0)
{
  pinMode(Gpin, OUTPUT);
  id = Add_HardwareObject(this);
  set(false);
}

void GLed::set(bool state)
{
  digitalWrite(pin, state == lightsLevel);
  curState = state;
}

void GLed::blink(int dur)
{
  if(dur>=1)
    {
      set(1);
      blinkDuration = dur;
      blinkStartTime = 0;
    }
  else
    {
      set(0);
      blinkDuration = -1;
      blinkStartTime = 0;
    }
}

void GLed::Update(unsigned int time)
{
  if(blinkDuration != -1)
    {
      if(blinkStartTime != 0)
	{
	  if(blinkStartTime + blinkDuration >= time)
	    set(true);
	  else
	    {
	      set(false);
	      blinkStartTime = 0;
	      blinkDuration = -1;
	    }
	}
      else
	{
	  blinkStartTime = time;
	}
    }
  else
    {
      blinkStartTime = -1;
    }
}

std::list<HardwareObject*> *UpdateHardwareObjectsList = 0;
GLed *_indicatorLed = 0;


void Init_Hardware()
{
  wiringPiSetup();
  UpdateHardwareObjectsList = new std::list<HardwareObject*>;
}
int Add_HardwareObject(HardwareObject * ho)
{
  UpdateHardwareObjectsList->push_back(ho);
  return (int)UpdateHardwareObjectsList->size();
}

void Update_Hardware()
{
  while (1)
    {
      for(auto *i : *UpdateHardwareObjectsList)
	{
	  i->Update(millis());
	}
      delay(HardwareUpdatesDelay);
    }
}

void SetIndicatorLed(GLed * led)
{
  _indicatorLed = led;
}
void IndicateClick()
{
  if(_indicatorLed != 0)
    {
      _indicatorLed->blink(50);
    }
}

