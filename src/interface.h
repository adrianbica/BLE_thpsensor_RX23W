/*
 * interface.h
 *
 *  Created on: Feb 18, 2020
 *      Author: adrian.bica
 */

#ifndef INTERFACE_H_
#define INTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif

void Tick(void);
void CompleteI2CTransfer(void);
void InitSensorTask(void);
void InitRelays(void);

#ifdef __cplusplus
}
#endif



#endif /* INTERFACE_H_ */
