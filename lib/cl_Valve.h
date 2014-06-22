/*
 * cl_Valve.h
 *
 *  Created on: Jun 22, 2014
 *      Author: temmka
 *  Description: 
 */

#ifndef CL_VALVE_H_
#define CL_VALVE_H_

enum
{
	T1 = 1, T2 = 2, T3 = 3
};

enum
{
	AllDsbl = 0, OpEn = 1, ClEn = 2, OpClEn = 3
};

#include "stm32f4xx_conf.h"
#include "user_conf.h"
/*
 *@autor
 */
class cl_Valve
{
public:
	cl_Valve();

	void process(u16 &reg);

	u8 Model;  //1=T1, 1=T2, 2=T2;
	u8 FeedbackMode; //0=No Use, 1= Only opened, 2= Only closed, 3 = 1+2;
	void getMode(u16 regs, u8 Number);
	static const u32 T1_ImpulseTime = 5;;
	static const  u32 FeedbackTime = 10;



	u32 tFbOp;
	u32 tFbCl;
	u32 tImp;

	bool pRun;
	bool pNRun;

	bool Run;

	bool alOpened;
	bool alClosed;
	bool alCurrent;

	bool in;
	bool Opened;
	bool Closed;
	bool Current;
	bool out;
	bool reset;

	  GPIO_TypeDef *PortOut;
	  uint16_t PinOut;

	  GPIO_TypeDef *PortOpen;
	  uint16_t PinOpen;

	  GPIO_TypeDef *PortClose;
	  uint16_t PinClose;

	  GPIO_TypeDef *PortCurr;
	  uint16_t PinCurr;




};

#endif /* CL_VALVE_H_ */
