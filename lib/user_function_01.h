#ifndef USER_FUNCTION_01_H
#define USER_FUNCTION_01_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_conf.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void initModbusUsart(void);
void initModbusTimer(void);

void initRTC(void);

/*read 8 input pin to byte*/
uint8_t readSlaveAddress(void);
void initRTC(void);
void pinConfig(void);
void writeBitInReg(uint16_t reg, uint8_t bitNumber, uint8_t value);
uint8_t readBitInReg(uint16_t reg, uint8_t bitNumber);
uint16_t tPow2(uint16_t n);
void regBitToPin(uint16_t reg, uint8_t bitNumber, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void PinToregBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint16_t &reg, uint8_t bitNumber);
void invPinToregBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint16_t &reg, uint8_t bitNumber);
void BitToRegBit(uint16_t &reg, uint8_t bitNumber, bool Value);
bool regBitToBit(uint16_t reg, uint8_t bitNumber);
uint8_t highByte(uint16_t Value);
uint8_t lowByte(uint16_t Value);
uint16_t _crc16_update(uint16_t crc, uint8_t a);
int bitRead(uint8_t b, int bitPos);






uint16_t word(uint8_t high, uint8_t low);
void delayMicroseconds(uint32_t time);

bool ton(bool input, u16 delayTime);
bool tof(bool input, u16 delayTime);

bool tpulse(void);


void alToRegBit (uint16_t *reg, uint8_t bitNumber);




#ifdef __cplusplus
}
#endif

#endif /* FUNC_H */

