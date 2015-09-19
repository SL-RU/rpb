#include <thread>
#include "gui.hpp"

GUIElement::GUIElement(int X, int Y, GUI* Gui):
  x(X),
  y(Y),
  tab_priority(0),
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

int GUIElement::getX()
{
  if(parent != 0)
    return x + parent->getX();
  else
    return x;
}
int GUIElement::getY()
{
  if(parent != 0)
    return y + parent->getY();
  else
    return y;
}

void GUIElement::Update(ArduiPi_OLED *oled, int time){}
void GUIElement::Input(INPUT_ACTIONS action){}
void GUIElement::FocusElement(){}
void GUIElement::ReleaseFocus(){}

GUI::GUI():
  root(new GUIParent(0,0,this)),
  oled(new ArduiPi_OLED()),
  last_id(0),
  turnOn(true),
  tablist(new list<GUIElement*>)
{
  oled->init(OLED_I2C_RESET,3);
  oled->begin();
  cls();
}

int GUI::AddElement(GUIElement *el)
{
  return last_id++;
}

void GUI::AddElementToTabList(GUIElement*el)
{
  tablist->push_back(el);
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
      if(turnOn)
	{
	  oled->clearDisplay();
	  root->Update(oled, millis());
	  oled->display();
	}
    }
}

void GUI::TurnOn()
{
  turnOn = true;
}
void GUI::TurnOff()
{
  turnOn = false;
  cls();
}

