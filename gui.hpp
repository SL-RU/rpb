#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <iconv.h>
#include "ardupi_oled/ArduiPi_OLED_lib.h"
#include "ardupi_oled/ArduiPi_OLED.h"
#include "ardupi_oled/Adafruit_GFX.h"


enum INPUT_ACTIONS //!< GUI actions from input devices
  {
    none,
    up,				//!< up
    down,			//!< down
    ok,				//!< okay click(select, apply, do smth, etc...)
    description,		//!< request for description
    set,			//!< change value of smth or request changing form
    forw,			//!< Interface action.Tab to next GUIElement
    backw			//!< Interface action.Tab to previous GUIElement
  };


using namespace std;

class GUIElement;
class GUI;
class GUIParent;

class GUIElement
{
public:
  GUIElement(int X, int Y, GUI *Gui);
  virtual void Update(ArduiPi_OLED *oled, int time);
  virtual void Input(INPUT_ACTIONS action);
  void FocusElement();
  void ReleaseFocus();
  GUIParent* GetParent();
  void SetParent(GUIParent*);  //!< change parent and call call old parent's remove child and new parent's add child functions.
  int x, y;
  int getX();
  int getY();
  unsigned int tab_priority; //! if tab_priority == 0, then element is not in tab queue.
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
  void AddElementToTabList(GUIElement*);
  void RemoveElement(GUIElement*);  //!< Removes element from list
  void Update();
  void cls();
  void TurnOn();
  void TurnOff();
  void Input(INPUT_ACTIONS action);
  GUIParent *root;
  ArduiPi_OLED *oled;
private:
  int last_id;
  bool turnOn;
  list<GUIElement*> *tablist;
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
  string SetText(string);
  string SetText(string, bool);
  void SetBGColor(short);
  void SetTextColor(short);
  void SetBorderColor(short);
  string GetText(){return text;}
  
private:
  string text;
  int w, h;

  int curPos,
    scrollSpeed,
    lastScrollTime;
  string lastString;
  short bgc, brc, txtc;
};

class GUIRect : public GUIElement
{
public:
  GUIRect(int X, int Y, int W, int H, GUI *Gui);
  void Update(ArduiPi_OLED *oled, int time);
  int w, h;
};

class GUIListItem
{
public:
  GUIListItem();
  GUIListItem(string t, int i);
  GUIListItem(string t, int i, int prior);
  string txt;
  int id, priority;
  bool converted;
};

class GUIList : public GUIParent
{
public:
  GUIList(int X, int Y, int W, int H, GUI *Gui);
  void Update(ArduiPi_OLED *oled, int time);
  int AddItem(string,int);
  int RemoveItem(int);
  void ChangeItem(int, string);
  void SetOnClickAction(void (*func)(int,string));
  void Input(INPUT_ACTIONS ia);
  void SetOnDescriptionAction(void (*func)(int,string));
  vector<GUIListItem*> *data;
private:
  int w, h, linesCount,
    curPos;
  vector<GUILable*> *lables;
  void (*onClick)(int,string);
  void (*onDescription)(int,string);
  void init_children();
};

class GUIBitmap : public GUIElement
{
public:
  GUIBitmap(int X, int Y, int W, int H, unsigned char *BITMap, GUI *Gui);
  void Update(ArduiPi_OLED *oled, int time);
  void SetBitmap(int W, int H, unsigned char *BITMap);
private:
  unsigned char *bitmap;
  int w, h;
};

string convert_encoding(const string& data, const string& from, const string& to);
