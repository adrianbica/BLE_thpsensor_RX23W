/***********************************************************************
*
*  FILE        : RX23WPeripheral.c
*  DATE        : 2020-02-21
*  DESCRIPTION : Main Program
*
*  NOTE:THIS IS A TYPICAL EXAMPLE.
*
***********************************************************************/
#include "r_smc_entry.h"
#include "r_cmt_rx_if.h"
#include "interface.h"

extern void app_main(void);

void main(void)
{
	uint32_t	ch;

	R_Config_PORT_Create();
    R_CMT_CreatePeriodic(1000UL, Tick, &ch);
	R_Config_RIIC0_Create();
	R_Config_RIIC0_Start();

	InitSensorTask();
	app_main();
}
