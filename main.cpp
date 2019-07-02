#include "pch.h"
#include <iostream>
#include <list>
#include <conio.h>


class MenuItemB {
public:
	virtual void inc() {};
	virtual void dec() {};
	virtual const char * desc() { return "Haha"; };
	virtual const char * svalue() { return "---"; };
};

template<typename T>
class MenuItem : public MenuItemB {
protected:
	char _desc[80];
	T value;
public:
	MenuItem(const char *desc) {
		strncpy_s(_desc, desc, 80);
	}
	const char * desc() override { return _desc; };
	virtual	void set(T val) { value = val; }
};

class FloatByHalf : public MenuItem<float> {
	char buffer[12];
public:
	FloatByHalf(const char *desc, float val) :MenuItem(desc) {
		value = val;
	}
	void inc() override {
		value += 0.2f;
	}
	void dec() override {
		value -= 0.2f;
	}
	//Has a little buffer, you should use it while you got it.....
	const char *svalue() override {
		snprintf(buffer, 12, "%2.1f", value);
		return buffer;
	}
};

class FloatBy1 : public FloatByHalf {
public:
	FloatBy1(const char *desc, float val) :FloatByHalf(desc,val) {
	}
	void inc() override {
		value += 1;
	}
	void dec() override {
		value -= 1;
	}
};

class OnOff : public MenuItem<bool> {
public:
	OnOff(const char *desc, bool val) :MenuItem(desc) {
		value = val;
	}
	void inc() override {
		value = !value;
	}
	void dec() override {
		value = !value;
	}
	const char *svalue() override {
		if (value) return "On";
		return "Off";
	}

};

class TriState : public MenuItem<uint8_t> {
public:
	TriState(const char *desc, uint8_t val) : MenuItem(desc) {
		value = val;
	}
	void inc() override {
		value++;
		if (value >= 3) value = 0;
	}
	void dec() override {
		value--;
		if (value < 0) value = 2;
	}
	const char *svalue() override {
		if (value == 0) return "Auto";
		if (value == 1) return "On";
		if (value == 2) return "Off";
	}
};

int main()
{
	std::list<MenuItemB*> list;

	FloatByHalf mTDay("Day",77.0f);
	FloatByHalf mNight("Night",65.0f);
	OnOff mSystemOn("System", true);
	OnOff mVacayMode("Vacation", false);
	OnOff mPumpMode("Pump Mode", 0);
	OnOff mFanMode("Fan Mode", 0);
	
	list.push_back(&mTDay);
	list.push_back(&mNight);	
	list.push_back(&mSystemOn);
	list.push_back(&mVacayMode);
	list.push_back(&mPumpMode);
	list.push_back(&mFanMode);

	auto ip = list.begin();
	std::cout << "Test Menu\n";
	bool done = false;
	while (!done) {
		std::cout << (*ip)->desc() << ":" << (*ip)->svalue() << std::endl;
 		char cc = _getch();
		std::cout << "Pressed: " << cc  << "\n";
  		if (cc == 'a') (*ip)->dec();
		if (cc == 'd') (*ip)->inc();
		if (cc == 's') {
			ip++;
			if (ip == list.end()) ip = list.begin();
		}
		if (cc == ' ') done = true;
	}
	return 0;
}
