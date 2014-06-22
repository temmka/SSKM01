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

	cl_Valve v1;
	cl_Valve v2;
	cl_Valve v3;
	cl_Valve v4;


    //Startup blink
	for (int i = 0; i < 10; i++)
	{
		delayMicroseconds(100000);
		GPIO_ToggleBits(MODBUS_LED_PORT, MODBUS_LED_PIN);
	}

	initModbusTimer();
	initModbusUsart();
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
	for (int i = 0; i < 8; i++)
	{
		exp[i].deviceAddress = i;
		exp[i].spiNum = &spiExp;

	}

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



	v1.PortOpen = GPIOC ;
	v1.PinOpen = GPIO_Pin_1 ;

	v1.PortClose = GPIOC;
	v1.PinClose = GPIO_Pin_2;

	v1.PortCurr  = GPIOC;
	v1.PinCurr  = GPIO_Pin_13;

	v1.PortOut = GPIOB;
	v1.PinOut = GPIO_Pin_1;

	v1.PortOpen = GPIOC ;
	v1.PinOpen = GPIO_Pin_1 ;

	v1.PortClose = GPIOC;
	v1.PinClose = GPIO_Pin_2;

	v1.PortCurr  = GPIOC;
	v1.PinCurr  = GPIO_Pin_13;

	v1.PortOut = GPIOB;
	v1.PinOut = GPIO_Pin_1;


	v1.PortOpen = GPIOC ;
	v1.PinOpen = GPIO_Pin_1 ;

	v1.PortClose = GPIOC;
	v1.PinClose = GPIO_Pin_2;

	v1.PortCurr  = GPIOC;
	v1.PinCurr  = GPIO_Pin_13;

	v1.PortOut = GPIOB;
	v1.PinOut = GPIO_Pin_1;


	v1.PortOpen = GPIOC ;
	v1.PinOpen = GPIO_Pin_1 ;

	v1.PortClose = GPIOC;
	v1.PinClose = GPIO_Pin_2;

	v1.PortCurr  = GPIOC;
	v1.PinCurr  = GPIO_Pin_13;

	v1.PortOut = GPIOB;
	v1.PinOut = GPIO_Pin_1;


	while (1)
	{







		v1.process(table[0]);



//		//40001 Input register
//		invPinToregBit(GPIOC, GPIO_Pin_5, table[0], 0); //	 PC5 = POWER_1_OK_iso
//		invPinToregBit(GPIOB, GPIO_Pin_0, table[0], 1); //	 PB0 = POWER_2_OK_iso
//		PinToregBit(GPIOC, GPIO_Pin_1, table[0], 2);  //	 PC1 = 1_open
//		PinToregBit(GPIOC, GPIO_Pin_2, table[0], 3);  //	 PC2 = 1_close
//		PinToregBit(GPIOC, GPIO_Pin_3, table[0], 4);  //	 PC3 = 2_open
//		PinToregBit(GPIOA, GPIO_Pin_0, table[0], 5);  //	 PA0 = 2_close
//		PinToregBit(GPIOA, GPIO_Pin_1, table[0], 6);  //	 PA1 = 3_open
//		PinToregBit(GPIOA, GPIO_Pin_2, table[0], 7);  //	 PA2 = 3_close
//		PinToregBit(GPIOA, GPIO_Pin_3, table[0], 8);  //	 PA3 = 4_open
//		PinToregBit(GPIOC, GPIO_Pin_4, table[0], 9);  //	 PC4 = 4_close
//		PinToregBit(GPIOB, GPIO_Pin_15, table[0], 10); //	 PB15 = res_in_1
//		PinToregBit(GPIOC, GPIO_Pin_8, table[0], 11); //	 PC8  = res_in_2
//		PinToregBit(GPIOC, GPIO_Pin_9, table[0], 12); //	 PC9  = res_in_3
//		PinToregBit(GPIOA, GPIO_Pin_8, table[0], 13); //	 PA8  = res_in_4
//		PinToregBit(GPIOD, GPIO_Pin_2, table[0], 14); //	 PD2  = res_in_5
//		PinToregBit(GPIOB, GPIO_Pin_3, table[0], 15); //	 PB3  = res_in_6
//
//		//40002 Input register
//		///table[1] = 0;
//
//		//40003 Fault register
//		invPinToregBit(GPIOC, GPIO_Pin_13, table[2], 0);  //  PC13 = FAULT_1_iso
//		invPinToregBit(GPIOC, GPIO_Pin_14, table[2], 1);  //  PC14 = FAULT_2_iso
//		invPinToregBit(GPIOC, GPIO_Pin_15, table[2], 2);  //  PC15 = FAULT_3_iso
//		invPinToregBit(GPIOC, GPIO_Pin_0, table[2], 3);
//		//  PC0  = FAULT_4_iso
//
//		tof((bool) table[12], 5);
//
//		//40004 Fault register
//	//	table[3] = 0;
//
//		//40005 Output register
//
//		//regBitToPin(table[4], 0, GPIOB, GPIO_Pin_1);  // PB1 = rel_1
//		regBitToPin(table[4], 1, GPIOB, GPIO_Pin_2);  // PB2 = rel_2
//		regBitToPin(table[4], 2, GPIOB, GPIO_Pin_10); // PB10 = rel_3
//		regBitToPin(table[4], 3, GPIOB, GPIO_Pin_11); // PB11 = rel_4
//		regBitToPin(table[4], 4, GPIOB, GPIO_Pin_12); // PB12 = res_rel_1
//		regBitToPin(table[4], 5, GPIOB, GPIO_Pin_13); // PB13 = res_rel_2
//		regBitToPin(table[4], 6, GPIOB, GPIO_Pin_14); // PB14 = res_rel_3
//		regBitToPin(table[4], 7, GPIOB, GPIO_Pin_6);  // PB6  = res_rel_4

		table[4] &= 0x00FF; //Reserve

		//40006 Output register
		table[5] = 0x0000;  //Reserve

		//40007 SPI Input register
		table[6] = exp[0].AB();

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

	} //while(1)

	return 0;
}
;
//main

