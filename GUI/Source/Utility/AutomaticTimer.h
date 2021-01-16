#pragma once

#include "Timer.h"

#include <functional>

class AutomaticTimer : public Timer {
private:
	std::function<void()> m_CallbackFunction;
	bool m_IsRecurring;
public:
	// Automatic Timer automatically calls the callback when it finishes and starts again if it is recurring
	AutomaticTimer(float timeLimit, bool isRecurring, const std::function<void()>& callback);
	
	// Returns true if the timer is recurring
	bool GetIsRecurring();

	// Sets whether or not the timer should automatically restart
	void SetIsRecurring(bool isRecurring);

	// Skips the timer to the end and calls the callback function, repeats if the timer is recurring
	void Skip();

	// Calls the callback function without affecting the timer
	void CallFunction();

	virtual void Update(float deltaTime) override;
};