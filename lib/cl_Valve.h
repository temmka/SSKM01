/*
 * cl_Valve.h
 *
 *  Created on: Jun 22, 2014
 *      Author: temmka
 *  Description: 
 */

#ifndef CL_VALVE_H_
#define CL_VALVE_H_



#include "stm32f4xx_conf.h"
#include "user_conf.h"
#include "bitset"


enum
{
	T1 = 1, T2 = 2, T3 = 3
};

enum
{
	vMode1,
	vMode2,
	vFbOpen,
	vFbClose,
	vOpen,
	vClose,
	vAlOpen,   //*
	vAlClose,  //*
	vAlCurrent,
	vIn,       //*
	vReset     //*

};


class cl_Valve
{
public:
	cl_Valve();
	void process(u16 &reg);


    std::bitset <16> breg;
	static const u32 T1_ImpulseTime = 6;
	static const u32 FeedbackTime = 40;



	GPIO_TypeDef *PortOut;
	uint16_t PinOut;

	GPIO_TypeDef *PortOpen;
	uint16_t PinOpen;

	GPIO_TypeDef *PortClose;
	uint16_t PinClose;

	GPIO_TypeDef *PortCurr;
	uint16_t PinCurr;

private:

	u8 Model;  //1=T1, 1=T2, 2=T2;
	u8 tModel;

	bool Current; //Signal current
	bool out;    //Output

	u32 tFbOp; //mem for open-feedback
	u32 tFbCl; //mem for close-feedback
	u32 tImp;  //mem for curr alarm

	bool pRun; //On front
	bool nRun; //Off front

	bool Run; //Bit for start alarm

	bool alOpened; //Alarm open
	bool alClosed; //Alarm close
	bool alCurrent; //Alarm current

};

#endif /* CL_VALVE_H_ */
