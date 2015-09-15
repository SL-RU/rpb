#include <iostream>
#include <string>
#include <list>
#include "ardupi_oled/ArduiPi_OLED_lib.h"
#include "ardupi_oled/ArduiPi_OLED.h"
#include "ardupi_oled/Adafruit_GFX.h"

using namespace std;

class GUIElement;
class GUI;
class GUIParent;

class GUIElement
{
public:
  GUIElement(int X, int Y, GUI *Gui);
  virtual void Update(ArduiPi_OLED *oled, int time);
  GUIParent* GetParent();
  void SetParent(GUIParent*);  //!< change parent and call call old parent's remove child and new parent's add child functions.
  int x, y;
protected:
  GUI *gui;
  GUIParent *parent;
  int id;
};

class GUI
{
public:
  GUI();
  int AddElement(GUIElement*);      //!< Gives id for element and adds to elements list
  void RemoveElement(GUIElement*);  //!< Removes element from list
  void Update();
  void cls();
  GUIParent *root;
  ArduiPi_OLED *oled;
private:
  int last_id;
};

class GUIParent: public GUIElement
{
public:
  GUIParent(int X, int Y, GUI *Gui);
  void AddChild(GUIElement *el); //!< Add child to children list and set child's parent
  void RemChild(GUIElement *ch); //!< ONLY REMOVE CHILD FROM CHILDREN LIST
  void Update(ArduiPi_OLED *oled, int time);

private:
  list<GUIElement*> *children;
};

class GUILable : public GUIElement
{
public:
  GUILable(int X, int Y, int W, int H, GUI *Gui, string str);
  void Update(ArduiPi_OLED *oled, int time);
  void SetText(string);
  void GetText();
  
private:
  string text;
  int w, h;

  int curPos,
    scrollSpeed,
    lastScrollTime;
  string lastString;
};

class GUIRect : public GUIElement
{
public:
  GUIRect(int X, int Y, int W, int H, GUI *Gui);
  void Update(ArduiPi_OLED *oled, int time);
  int w, h;
};


