#ifndef MAIN_H
#define MAIN_H





///////////////////////////////////////////////////////////////////////////////////////////////////
// MODBUS USART
///////////////////////////////////////////////////////////////////////////////////////////////////

#define MODBUS_USART              USART6
#define MODBUS_USART_BAUDRATE    (uint16_t)38400

#define MODBUS_USART_WORDLENGTH   USART_WordLength_8b
#define MODBUS_USART_STOPBITS     USART_StopBits_1
#define MODBUS_USART_PARITY       USART_Parity_No
#define MODBUS_USART_HWFWCTRL     USART_HardwareFlowControl_None

#define MODBUS_USART_TX_PORT      GPIOC
#define MODBUS_USART_TX_PORT_AF   RCC_AHB1Periph_GPIOC
#define MODBUS_USART_TX_PIN       GPIO_Pin_6
#define MODBUS_USART_TX_PINSOURCE GPIO_PinSource6

#define MODBUS_USART_RX_PORT      GPIOC
#define MODBUS_USART_RX_PORT_AF   RCC_AHB1Periph_GPIOC
#define MODBUS_USART_RX_PIN       GPIO_Pin_7
#define MODBUS_USART_RX_PINSOURCE GPIO_PinSource7

#define MODBUS_USART_IRQN	       USART6_IRQn
#define MODBUS_USART_IRQHANDLER    USART6_IRQHandler
#define MODBUS_USART_GPIO_AF       GPIO_AF_USART6
#define MODBUS_USART_RCC           RCC_APB2Periph_USART6
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// MODBUS TIMER
///////////////////////////////////////////////////////////////////////////////////////////////////
#define MODBUS_TIMER        	   TIM3
#define MODBUS_TIMER_IRQN	  	   TIM3_IRQn
#define MODBUS_TIMER_IRQHANDLER	   TIM3_IRQHandler
#define MODBUS_TIMER_RCC           RCC_APB1Periph_TIM3
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// USER PARAMETR
///////////////////////////////////////////////////////////////////////////////////////////////////

#define BUFF_SIZE 14 //Modbus table size

#define USE_COMUNICATION_LED
#define MODBUS_LED_PORT      GPIOB
#define MODBUS_LED_PIN       GPIO_Pin_6

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif /* MAIN_H */
