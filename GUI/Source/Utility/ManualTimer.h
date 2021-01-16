#pragma once

#include "Timer.h"

class ManualTimer : public Timer {
public:
	// Manual Timer requires manual checking of is finished and manual resetting
	ManualTimer(float timeLimit);
	// Returns true when the timer is finished
	bool IsFinished();
};