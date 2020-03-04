#ifndef __THP_SENSOR_H__
#define __THP_SENSOR_H__

#include "sensor.h"

class CALIBRATION {
public:
    uint16_t        dig_T1;
    int16_t         dig_T2;
    int16_t         dig_T3;
    uint16_t        dig_P1;
    int16_t         dig_P2;
    int16_t         dig_P3;
    int16_t         dig_P4;
    int16_t         dig_P5;
    int16_t         dig_P6;
    int16_t         dig_P7;
    int16_t         dig_P8;
    int16_t         dig_P9;
    uint8_t         dig_H1;
    int16_t         dig_H2;
    uint8_t         dig_H3;
    int16_t         dig_H4;
    int16_t         dig_H5;
    uint8_t         dig_H6;
};


class THPSensor: public sensor
{
private:
    int16_t         Temperature;
    uint32_t        Pressure;
    uint16_t        Humidity;

    uint8_t			TemperatureNotifyStatus;
    uint8_t			PressureNotifyStatus;
    uint8_t			HumidityNotifyStatus;

    CALIBRATION     Calibr;
    void checkAnswerTimeout(void);
    void calibrate(void);
    void configure(void);
    void measure(void);
public:
    void init(void);
    THPSensor(void);
    void task(void);
    void setTemperatureNotifyStatus(uint8_t v) { TemperatureNotifyStatus = v; }
    void setPressureNotifyStatus(uint8_t v) { PressureNotifyStatus = v; }
    void setHumidityNotifyStatus(uint8_t v) { HumidityNotifyStatus = v; }
};

#endif

