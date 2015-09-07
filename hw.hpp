#include <iostream>
#include <wiringPi.h>

class HardwareObject
{
public:
  virtual void Update(unsigned int) = 0;

protected:
  int id;
};


class GButton : public HardwareObject
{
public:
  GButton(int, bool);
  ~GButton();
  void Update(unsigned int);

private:
  int pin;
  bool pressLevel;
  unsigned int pressStartTime;
  void (*click)(int);
};


void Init_Hardware();
void Update_Hardware(unsigned int);
int Add_HardwareObject(HardwareObject*);
void Remove_HardwareObject(HardwareObject*);
void Remove_HardwareObject(int);
