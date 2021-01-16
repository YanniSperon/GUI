#include "Timer.h"

Timer::Timer(float timeLimit)
    : m_CurrentTime(0.0f), m_TimeLimit(timeLimit), m_IsRunning(true)
{

}

void Timer::Reset()
{
    m_CurrentTime = 0.0f;
    m_IsRunning = true;
}

void Timer::Start()
{
    if (m_CurrentTime >= m_TimeLimit) {
        m_CurrentTime = 0.0f;
    }
    m_IsRunning = true;
}

void Timer::Stop()
{
    m_IsRunning = false;
}

void Timer::SetIsRunning(bool isRunning)
{
    m_IsRunning = isRunning;
}

void Timer::SetCurrentTime(float currentTime)
{
    m_CurrentTime = currentTime;
}

void Timer::SetTimeLimit(float timeLimit)
{
    m_TimeLimit = timeLimit;
}

void Timer::SetProgress(float progress)
{
    m_CurrentTime = m_TimeLimit * progress;
}

float Timer::GetCurrentTime()
{
    return m_CurrentTime;
}

float Timer::GetTimeLimit()
{
    return m_TimeLimit;
}

float Timer::GetProgress()
{
    return m_CurrentTime/m_TimeLimit;
}

bool Timer::GetIsRunning()
{
    return m_IsRunning;
}

void Timer::Update(float deltaTime)
{
    if (m_IsRunning) {
        m_CurrentTime += deltaTime;
    }
}
