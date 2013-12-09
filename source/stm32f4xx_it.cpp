/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx_conf.h"
#include "user_conf.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

//Modbus Slave Variables
extern modbusSlave mbSlave;        //in main
extern u16 table[BUFF_SIZE];  //in main
u8 tBuffer[BUFF_SIZE]; //in modbusSlave
u16 rcvByteCounter;    //in modbusSlave


//Modbus Master Variables
//extern uint8_t u8ModbusADU[256];
//extern uint8_t u8ModbusADUSize;
//extern uint8_t u8BytesLeft;
//extern uint8_t waitResponseActive;
//extern uint8_t u8TimeLeft;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * @brief   This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void) {
}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void) {
	/* Go to infinite loop when Hard Fault exception occurs */
	while (1) {
	}
}

/**
 * @brief  This function handles Memory Manage exception.
 * @param  None
 * @retval None
 */
void MemManage_Handler(void) {
	/* Go to infinite loop when Memory Manage exception occurs */
	while (1) {
	}
}

/**
 * @brief  This function handles Bus Fault exception.
 * @param  None
 * @retval None
 */
void BusFault_Handler(void) {
	/* Go to infinite loop when Bus Fault exception occurs */
	while (1) {
	}
}

/**
 * @brief  This function handles Usage Fault exception.
 * @param  None
 * @retval None
 */
void UsageFault_Handler(void) {
	/* Go to infinite loop when Usage Fault exception occurs */
	while (1) {
	}
}

/**
 * @brief  This function handles SVCall exception.
 * @param  None
 * @retval None
 */
void SVC_Handler(void) {
}

/**
 * @brief  This function handles Debug Monitor exception.
 * @param  None
 * @retval None
 */
void DebugMon_Handler(void) {
}

/**
 * @brief  This function handles PendSVC exception.
 * @param  None
 * @retval None
 */
void PendSV_Handler(void) {
}

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void) {

}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f40xx.s/startup_stm32f427x.s).                         */
/******************************************************************************/

void MODBUS_TIMER_IRQHANDLER(void)
{
	if (TIM_GetITStatus(MODBUS_TIMER, TIM_IT_Update) != RESET)
	{
		if (rcvByteCounter > 0)
		{
			mbSlave.update(table, BUFF_SIZE);
			rcvByteCounter = 0;

#ifdef USE_COMUNICATION_LED
			MODBUS_LED_PORT->BSRRH = MODBUS_LED_PIN;
#endif
		}
		MODBUS_TIMER->CNT = 0;
		MODBUS_TIMER->SR = (uint16_t) ~TIM_IT_Update; //ClearITPendingBit
	}
}

void MODBUS_USART_IRQHANDLER(void)
{
  if (USART_GetITStatus(MODBUS_USART, USART_IT_RXNE) != RESET)
    {

      USART_ClearITPendingBit(MODBUS_USART, USART_SR_RXNE);
      tBuffer[rcvByteCounter] = (uint8_t) MODBUS_USART->DR;
      rcvByteCounter++;
      MODBUS_TIMER->CNT = 0;

    }
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
