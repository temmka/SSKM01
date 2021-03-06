#include "stm32f4xx_conf.h"
#include "user_conf.h"

u16 table[BUFF_SIZE]; //modbus table
modbusSlave mbSlave; //object mbSlave

int main(void)
{
	SystemInit();

	initRTC();

	GpioSpi spiPinStruct;

	softSpi spiFlash;
	softSpi spiExp;
	eeprom Flash;
	mcp23s17 exp[8];

	pinConfig();

	//Startup blink
//	for (int i = 0; i < 10; i++)
//	{
//		delayMicroseconds(100);
//		GPIO_ToggleBits(MODBUS_LED_PORT, MODBUS_LED_PIN);
//	}
	initRTC();
	mbSlave.configureAddress(readSlaveAddress());

	/*Init SPI structure*/
	spiPinStruct.csPORT = GPIOA;
	spiPinStruct.csPIN = GPIO_Pin_4;

	spiPinStruct.clkPORT = GPIOA;
	spiPinStruct.clkPIN = GPIO_Pin_5;

	spiPinStruct.siPORT = GPIOA;
	spiPinStruct.siPIN = GPIO_Pin_7;

	spiPinStruct.soPORT = GPIOA;
	spiPinStruct.soPIN = GPIO_Pin_6;

	/*Flash SPI init*/
	spiFlash.init(spiPinStruct);

	/*Expander SPI init*/
	spiPinStruct.csPORT = GPIOB;
	spiExp.init(spiPinStruct);

	/*Expander init*/

	exp[0].deviceAddress = 7;
	exp[1].deviceAddress = 0;
	exp[2].deviceAddress = 4;
	exp[3].deviceAddress = 2;
	exp[4].deviceAddress = 6;
	exp[5].deviceAddress = 1;
	exp[6].deviceAddress = 5;
	exp[7].deviceAddress = 3;

	exp[0].spiNum = &spiExp;
	exp[1].spiNum = &spiExp;
	exp[2].spiNum = &spiExp;
	exp[3].spiNum = &spiExp;
	exp[4].spiNum = &spiExp;
	exp[5].spiNum = &spiExp;
	exp[6].spiNum = &spiExp;
	exp[7].spiNum = &spiExp;

	/*Flash init*/
	Flash.spiNum = &spiFlash;
	Flash.init();

	exp[0].Init(0xFF, 0xFF); // Input
	exp[1].Init(0x00, 0x00); // Output
	exp[2].Init(0x00, 0x00); // Output
	exp[3].Init(0x00, 0x00); // Output
	exp[4].Init(0x00, 0x00); // Output
	exp[5].Init(0x00, 0x00); // Output
	exp[6].Init(0x00, 0x00); // Output
	exp[7].Init(0x00, 0x00); // Output

	cl_Valve v1;
	cl_Valve v2;
	cl_Valve v3;
	cl_Valve v4;

	initModbusUsart();
	initModbusTimer();

	v1.PortOpen = GPIOC;
	v1.PinOpen = GPIO_Pin_2;

	v1.PortClose = GPIOC;
	v1.PinClose = GPIO_Pin_1;

	v1.PortCurr = GPIOC;
	v1.PinCurr = GPIO_Pin_13;

	v1.PortOut = GPIOB;
	v1.PinOut = GPIO_Pin_1;

	v2.PortOpen = GPIOA;
	v2.PinOpen = GPIO_Pin_0;

	v2.PortClose = GPIOC;
	v2.PinClose = GPIO_Pin_3;

	v2.PortCurr = GPIOC;
	v2.PinCurr = GPIO_Pin_14;

	v2.PortOut = GPIOB;
	v2.PinOut = GPIO_Pin_2;

	v3.PortOpen = GPIOA;
	v3.PinOpen = GPIO_Pin_2;

	v3.PortClose = GPIOA;
	v3.PinClose = GPIO_Pin_1;

	v3.PortCurr = GPIOC;
	v3.PinCurr = GPIO_Pin_15;

	v3.PortOut = GPIOB;
	v3.PinOut = GPIO_Pin_10;

	v4.PortOpen = GPIOC;
	v4.PinOpen = GPIO_Pin_4;

	v4.PortClose = GPIOA;
	v4.PinClose = GPIO_Pin_3;

	v4.PortCurr = GPIOC;
	v4.PinCurr = GPIO_Pin_0;

	v4.PortOut = GPIOB;
	v4.PinOut = GPIO_Pin_5;

	u32 cmpVal[8] =
	{ 100000, 1000000, 1000000, 1000000, 1000000, 1000000, 1000000, 1000000 };

	bool p[8] =

	{ 1, 1, 1, 1, 1, 1, 1, 1 };

	/* WWDG configuration */
	/* Enable WWDG clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);

	/* WWDG clock counter = (PCLK1 (42MHz)/4096)/8 = 1281 Hz (~780 us)  */
	WWDG_SetPrescaler(WWDG_Prescaler_8);

	/* Set Window value to 80; WWDG counter should be refreshed only when the counter
	 is below 80 (and greater than 64) otherwise a reset will be generated */
	WWDG_SetWindowValue(255);

	/* Enable WWDG and set counter value to 127, WWDG timeout = ~780 us * 64 = 49.92 ms
	 In this case the refresh window is:
	 ~780 * (127-80) = 36.6ms < refresh window < ~780 * 64 = 49.9ms
	 */
	WWDG_Enable(255);

	while (1)
	{
        //Reset Inputs
		if (table[0] >> 10 & 0x01)
		{
			table[4] = 0;
		}


		v1.process(table[0]);
		v2.process(table[1]);
		v3.process(table[2]);
		v4.process(table[3]);


		PinToregBit(GPIOB, GPIO_Pin_15, table[4], 0); // PB15 = res_in_1
		PinToregBit(GPIOC, GPIO_Pin_8, table[4], 1); //	 PC8  = res_in_2
		PinToregBit(GPIOC, GPIO_Pin_9, table[4], 2); //	 PC9  = res_in_3
		PinToregBit(GPIOA, GPIO_Pin_8, table[4], 3); //	 PA8  = res_in_4
		PinToregBit(GPIOD, GPIO_Pin_2, table[4], 4); //	 PD2  = res_in_5
		PinToregBit(GPIOB, GPIO_Pin_3, table[4], 5); //	 PB3  = res_in_6
//		PinToregBit(GPIOC, GPIO_Pin_5, table[4], 6, cmpVal[6], p[6]); //	 PC5 = POWER_1_OK_iso
//		PinToregBit(GPIOB, GPIO_Pin_0, table[4], 7, cmpVal[7], p[7]); //	 PB0 = POWER_2_OK_iso

		//40006 SPI Input register
		table[5] = exp[0].AB();

		regBitToPin(table[6], 0, GPIOB, GPIO_Pin_12); // PB12 = res_rel_1
		regBitToPin(table[6], 1, GPIOB, GPIO_Pin_13); // PB13 = res_rel_2
		regBitToPin(table[6], 2, GPIOB, GPIO_Pin_14); // PB14 = res_rel_3
		regBitToPin(table[6], 3, GPIOB, GPIO_Pin_6);  // PB6  = res_rel_4

		//40008 SPI Output register
		exp[1].AB(table[7]);

		//40009 SPI Output register
		exp[2].AB(table[8]);

		//40010 SPI Output register
		exp[3].AB(table[9]);

		//40011 SPI Output register
		exp[4].AB(table[10]);

		//40012 SPI Output register
		exp[5].AB(table[11]);

		//40013 SPI Output register
		exp[6].AB(table[12]);

		//40014 SPI Output register
		exp[7].AB(table[13]);

		if ((table[4] & 0xFF) == 0x3F)
		{
			while (1)
			{

			}
		}

		WWDG_SetCounter(255);

	} //while(1)

	return 0;
}
;
//main

