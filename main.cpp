#include "main.hpp"


Core::Core()
{
  std::cout<<"Core: begin initialising\n";
  
  core = this;
  InitHardware();
  InitGUI();

  std::cout<<"Core: core initialized\n";
}

void Core::InitHardware()
{
  Init_Hardware();
  std::cout << "Core: hardware initialization:\n";
  Buttons = new GButton*[9];
  Leds = new GLed*[2];
  for (int i = 0; i < 9; i++) {
    Buttons[i] = new GButton(ButtonPins[i], true);
  }
  for (int i = 0; i < 2; i++) {
    Leds[i] = new GLed(LedPins[i], false);
  }
  SetIndicatorLed(Leds[1]);

  std::cout<<"Core: hardware inited\n";
  
  //start thread
  hardwareThread = new thread(Update_Hardware);
  std::cout<<"Core: hardware thread started\n";
}

void Core::InitGUI()
{
  std::cout << "Core: GUI initialization" << "\n";
  gui = new GUI;

  std::cout<<"Core: gui inited\n";
  
  //start thread
  guiThread = new thread(&GUI::Update, gui);

  std::cout<<"Core: gui thread started\n";
  
  for (int i = 0; i < 4; i++) {
    Buttons[i]->setOnClick(__hardwareeventsClickTocorefunctionsparser);
    Buttons[i]->setOnPress(__hardwareeventsPressTocorefunctionsparser);    
  }

  std::cout<<"Core: hardware events for gui inited\n";
}
void __hardwareeventsClickTocorefunctionsparser(int i)
{
  core->GuiInputHWButtonsEventsClickHander(i);
}
void __hardwareeventsPressTocorefunctionsparser(int i)
{
  core->GuiInputHWButtonsEventsPressHander(i);
}
void Core::GuiInputHWButtonsEventsClickHander(int l)
{
  INPUT_ACTIONS ia = none;
  switch (l) {
  case ButtonPins[0]: {
    ia = up;
    break;
  }
  case ButtonPins[1]: {
    ia = ok;
    break;
  }
  case ButtonPins[2]: {
    ia = down;
    break;
  }
  case ButtonPins[3]: {
    ia = description;
    break;
  }
  default: break;
  }
  gui->Input(ia);
}
void Core::GuiInputHWButtonsEventsPressHander(int l)
{
  INPUT_ACTIONS ia = none;
  switch (l) {
  case ButtonPins[0]: {
    ia = backw;
    break;
  }
  case ButtonPins[1]: {
    ia = ok;
    break;
  }
  case ButtonPins[2]: {
    ia = forw;
    break;
  }
  case ButtonPins[3]: {
    ia = set;
    break;
  }
  default: break;    
  }
  gui->Input(ia);
}

int main(int argc, char *argv[])
{
  cout<<"STARTING\n";
  cout<<"...\n";
  Core *cr = new Core();
  cout<<"Started\n";
  GUIList *gl = new GUIList(0, 9, 128, 55, cr->gui);
  cr->gui->root->AddChild(gl);
  
  string q = "";
  while (q!="q")
    {
      getline(cin, q);
      gl->AddItem(q, 1);
    }

  cr->gui->TurnOff();
  sleep(1);
  return 0;
}

