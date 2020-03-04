/*
 * relay_module.cpp

 *
 *  Created on: Mar 1, 2020
 *      Author: adrian.bica
 */

#include "relay_module.h"
#include "r_smc_entry.h"
//#include "r_ble_relay_services.h"

typedef uint16_t	ble_status_t;

extern "C" {
	ble_status_t R_BLE_RELAY_SERVICES_GetDigital1(uint8_t *p_value);
}



relayModule::relayModule(void)
{
	// do not initialize here, this would be called before main
}

relayModule::~relayModule(void){}

relayModule& relayModule::GetInstance(void)
{
	static relayModule		instance;
	return instance;
}

void relayModule::Init(int relay)
{
	Deactivate(relay);
}

void relayModule::Deactivate(int relay)
{
	/*
	 * set the corresponding GPIO to 1 to turn off relay
	 */
	switch(relay)
	{
	case 0:
		PORT4.PODR.BYTE = _02_Pm1_OUTPUT_1 | PORT4.PODR.BYTE;
		break;
	case 1:
		PORT4.PODR.BYTE = _20_Pm5_OUTPUT_1 | PORT4.PODR.BYTE;
		break;
	case 2:
		PORT4.PODR.BYTE = _40_Pm6_OUTPUT_1 | PORT4.PODR.BYTE;
		break;
	case 3:
		PORT4.PODR.BYTE = _80_Pm7_OUTPUT_1 | PORT4.PODR.BYTE;
		break;
	default:
		break;
	}
}

void relayModule::Activate(int relay)
{
	/*
	 * set the corresponding GPIO to 0 to turn on relay
	 */
	switch(relay)
	{
	case 0:
		PORT4.PODR.BYTE = (~_02_Pm1_OUTPUT_1) & PORT4.PODR.BYTE;
		break;
	case 1:
		PORT4.PODR.BYTE = (~_20_Pm5_OUTPUT_1) & PORT4.PODR.BYTE;
		break;
	case 2:
		PORT4.PODR.BYTE = (~_40_Pm6_OUTPUT_1) & PORT4.PODR.BYTE;
		break;
	case 3:
		PORT4.PODR.BYTE = (~_80_Pm7_OUTPUT_1) & PORT4.PODR.BYTE;
		break;
	default:
		break;
	}
}


void relayModule::task(void)
{
	uint16_t		value;

	if(0 == R_BLE_RELAY_SERVICES_GetDigital1((uint8_t*)&value))
	{
		if((value & 0x0003) == 0x0001)
		{
			Activate(0);
		}
		else if((value & 0x0003) == 0x0000)
		{
			Deactivate(0);
		}
		if((value & 0x0030) == 0x0010)
		{
			Activate(1);
		}
		else if((value & 0x0030) == 0x0000)
		{
			Deactivate(1);
		}
		if((value & 0x0300) == 0x0100)
		{
			Activate(2);
		}
		else if((value & 0x0300) == 0x0000)
		{
			Deactivate(2);
		}
		if((value & 0x3000) == 0x1000)
		{
			Activate(3);
		}
		else if((value & 0x3000) == 0x0000)
		{
			Deactivate(3);
		}
	}
}






