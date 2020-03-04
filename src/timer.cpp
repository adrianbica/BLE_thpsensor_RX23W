#include "timer.h"

Timer& Timer::GetInstance(void)
{
    static Timer  instance;
    return instance;
}

void Timer::Tick(void)
{
    mSysTimer++;
}

void Timer::ArmTimer(uint32_t numTimer)
{
    TimersTable[numTimer] = mSysTimer;
}

bool Timer::IsTimerElapsed(uint32_t numTimer, uint32_t valTimer)
{
    uint32_t      ExpectedValue, AuxValue;

    ExpectedValue = (AuxValue = TimersTable[numTimer]) + valTimer;
    if (ExpectedValue > AuxValue) {
       return((mSysTimer < AuxValue) || (mSysTimer > ExpectedValue));
    } else {
       return((mSysTimer < AuxValue) && (mSysTimer >= ExpectedValue));
    }
}

