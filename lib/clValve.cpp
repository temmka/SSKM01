/*
 * clValve.cpp
 *
 *  Created on: Jun 22, 2014
 *      Author: temmka
 *  Description: 
 */

#include "cl_Valve.h"

cl_Valve::cl_Valve()
{
	Model = 0;
	tModel = 0;
	tFbOp = 0;
	tFbCl = 0;
	tImp = 0;

	pRun = 0;
	nRun = 0;
	tFbCl = RTC->TR + FeedbackTime;
	Run = 0;

	alOpened = 0;
	alClosed = 0;
	alCurrent = 0;

	Current = 0;
	out = 0;

	PortOut = 0;
	PinOut = 0;
	PortOpen = 0;
	PinOpen = 0;
	PortClose = 0;
	PinClose = 0;
	PortCurr = 0;
	PinCurr = 0;

}

void cl_Valve::process(u16 &reg)
{
	breg = reg;
	Model = reg & 0x03;

	if (tModel != Model)
	{
		tModel = Model;
		nRun = false;
		pRun = false;
	}


	if (breg[vFbClose])
	{
		breg[vClose] = GPIO_ReadInputDataBit(PortClose, PinClose);
	}
	else
	{
		breg[vClose] = !breg[vIn];
	}

	if (breg[vFbOpen])
	{
		breg[vOpen] = GPIO_ReadInputDataBit(PortOpen, PinOpen);
	}
	else
	{
		breg[vOpen] = breg[vIn];
	}

	if (alOpened && breg[vOpen] ){
		alOpened = false;
	}
	if (alClosed && breg[vClose] ){
		alClosed = false;
	}

	Current = GPIO_ReadInputDataBit(PortCurr, PinCurr);

	//Reset Alarm
	if (breg[vReset])
	{
		alOpened = false;
		alClosed = false;
		alCurrent = false;
		tFbCl = RTC->TR + FeedbackTime;
		tFbOp = RTC->TR + FeedbackTime;
		//breg[vReset] = false;
	}

	//Start
	if (breg[vIn])
	{
		nRun = false;

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

		if (!nRun)
		{
			nRun = true;

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

		if (Model == T3 && Current && RTC->TR > tFbCl)
		{
			alCurrent = true;
		}

	}

	//Alarm open
	if (breg[vFbOpen])
	{
		if (Run && RTC->TR > tFbOp && !breg[vOpen])
		{
			alOpened = true;
		}
	}

	//Alarm close
	if (breg[vFbClose])
	{
		if (!Run && RTC->TR > tFbCl && !breg[vClose])
		{
			alClosed = true;
		}
	}

	//Output relay
	if (out != false)
	{
		PortOut->BSRRL = PinOut;
	}
	else
	{
		PortOut->BSRRH = PinOut;
	}

	breg[vAlOpen] = alOpened;
	breg[vAlClose] = alClosed;
	breg[vAlCurrent] = alCurrent;

	reg = (u16) breg.to_ulong();

}

