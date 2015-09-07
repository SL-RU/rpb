#include "hw.hpp"
#include <wiringPi.h>


GButton::GButton(int Gpin, bool buttonPressedLevel):
  pin(Gpin),
  pressLevel(buttonPressedLevel),
  pressStartTime(0)
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
  if(r == pressLevel)
    {
      if(pressStartTime == 0)
	{
	  pressStartTime = time;
	}
    }
  else
    {
      if(pressStartTime != 0)
	{
	  if(time - pressStartTime >= 200)
	    std::cout << "click" << "\n";
	  pressStartTime = 0;
	}
      else
	{
	}
    }
}

void Init_Hardware()
{
  wiringPiSetup();
}
int Add_HardwareObject(HardwareObject * ho)
{
  return 0;
}

int main()
{
  Init_Hardware();
  
  GButton *gb = new GButton(5, true);
  while(true)
    {
      gb->Update(millis());
      delay(10);
    }
}
