#include "hw.hpp"
#include <thread>
#include <iostream>
#include "gui.hpp"

using namespace std;

constexpr int ButtonPins[] = {13, 12, 14, 11, 5, 2, 0, 4, 3};
const int LedPins[] = {10, 6};



//class App;
class Core;


//костыли:
Core *core;
void __hardwareeventsClickTocorefunctionsparser(int i);
void __hardwareeventsPressTocorefunctionsparser(int i);


class Core
{
public:
  Core();

  GUI     *gui;
  GButton **Buttons;
  GLed    **Leds;
  //  App     *curApp;
  void GuiInputHWButtonsEventsClickHander(int l);
  void GuiInputHWButtonsEventsPressHander(int l);
  
private:
  void InitHardware();
  void InitGUI();

  thread *hardwareThread;
  thread *guiThread;
};
