#include <thread>
#include "gui.hpp"

GUIElement::GUIElement(int X, int Y, GUI* Gui):
  x(X),
  y(Y),
  gui(Gui),
  parent(0)
{
  id = gui->AddElement(this);
}

void GUIElement::SetParent(GUIParent *gparent)
{
  if(parent != 0)
    parent->RemChild(this);
  if(gparent != 0)
    gparent->AddChild(this);
  
  parent = gparent;
}

GUIParent* GUIElement::GetParent()
{
  return parent;
}

void GUIElement::Update(ArduiPi_OLED *oled, int time)
{
}

GUI::GUI():
  root(new GUIParent(0,0,this)),
  oled(new ArduiPi_OLED()),
  last_id(0)
{
  oled->init(OLED_I2C_RESET,3);
  oled->begin();
  cls();
}

int GUI::AddElement(GUIElement *el)
{
  return last_id++;
}

void GUI::cls()
{
  oled->clearDisplay();
  oled->display();
}

void GUI::Update()
{
  while(1)
    {
      oled->clearDisplay();
      root->Update(oled, millis());
      oled->display();
    }
}

GUIParent::GUIParent(int X, int Y, GUI* Gui):
  GUIElement(X, Y, Gui),
  children(new list<GUIElement*>)
{
}

void GUIParent::AddChild(GUIElement* el)
{
  if(el->GetParent() == this)
    return;
  for(GUIElement *l : *children)
    {
      if(el == l)
	return;
    }
  children->push_front(el);
  el->SetParent(this);
}

void GUIParent::RemChild(GUIElement* ch)
{
  children->remove(ch);
}
void GUIParent::Update(ArduiPi_OLED *oled, int time)
{
  for(GUIElement *l : *children)
    {
      l->Update(oled, time);
    }
}

GUIRect::GUIRect(int X, int Y, int W, int H, GUI* Gui):
  GUIElement(X, Y, Gui),
  w(W),
  h(H)
{
}
void GUIRect::Update(ArduiPi_OLED *oled, int time)
{
  gui->oled->drawRect(x, y, w, h, 1);
}


GUILable::GUILable(int X, int Y, int W, int H, GUI* Gui, string str):
  GUIElement(X, Y, Gui),
  text(str),
  w(W),
  h(H),
  curPos(0),
  scrollSpeed(200),
  lastScrollTime(0),
  lastString("")
{
  
}

void GUILable::Update(ArduiPi_OLED *oled, int time)
{
  int maxLen = w / 6;
  if(text.length() <= maxLen)
    {
      gui->oled->setTextSize(1);
      gui->oled->setTextColor(WHITE, BLACK);
      gui->oled->setCursor(x, y);
      gui->oled->print(text);
      return;
    }
  if(maxLen <=1) //ибо нефиг
    return;
  if(curPos >= text.length())
    {
      curPos = 0;
    }

  string s = "";
  if(time - lastScrollTime >= scrollSpeed)
    {
      lastString = "";
      lastScrollTime = time;
      curPos++;
    }
  else
    {
      s=lastString;
    }
  
  if(lastString == "")
    {
      s.push_back(text[curPos]);

      int f = 1, b = 1, len = text.length();
      bool fb = true;
      while (s.length() < maxLen) { //this sucks. But i'm just wanna sleep
	if(fb)
	  {
	    if(b+curPos < len)
	      s.push_back(text[curPos+b]);
	    b++;
	  }
	else
	  {
	    if(curPos - f >= 0)
	      s.insert(0, 1, text[curPos-f]);
	    f++;
	  }
	fb=!fb;
      }
    }

  gui->oled->setTextSize(1);
  gui->oled->setTextColor(WHITE, BLACK);
  gui->oled->setCursor(x, y);
  gui->oled->print(s);
  
}

void GUILable::SetText(string str)
{
  text = str;
  curPos = 0;
}
