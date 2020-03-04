#ifndef __SENSOR_H__
#define __SENSOR_H__

typedef enum {
    STATE_INIT = 0,
    STATE_READ_CALIBR,
    STATE_CONFIGURE,
    STATE_IDLE,
    STATE_ERROR
} SENSOR_STATE;

class sensor
{
private:
    virtual void calibrate(void) = 0;
    virtual void measure(void) = 0;
protected:
    SENSOR_STATE    state;
    uint8_t         sub_state;
public:
    sensor(void) {state = STATE_INIT, sub_state = 0;};
    virtual ~sensor(void){};
    virtual void task(void) = 0;
};

#endif

