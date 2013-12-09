#ifndef EXPANDER_H
#define EXPANDER_H

#include "stm32f4xx_conf.h"
#include "user_conf.h"
class mcp23s17 {
public:
	mcp23s17();
	mcp23s17(softSpi* spiNumber);
	mcp23s17(softSpi* spiNumber, u8 Address);


	softSpi* spiNum;

	u8 deviceAddress;

	u8 A();
	void A(u8 data);


	u8 B();
	void B(u8 data);

	u8 Read(u8 port);
	void Write(u8 reg, u8 data);


	u16 AB();
	void AB(u16 data);

	void Init(uint8_t portA, uint8_t portB);




};







#endif /* EXPANDER_H */




