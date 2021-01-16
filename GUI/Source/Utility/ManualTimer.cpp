#include "ManualTimer.h"

ManualTimer::ManualTimer(float timeLimit)
    : Timer(timeLimit)
{

}

bool ManualTimer::IsFinished()
{
    return m_CurrentTime >= m_TimeLimit;
}
