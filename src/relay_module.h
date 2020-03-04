/*
 * relay_module.h
 *
 *  Created on: Mar 1, 2020
 *      Author: adrian.bica
 */

#ifndef RELAY_MODULE_H_
#define RELAY_MODULE_H_

#include <stdint.h>

/*
 * A relay module has 4 relays connected to four digital outputs from the microcontroller
 * A logic 0 to the output will activate the relay, while a
 */
class relayModule
{
#define NB_RELAYS	(4)
private:
	relayModule(void);
	void Deactivate(int relay);
	void Activate(int relay);
public:
	~relayModule(void);
    static relayModule& GetInstance(void);
	void Init(int relay);
	void task(void);
};



#endif /* RELAY_MODULE_H_ */
