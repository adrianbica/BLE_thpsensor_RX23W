#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdint.h>

#define SENSOR_TIMER            0
#define NEXT_AVAILABLE_TIMER    SENSOR_TIMER + 1
#define MAX_TIMERS              NEXT_AVAILABLE_TIMER

class Timer {
private:
    uint32_t            mSysTimer;
    uint32_t	 		TimersTable[MAX_TIMERS];
    Timer(void) {mSysTimer = 0;};
    Timer(const Timer&);
    ~Timer(void){};
public:
    static Timer& GetInstance(void);
    void Tick(void);
    void ArmTimer(uint32_t numTimer);
    bool IsTimerElapsed(uint32_t numTimer, uint32_t valTimer);
};
#endif

