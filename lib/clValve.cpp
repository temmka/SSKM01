/*
 * clValve.cpp
 *
 *  Created on: Jun 22, 2014
 *      Author: temmka
 *  Description: 
 */

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


















#include "cl_Valve.h"

cl_Valve::cl_Valve()
{
	Model = 0;
	FeedbackMode = 0;

	tFbOp = 0;
	tFbCl = 0;
	tImp = 0;

	pRun = 0;
	pNRun = 0;

	Run = 0;

	alOpened = 0;
	alClosed = 0;
	alCurrent = 0;

	in = 0;
	Opened = 0;
	Closed = 0;
	out = 0;

}

void cl_Valve::process(u16 &reg)
{
	u16 regs=0;
	regs = reg & 0xF;

	Model = regs & 0x03;
	FeedbackMode = (regs >> 2) & 0x03;

	Closed = GPIO_ReadInputDataBit(PortClose, PinClose);
	Opened = GPIO_ReadInputDataBit(PortOpen, PinOpen);
	Current = GPIO_ReadInputDataBit(PortCurr, PinCurr);


	in = regBitToBit(reg, 9);

	if (in)
	{
		pNRun = false;

		if (!pRun)
		{
			pRun = true;

			switch (Model)
			{
			case T1:
				Run = true;
				out = true;
				tImp = RTC->TR + T1_ImpulseTime;
				break;

			case T2:
				Run = true;
				out = true;
				break;

			case T3:
				Run = true;
				out = false;
				break;

			default:
				Run = false;
				out = false;
				break;
			}

			tFbOp = RTC->TR + FeedbackTime;

		}

		if (Model == T1 && RTC->TR > tImp)
		{

			out = false;

		}

	}
	else
	{
		pRun = false;

		if (!pNRun)
		{

			pNRun = true;

			switch (Model)
			{
			case T1:
				Run = false;
				out = false;
				break;

			case T2:
				Run = false;
				out = false;
				break;

			case T3:
				Run = false;
				out = true;
				break;

			default:
				Run = false;
				out = false;
				break;
			}

			tFbCl = RTC->TR + FeedbackTime;

		}

		if (Model == T3 && RTC->TR > tFbCl)
		{

			alCurrent = true;

		}

	}

	switch (FeedbackMode)
	{
	case OpEn:

		if (Run && RTC->TR > tFbOp && !Opened)
		{
			alOpened = true;
		}
		break;

	case ClEn:

		if (Run && RTC->TR > tFbCl && !Closed)
		{
			alClosed = true;
		}
		break;

	case OpClEn:
		if (Run && RTC->TR > tFbOp && !Opened)
		{
			alOpened = true;
		}

		if (Run && RTC->TR > tFbCl && !Closed)
		{
			alClosed = true;
		}
		break;

	default:
		alOpened = false;
		alClosed = false;
		break;
	}

	if (out != false)
	{
		PortOut->BSRRL = PinOut;
	}
	else
	{
		PortOut->BSRRH = PinOut;
	}

	BitToRegBit(reg, 4, Opened);
	BitToRegBit(reg, 5, Closed);
	BitToRegBit(reg, 6, alOpened);
	BitToRegBit(reg, 7, alClosed);
	BitToRegBit(reg, 8, alCurrent);

reset = regBitToBit(reg, 10);
 if (reset){
	 alOpened = false;
	 alClosed = false;
	 alCurrent = false;
 }


}

void cl_Valve::getMode(u16 regs, u8 Number)
{

	Number = Number * 4 - 4;
	regs = regs >> Number;
	regs = regs & 0xF;

	Model = regs & 0x03;
	FeedbackMode = (regs >> 2) & 0x03;

}
