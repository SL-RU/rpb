#include "hw.hpp"
#include <thread>
#include <iostream>
#include <clocale>
#include "gui.hpp"


void Init();

GUI *gui;
GUIRect *gr;

void prt(int p)
{
  std::cout << p << "\n";
  gr->x+=3;
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


  gui = new GUI;
  gr = new GUIRect(0, 20, 10, 40, gui);
  gui->root->AddChild(gr);
  
  thread th = thread(&GUI::Update, gui);
  
  thread thr(Update_Hardware); 


  GUILable gl = GUILable(0,0,128,8,gui,"SL_RU");
  gui->root->AddChild(&gl);
  string q = "вйцукенгшщзххъфывапролджэячсмитьбю.";
  while (q!="q")
    {
      cout << q;
      gl.SetText(q);
      getline(cin, q);
    }
  gui->cls();
  gui->root=0;
  sleep(5);
  
  return 0;
}


void Init()
{
  Init_Hardware();
}
