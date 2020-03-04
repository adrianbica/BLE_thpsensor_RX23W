/*
 * interface.cpp
 *
 *  Created on: Feb 18, 2020
 *      Author: adrian.bica
 */

#include "timer.h"

#include "THPSensor.h"
#include "i2c_bus.h"
//#include "ServoMotor.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "r_smc_entry.h"
#include "relay_module.h"

void abort(void)
{
	while(1);
}

static THPSensor* 	thp_sensor;

/*
 * Callback function called in 1 msec interrupt
 * Since file app_main.c is auto generated, will provide here updates to and from BLE database
 */
void Tick(void* data)
{
	Timer::GetInstance().Tick();
	thp_sensor->task();
	relayModule::GetInstance().task();
}

/*
 * Callback function, called when a I2C transaction is completed
 */
void CompleteI2CTransfer(void)
{
	i2c_bus::GetInstance().complete_transfer();
}

void InitSensorTask(void)
{
	thp_sensor = new THPSensor();
	Timer::GetInstance().ArmTimer(SENSOR_TIMER);
}

void InitRelays(void)
{
	for(int i = 0; i < NB_RELAYS; i++)
	{
		relayModule::GetInstance().Init(i);
	}
}

#ifdef __cplusplus
}
#endif

