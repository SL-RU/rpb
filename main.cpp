#include "hw.hpp"
//#include <iostream>
#include <thread>


void Init();

void prt(int p)
{
  std::cout << p << "\n";
}

int main(int argc, char *argv[])
{
  Init();
  
  GButton *Buttons[9];
  int ButtonPins[] = {13, 12, 14, 11, 5, 2, 0, 4, 3};
  for (int i = 0; i < 9; i++) {
    Buttons[i] = new GButton(ButtonPins[i], true);
  }
  GLed *Leds[2];
  int LedPins[] = {10, 6};
  for (int i = 0; i < 2; i++) {
    Leds[i] = new GLed(LedPins[i], false);
  }
  SetIndicatorLed(Leds[1]);

  Buttons[4]->setOnClick(prt);
  Leds[0]->set(true);
  
  std::thread thr(Update_Hardware);
  thr.join();
  return 0;
}


void Init()
{
  Init_Hardware();
}
