
#include "pch.h"
#include <iostream>
#include <list>
#include <conio.h>
#include <functional>


class MenuItemB {
public:
	std::function<void()> onExit;
	virtual void inc() {};
	virtual void dec() {};
	virtual const char * desc() { return "Haha"; };
	virtual const char * svalue() { return "---"; };
	virtual void changed() { };
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
	virtual typename T get() { return value; }
};

class FloatByHalf : public MenuItem<float> {
	char buffer[12];
public:
	FloatByHalf(const char *desc, float val) :MenuItem(desc) {
		value = val;
	}
	void inc() override {
		value += 0.5f;

	}
	void dec() override {
		value -= 0.5f;

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

class YesNo : public OnOff {
public:
	YesNo(const char *desc, bool val) :OnOff(desc, val) {}
	
	const char *svalue() override {
		if (value) return "Yes";
		return "No";
	}

	void changed() override {
		if (value) {
			if (onExit != NULL) onExit();
		}
	}

};

class TriState : public MenuItem<uint8_t> {
public:
	TriState(const char *desc, uint8_t val) : MenuItem(desc) {
		value = val;
	}
	void inc() override {
		value++;
		if (value > 2) value = 0;
	}
	void dec() override {
		if (value > 0) {
			value--;
		}
		else {
			value = 2;
		}
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
	bool done = false;
	FloatByHalf mTDay("Day",77.0f);
	FloatByHalf mNight("Night",65.0f);
	OnOff mSystemOn("System", true);
	OnOff mVacayMode("Vacation", false);
	TriState mPumpMode("Pump Mode", 0);
	TriState mFanMode("Fan Mode", 0);
	YesNo mSaveEEPROM("Save to EEPROM", false);
	YesNo mSave("Exit", false);

	mSave.onExit = [&done] {
		done = true;
	};
	
	list.push_back(&mTDay);
	list.push_back(&mNight);	
	list.push_back(&mSystemOn);
	list.push_back(&mVacayMode);
	list.push_back(&mPumpMode);
	list.push_back(&mFanMode);
	list.push_back(&mSaveEEPROM);
	list.push_back(&mSave);

	auto ip = list.begin();
	std::cout << "Test Menu\n";
	
	while (!done) {
		MenuItemB *menu = (*ip);
		std::cout << menu->desc() << ":" << menu->svalue() << std::endl;
 		char cc = _getch();
		std::cout << "Pressed: " << cc  << "\n";
  		if (cc == 'a') menu->dec();
		if (cc == 'd') menu->inc();
		if (cc == 's') {
			menu->changed();
			ip++;
			if (ip == list.end()) ip = list.begin();
		}

		if (cc == ' ') {
			
			done = true;
		}
	}
	std::cout << "Menu Exit\n";
	if (mSaveEEPROM.get()) {
		std::cout << "Saving to EEPROM\n";
	}
	return 0;
}
