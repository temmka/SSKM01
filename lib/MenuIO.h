/*
 * MenuIO.h
 *
 *  Created on: 22 лист. 2013
 *      Author: ANK
 */


#ifndef MENUIO_H
#define MENUIO_H

#include "stm32f4xx_conf.h"
//#include "user_conf.h"

 u8 letter[21]{
    0x3F ,//0
    0x0F ,//1
    0x5B ,//2
    0x4F ,//3
    0x66 ,//4
    0x6D ,//5
    0x7C ,//6
    0x07 ,//7
    0x7F ,//8
    0x6F ,//9
    0x77 ,//A
    0x7C ,//b
    0x39 ,//C
    0x5E ,//d
    0x79 ,//E
    0x71 ,//F
    0x58 ,//c
    0x74 ,//h
    0x76 ,//H
    0x0E ,//J
    0x38  //L
    };



typedef struct{

	GPIO_TypeDef* Port;
	u16           Pin;

}GpioLt;

typedef struct{

	GPIO_TypeDef* A_Port;
	GPIO_TypeDef* B_Port;
	GPIO_TypeDef* C_Port;
	GPIO_TypeDef* D_Port;
	GPIO_TypeDef* E_Port;
	GPIO_TypeDef* F_Port;
	GPIO_TypeDef* G_Port;
	GPIO_TypeDef* DP_Port;

	u16           A_Pin;
	u16           B_Pin;
	u16           C_Pin;
	u16           D_Pin;
	u16           E_Pin;
	u16           F_Pin;
	u16           G_Pin;
	u16           DP_Pin;


}GpioSS_IO;



class Menu_IO {
public:

	static const u8 DigitMax = 4; //Количество сегментов
	u8 DigitCnt; //Количество используемых сегментов
	GpioSS_IO GpioIO;

	Menu_IO();
	GpioLt Digit[DigitMax];

    void addDiigit(GPIO_TypeDef* Port, u16 Pin);

    void  vPortWrite(u8 value, bool digit, u8 CurrDigit);




    void GpioConfig();







};

#endif /* MENUIO_H_ */
