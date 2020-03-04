#ifndef __I2C_BUD_H__
#define __I2C_BUD_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "r_smc_entry.h"

#ifdef __cplusplus
}
#endif

class i2c_bus
{
private:
    uint8_t     		mDevAddr;
    volatile bool 		mXferDone;
    uint32_t			mErrorCode;
    // private default constructor(prohibits creation from outside)
    i2c_bus(void);
    // private copy constructor (prohibits copy creation)
    i2c_bus(const i2c_bus&);
    ~i2c_bus(void);
public:
    static i2c_bus& GetInstance(void);
    void init(uint8_t dev_addr);
    MD_STATUS write(uint8_t *buffer, uint8_t len);
    MD_STATUS read(uint8_t *buffer, uint8_t len);
    bool transfer_ready(void) {return mXferDone;};
    void complete_transfer(void) {mXferDone = true;};
    void setError(uint32_t err) { mErrorCode = err; };
    uint32_t getError() { return mErrorCode; };
};

#endif

