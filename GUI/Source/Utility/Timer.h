#pragma once

class Timer {
protected:
	float m_CurrentTime;
	float m_TimeLimit;

	bool m_IsRunning;
public:
	// Use AutomaticTimer or ManualTimer, do not construct this class directly
	Timer(float timeLimit);

	// Resets the timer's progress
	void Reset();

	// Starts the timer's counting
	void Start();
	// Stops the timer from progressing
	void Stop();

	// Sets the progress of the timer
	void SetCurrentTime(float currentTime);
	// Sets the timer limit
	void SetTimeLimit(float timeLimit);
	// Sets the completion progress between 0 and 1 of the timer
	void SetProgress(float progress);
	// Equivalent of calling start/stop
	void SetIsRunning(bool isRunning);

	// Gets the time progressed in the timer
	float GetCurrentTime();
	// Gets the timer's limit
	float GetTimeLimit();
	// Returns the completion progress between 0 and 1 of the timer
	float GetProgress();
	// Returns true if the timer is running
	bool GetIsRunning();

	virtual void Update(float deltaTime);
};