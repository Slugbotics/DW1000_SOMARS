#include <cstdint>
#include <cassert>
#include <iostream>

class RangingDevice {
public:
	RangingDevice(int32_t inactivityThreshold, int32_t (*millis)(void)) {
		_millis = millis;
		_activity = 0;
		_inactivityThreshold = inactivityThreshold;
	}

	void noteActivity() {
		_activity = _millis();
	}

	bool isInactive() {
		bool isInactive =  _millis() - _activity > _inactivityThreshold;
		if (isInactive) {
			noteActivity();
		}
		return isInactive;
	}
private:
	int32_t _activity;
	int32_t _inactivityThreshold;
	int32_t (*_millis)(void);
};

int32_t currentTimestamp = 0;

int32_t millis() {
	return currentTimestamp;
}

void testActivityIsNotInvalidatedWhenLessThanThreshold() {
	RangingDevice test(1000, millis);
	test.noteActivity();
	assert(!test.isInactive());
}

void testActivityIsInactiveWhenPastTimerThreshold() {
	RangingDevice test(1000, millis);
	test.noteActivity();
	currentTimestamp = 1001;
	assert(test.isInactive());
}

void testIsInactiveRenotesActivityWhenInactive() {
	RangingDevice test(1000, millis);
	test.noteActivity();
	currentTimestamp = 1001;
	test.isInactive(); // NB: This should have a side-effect which renotes the activity
	assert(!test.isInactive());
}

int main(void) {
	currentTimestamp = 0;
	testActivityIsNotInvalidatedWhenLessThanThreshold();
	currentTimestamp = 0;
	testActivityIsInactiveWhenPastTimerThreshold();
	currentTimestamp = 0;
	testIsInactiveRenotesActivityWhenInactive();
	std::cout << "Tests Passed" << std::endl;
	return 0;
}
