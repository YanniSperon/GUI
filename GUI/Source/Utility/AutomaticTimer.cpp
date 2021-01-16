#include "AutomaticTimer.h"

AutomaticTimer::AutomaticTimer(float timeLimit, bool isRecurring, const std::function<void()>& callback)
	: Timer(timeLimit), m_IsRecurring(isRecurring), m_CallbackFunction(callback)
{

}

bool AutomaticTimer::GetIsRecurring()
{
	return m_IsRecurring;
}

void AutomaticTimer::SetIsRecurring(bool isRecurring)
{
	m_IsRecurring = isRecurring;
}

void AutomaticTimer::Skip()
{
	m_CurrentTime = m_TimeLimit;
}

void AutomaticTimer::Update(float deltaTime)
{
	if (m_IsRunning) {
		m_CurrentTime += deltaTime;
		if (m_CurrentTime >= m_TimeLimit) {
			m_CallbackFunction();
			m_CurrentTime = 0.0f;
			m_IsRunning = m_IsRecurring;
		}
	}
}

void AutomaticTimer::CallFunction()
{
	m_CallbackFunction();
}
