#ifndef hw
#define hw

#include <iostream>
#include <wiringPi.h>
#include <list>


class HardwareObject
{
public:
  virtual void Update(unsigned int) = 0;
  int GetID(){return id;}
protected:
  int id;
};


class GButton : public HardwareObject
{
public:
  GButton(int, bool); //!< pin - gpio pin, where button connected
                      //!< buttonPressedLevel - a gpio signal level, on which button will be pressed
  ~GButton();
  void Update(unsigned int);
  void setOnClick(void (*func)(int)); //!< set function, which will be executed on button click
				      //!< click - short button press and release
				      //!< MinClickDur <= current_press_duration < MinPressDur
  void setOnPress(void (*func)(int)); //!< set function, which will be executed on button press
				      //!< press - long button press and release
				      //!< current_press_duration >= MinPressDur
  int MinClickDur; //!< CLICK when: MinClickDur <= current_press_duration < MinPressDur
		   //!< milliseconds
  int MinPressDur; //!< PRESS when: MinPressDur <= current_press_duration
		   //!< milliseconds

private:
  int pin;          //!< gpio pin number, where button connected
  bool pressLevel;  //!< gpio signal level, on which button will be pressed
  unsigned int pressStartTime;
  void (*clickEvent)(int);
  void (*pressEvent)(int);
};

class GLed : public HardwareObject
{
public:
  GLed(int, bool);//!< pin - gpio pin, where led connected
                  //!< ledLightsLevel - a gpio signal level, on which led lights
  ~GLed();
  void set(bool); //!< set current led state
		  //!< true - led lights, false - led off
  void blink(int);//!< light led for a certain time in milliseconds
  void Update(unsigned int);
  
private:
  int pin;
  bool lightsLevel;
  bool curState;
  int blinkDuration;
  unsigned int blinkStartTime;
};

void Init_Hardware();
extern std::list<HardwareObject*> *UpdateHardwareObjectsList;
const int HardwareUpdatesDelay = 10;
void Update_Hardware();
int Add_HardwareObject(HardwareObject*);
void Remove_HardwareObject(HardwareObject*);
void Remove_HardwareObject(int);
extern GLed *_indicatorLed;
void SetIndicatorLed(GLed*);
void IndicateClick();


#endif
