#include <stdint.h>
#include "i2c_bus.h"


i2c_bus::i2c_bus(void)
{
}

i2c_bus::~i2c_bus(void)
{
}

i2c_bus& i2c_bus::GetInstance(void)
{
    static i2c_bus  instance;
    return instance;
}

/*void i2c_bus::setHandle(I2C_HandleTypeDef *ph)
{
	pHandle = ph;
}*/

void i2c_bus::init(uint8_t dev_addr)
{
    mDevAddr = dev_addr;
}

MD_STATUS i2c_bus::write(uint8_t *buffer, uint8_t len)
{
	MD_STATUS      err_code;
	mXferDone = false;
    err_code = R_Config_RIIC0_Master_Send(mDevAddr, buffer, len);
    return err_code;
}

MD_STATUS i2c_bus::read(uint8_t *buffer, uint8_t len)
{
	MD_STATUS      err_code;
	mXferDone = false;
    err_code = R_Config_RIIC0_Master_Receive(mDevAddr, buffer, len);
    return err_code;
}