void GUI::Input(INPUT_ACTIONS action)
{
  if(tablist->size()>0)
    {
      tablist->front()->Input(action);
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
  gui->oled->drawRect(getX(), getY(), w, h, 1);
}


GUILable::GUILable(int X, int Y, int W, int H, GUI* Gui, string str):
  GUIElement(X, Y, Gui),
  text(str),
  w(W),
  h(H),
  curPos(0),
  scrollSpeed(200),
  lastScrollTime(0),
  lastString(""),
  bgc(0),
  brc(0),
  txtc(1)
{
  
}

void GUILable::Update(ArduiPi_OLED *oled, int time)
{
  int maxLen = w / 6;
  if(text.length() <= maxLen)
    {
      gui->oled->drawRect(getX(), getY(), w, h, brc);
      oled->fillRect(getX(), getY(), w, h, bgc);
      gui->oled->setTextSize(1);
      gui->oled->setTextColor(txtc, bgc);
      gui->oled->setCursor(getX(), getY());
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

  gui->oled->drawRect(getX(), getY(), w, h, brc);
  oled->fillRect(getX(), getY(), w, h, bgc);
  gui->oled->setTextSize(1);
  gui->oled->setTextColor(txtc, bgc);
  gui->oled->setCursor(getX(), getY());
  gui->oled->print(s);
  
}


string GUILable::SetText(string str)
{
  str = convert_encoding(str, "UTF-8", "CP1251");
  text = str;
  curPos = 0;
  return str;
}

string GUILable::SetText(string str, bool convert)
{
  if(convert)
    str = convert_encoding(str, "UTF-8", "CP1251");
  text = str;
  curPos = 0;
  return str;
}

void GUILable::SetBGColor(short c)
{
  bgc = c;
}
void GUILable::SetBorderColor(short c)
{
  brc = c;
}
void GUILable::SetTextColor(short c)
{
  txtc = c;
}



GUIList::GUIList(int X, int Y, int W, int H, GUI* Gui):
  GUIParent(X, Y, Gui),
  data(new vector<GUIListItem*>),
  w(W),
  h(H),
  curPos(0),
  lables(new vector<GUILable*>)
{
  tab_priority = 1;
  gui->AddElementToTabList(this);
  init_children();
}

int GUIList::AddItem(string s, int i)
{
  data->push_back(new GUIListItem(s, i));
}

void GUIList::init_children()
{
  linesCount = h/9;
  std::cout << "GUIList lines: " << linesCount << "\n";
  GUILable *lb;
  for (int i = 0; i < linesCount; i++) {
    lb = new GUILable(0, i*9, w, 9, gui, "");
    lb->SetParent(this);
    lables->push_back(lb);
  }
}

void GUIList::Update(ArduiPi_OLED* oled, int time)
{

  oled->drawLine(getX()+w-1, getY(), getX()+w-1, h+getY(), 1);
  if(lables->size() == 0)
    return;
  if(data->size() == 0)
    return;
  int f = 1, b = 1, cu = 1, ds= data->size(), seli = 0;
  if(curPos < 0)
    {
      curPos = 0;
    }
  if(curPos > data->size())
    {
      curPos = data->size();
    }

  vector<GUIListItem*> da;
  vector<GUIListItem*>::iterator ite;
  da.push_back(data->at(curPos));
  for (int i = 0; i < lables->size() - 1; i++)
    {
      if(cu)
	{
	  if(curPos + f < ds)
	    {
	      da.push_back(data->at(curPos + f));
	      f++;
	    }
	  else
	    {
	      if(curPos - b >= 0)
		{
		  i--;
		}
	      else
		{
		  da.push_back(0);
		}
	    }
	  cu = 0;
	}
      else
	{
	  if(curPos - b >= 0)
	    {
	      seli ++;
	      ite = da.begin();
	      da.insert(ite, data->at(curPos - b));
	      b++;
	    }
	  else
	    {
	      if(curPos + f < ds)
		{
		  i--;
		}
	      else
		{
		  da.push_back(0);
		}
	    }
	  cu = 1;    
	}
    }

  int i = 0;
  for (GUILable *lb : *lables)
    {
      if(da[i] != 0)
	{
	  if(lb->GetText() != da[i]->txt)
	    {
	      da[i]->txt = lb->SetText(da[i]->txt, !da[i]->converted);
	      da[i]->converted = true;
	    }
	}
      if(seli == i)
	{
	  lb->SetBGColor(1);
	  lb->SetTextColor(0);
	  lb->SetBorderColor(1);
	}
      else
	{
	  lb->SetBGColor(0);
	  lb->SetTextColor(1);
	  lb->SetBorderColor(0);
	} 
      lb->Update(oled, time);
      i++;
    }

  oled->drawLine(getX()+w-1, getY(), getX()+w-1, getY()+h, 1);
  if(lables->size() < data->size())
    {
      oled->drawLine(getX()+w-2, getY(), getX()+w-2, getY()+h, 0);
      int po = h * (curPos) / data->size();
      oled->drawLine(getX()+w-2, getY() + po, getX()+w-2, getY()+h, 1);
    }
}

void GUIList::Input(INPUT_ACTIONS ia)
{
  if(ia == up)
    {
      curPos--;
      if(curPos < 0)
	{
	  curPos = data->size() - 1;
	}
    }
  if(ia == down)
    {
      curPos++;
      if(curPos >= data->size())
	{
	  curPos = 0;
	}
    }
  if(ia == ok)
    {
      if(onClick != 0 && data->size() > 0)
	onClick(data->at(curPos)->id, data->at(curPos)->txt);
    }
}

GUIListItem::GUIListItem():
  txt(""),
  id(0),
  priority(0),
  converted(false)
{
}

GUIListItem::GUIListItem(string t, int i):
  txt(t),
  id(i),
  priority(0),
  converted(false)
{
}

GUIListItem::GUIListItem(string t, int i, int prior):
  txt(t),
  id(i),
  priority(prior),
  converted(false)
{
}


GUIBitmap::GUIBitmap(int X, int Y, int W, int H, unsigned char* BITMap, GUI* Gui):
  GUIElement(X,Y,Gui),
  bitmap(BITMap),
  w(W),
  h(H)
{
}
void GUIBitmap::Update(ArduiPi_OLED* oled, int time)
{
  oled->drawBitmap(x, y, bitmap, w, h, 1);
}



string convert_encoding(const string& data, const string& from, const string& to)
{
   if (data.empty())
   {
      return string();
   }
   iconv_t convert_hnd = iconv_open(to.c_str(), from.c_str());
   if (convert_hnd == (iconv_t)(-1))
     {
       cout << "error: unable to create convertion descriptor\n";
       return "";
     }
 
   char* in_ptr = const_cast<char*>(data.c_str());
   size_t in_size = data.size();
   vector<char> outbuf(6 * data.size());
   char* out_ptr = &outbuf[0];
   size_t reverse_size = outbuf.size();
 
   size_t result = iconv(convert_hnd, &in_ptr, &in_size, &out_ptr, &reverse_size);
   iconv_close(convert_hnd);
   if (result == (size_t)(-1))
   {
     cout << "error: unable to convert\n";
     //return "";
   }
   return string(outbuf.data(), outbuf.size() - reverse_size);
}
