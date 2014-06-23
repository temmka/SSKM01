/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx_conf.h"
#include "user_conf.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void initModbusUsart(void)
{

	NVIC_InitTypeDef NVIC_Init_Structure;
	USART_InitTypeDef USART_Init_Structure;
	GPIO_InitTypeDef GPIO_Init_Structure;

	RCC_AHB1PeriphClockCmd(MODBUS_USART_TX_PORT_AF, ENABLE);
	RCC_AHB1PeriphClockCmd(MODBUS_USART_RX_PORT_AF, ENABLE);
	RCC_APB2PeriphClockCmd(MODBUS_USART_RCC, ENABLE);

	//  Connect PXx to USARTx_Tx
	GPIO_PinAFConfig(MODBUS_USART_TX_PORT, MODBUS_USART_TX_PINSOURCE,
	MODBUS_USART_GPIO_AF);

	//  Connect PXx to USARTx_Rx
	GPIO_PinAFConfig(MODBUS_USART_RX_PORT, MODBUS_USART_RX_PINSOURCE,
	MODBUS_USART_GPIO_AF);

	GPIO_Init_Structure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init_Structure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init_Structure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init_Structure.GPIO_Speed = GPIO_Speed_100MHz;

	//Configure USART Tx as alternate function
	GPIO_Init_Structure.GPIO_Pin = MODBUS_USART_TX_PIN;
	GPIO_Init(MODBUS_USART_TX_PORT, &GPIO_Init_Structure);

	//Configure USART Rx as alternate function
	GPIO_Init_Structure.GPIO_Pin = MODBUS_USART_RX_PIN;
	GPIO_Init(MODBUS_USART_RX_PORT, &GPIO_Init_Structure);

	USART_Init_Structure.USART_BaudRate = MODBUS_USART_BAUDRATE;
	USART_Init_Structure.USART_WordLength = MODBUS_USART_WORDLENGTH;
	USART_Init_Structure.USART_StopBits = MODBUS_USART_STOPBITS;
	USART_Init_Structure.USART_Parity = MODBUS_USART_PARITY;
	USART_Init_Structure.USART_HardwareFlowControl = MODBUS_USART_HWFWCTRL;
	USART_Init_Structure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(MODBUS_USART, &USART_Init_Structure);
	USART_Cmd(MODBUS_USART, ENABLE);
	USART_ITConfig(MODBUS_USART, USART_IT_RXNE, ENABLE);

	NVIC_Init_Structure.NVIC_IRQChannel = MODBUS_USART_IRQN;
	NVIC_Init_Structure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Init_Structure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init_Structure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_Init_Structure);
	NVIC_EnableIRQ(MODBUS_USART_IRQN);
}

void initModbusTimer(void)
{
	uint16_t bytePerSecond; //  serial speed (bit/s)  / 8
	float timeForByte; // 1 byte transmit/recive time
	uint16_t overtime; // overtime
	uint16_t TimPeriod;

	bytePerSecond = MODBUS_USART_BAUDRATE / 8;

	timeForByte = (1.0 / (float) bytePerSecond) * 1000000.0; // = N mks  (*1000000 - correction for timer )

	overtime = 300; // added to timeForByte

	TimPeriod = (uint16_t) timeForByte + overtime;

	NVIC_InitTypeDef NVIC_Init_Structure;
	TIM_TimeBaseInitTypeDef TimeBaseInit_Structure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TimeBaseInit_Structure.TIM_Prescaler = 83;
	TimeBaseInit_Structure.TIM_Period = TimPeriod; // value -1; 1 = 2 mks 999 = 1000mks = 1ms
	TimeBaseInit_Structure.TIM_ClockDivision = TIM_CKD_DIV1;
	TimeBaseInit_Structure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(MODBUS_TIMER, &TimeBaseInit_Structure);
	TIM_Cmd(MODBUS_TIMER, ENABLE);
	TIM_ARRPreloadConfig(MODBUS_TIMER, ENABLE);

	TIM_ITConfig(MODBUS_TIMER, TIM_IT_Update, ENABLE);

	NVIC_Init_Structure.NVIC_IRQChannel = MODBUS_TIMER_IRQN;
	NVIC_Init_Structure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_Init_Structure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init_Structure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_Init_Structure);

}






uint8_t readSlaveAddress(void)
{
	uint8_t addr = 0;
	addr |= (GPIOA->IDR & GPIO_Pin_9) ? 0 : 1;
	addr |= ((GPIOA->IDR & GPIO_Pin_10) ? 0 : 1) << 1;
	addr |= ((GPIOA->IDR & GPIO_Pin_11) ? 0 : 1) << 2;
	addr |= ((GPIOA->IDR & GPIO_Pin_12) ? 0 : 1) << 3;
	addr |= ((GPIOA->IDR & GPIO_Pin_15) ? 0 : 1) << 4;
	addr |= ((GPIOC->IDR & GPIO_Pin_10) ? 0 : 1) << 5;
	addr |= ((GPIOC->IDR & GPIO_Pin_11) ? 0 : 1) << 6;
	addr |= ((GPIOC->IDR & GPIO_Pin_12) ? 0 : 1) << 7;

	return addr;
}

void pinConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); // GPIO_F
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); // GPIO_F
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); // GPIO_F
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); // GPIO_F

	/* Address pin INPUT (ground control)
	 PA9  = adres1
	 PA10 = adres2
	 PA11 = adres3
	 PA12 = adres4
	 PA15 = adres5
	 PC10 = adres6
	 PC11 = adres7
	 *PC12 = adres8
	 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_15;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Fault pin INPUT
	 PC13 = FAULT_1_iso
	 PC14 = FAULT_2_iso
	 PC15 = FAULT_3_iso
	 PC0  = FAULT_4_iso
	 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 | GPIO_Pin_0;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* State pin INPUT
	 PC1 = 1_open
	 PC2 = 1_close
	 PC3 = 2_open
	 PA0 = 2_close
	 PA1 = 3_open
	 PA2 = 3_close
	 PA3 = 4_open
	 PC4 = 4_close
	 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* State Power pin INPUT
	 PC5 = POWER_1_OK_iso
	 PB0 = POWER_2_OK_iso
	 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* State Reserved pin INPUT
	 PB15 = res_in_1
	 PC8  = res_in_2
	 PC9  = res_in_3
	 PA8  = res_in_4
	 PD2  = res_in_5
	 PB3  = res_in_6
	 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_3;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/*Relay pin OUTPUT
	 PB1 = rel_1
	 PB2 = rel_2
	 PB10 = rel_3
	 PB11 = rel_4
	 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/*Reserved pin OUTPUT
	 PB12 = res_rel_1
	 PB13 = res_rel_2
	 PB14 = res_rel_3
	 PB6  = res_rel_4
	 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/*Led Comunication pin OUTPUT
	 PB5  = LedComunication
	 */

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}

uint16_t tPow2(uint16_t n)
{
	uint16_t i, result;

	result = 1;

	for (i = 1; i <= n; i++)
	{
		result = result * 2;
	}

	return result;

}

void regBitToPin(uint16_t reg, uint8_t bitNumber, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	if (reg & (1 << bitNumber))
	{
		GPIOx->BSRRL = GPIO_Pin;
	}
	else
	{
		GPIOx->BSRRH = GPIO_Pin;
	}

}

bool regBitToBit(uint16_t reg, uint8_t bitNumber)
{
	if (reg & (1 << bitNumber))
	{
		return true;
	}
	else
	{
		return false;
	}

}

void PinToregBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint16_t &reg, uint8_t bitNumber)
{

	if ((GPIOx->IDR & GPIO_Pin) != (uint32_t) Bit_RESET)
	{
		reg |= (1 << bitNumber);
	}
	else
	{
		reg &= ~(1 << bitNumber);
	}
}


void BitToRegBit(uint16_t &reg, uint8_t bitNumber, bool Value)
{

	if (Value)
	{
		reg |= (1 << bitNumber);
	}
	else
	{
		reg &= ~(1 << bitNumber);
	}
}





void invPinToregBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint16_t &reg, uint8_t bitNumber)
{
	if ((GPIOx->IDR & GPIO_Pin) != (uint32_t) Bit_RESET)
	{
		reg &= ~(1 << bitNumber);
	}
	else
	{
		reg |= (1 << bitNumber);
	}
}

uint8_t highByte(uint16_t Value)
{
	return (Value >> 8) & 0xFF;
}

uint8_t lowByte(uint16_t Value)
{
	return Value & 0xFF;
}

uint16_t _crc16_update(uint16_t crc, uint8_t a)
{
	int i;

	crc ^= a;
	for (i = 0; i < 8; ++i)
	{
		if (crc & 1)
			crc = (crc >> 1) ^ 0xA001;
		else
			crc = (crc >> 1);
	}

	return crc;
}

int bitRead(uint8_t b, int bitPos)
{
	int x = b & (1 << bitPos);
	return x == 0 ? 0 : 1;
}

uint16_t word(uint8_t high, uint8_t low)
{
	uint16_t ret_val = low;
	ret_val += (high << 8);
	return ret_val;
}

void delayMicroseconds(uint32_t time)
{
	time *= 17;
	while (time > 0)
	{

		time--;
	}

}

/* delay-on function with RTC */
bool ton(bool input, u16 delayTime)
{
	static u32 cmpVal = 0; //setpoint value
	static bool p = 0;     //one cycle var
	bool out = 0;          //output signal(returned)

	if (input == true) //input signal on
	{
		if (p == false) //first cycle
		{
			cmpVal = RTC->TR + delayTime; //set on-value
			p = true;

		} //first cycle

		else //other cycle
		{
			if (RTC->TR >= cmpVal)
			{
				out = true;
			}
			else
			{
				out = false;
			}

		} //other cycle

	} //input signal on

	else //input signal off
	{
		out = false;
		cmpVal = 0;
		p = false;

	} //input signal off

	return out;
}

/* delay-off function with RTC */
bool tof(bool input, u16 delayTime)
{
	static u32 cmpVal = 0; //setpoint value
	static bool p = 0;     //one cycle var
	bool out = 0;          //output signal(returned)

	if (input == true) //input signal on
	{
		out = true;
		cmpVal = 0;
		p = false;
	} //input signal on

	else //input signal off
	{
		if (p == false) //first cycle
		{
			cmpVal = RTC->TR + delayTime; //set off-value
			p = true;

		} //first cycle

		else //other cycle
		{
			if (RTC->TR >= cmpVal)
			{
				out = false;

			}
			else
			{
				out = true;
			}

		} //other cycle

	} //input signal off

	return out;
}

bool tpulse(void)
{
	return  RTC->TR & 0x00000001;
}

void initRTC(void){


	RTC_InitTypeDef RTC_InitStructure;

		/* Enable the PWR clock */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

		/* Allow access to RTC */
		PWR_BackupAccessCmd(ENABLE);

		/* LSI used as RTC source clock */
		/* The RTC Clock may varies due to LSI frequency dispersion. */
		/* Enable the LSI OSC */
		RCC_LSICmd(ENABLE);

		/* Wait till LSI is ready */
		while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
		{
		}

		/* Select the RTC Clock Source */
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

		/* Enable the RTC Clock */
		RCC_RTCCLKCmd(ENABLE);

		/* Wait for RTC APB registers synchronisation */
		RTC_WaitForSynchro();

		/* Calendar Configuration with LSI supposed at 32KHz */
		RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
		RTC_InitStructure.RTC_SynchPrediv = 0xFF; /* (32KHz / 128) - 1 = 0xFF*/
		RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
		RTC_Init(&RTC_InitStructure);



}


void send_USART_dma( uint8_t *buff, uint8_t size)
{

	DMA_DeInit(DMA2_Stream6);

	DMA_InitTypeDef DMA_InitStructure;
	/* Configure DMA Initialization Structure */
	DMA_InitStructure.DMA_BufferSize = size;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) (&(USART6->DR));
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	/* Configure TX DMA */
	DMA_InitStructure.DMA_Channel = DMA_Channel_5;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) buff;
	DMA_Init(DMA2_Stream6, &DMA_InitStructure);

	DMA_Cmd(DMA2_Stream6, ENABLE);
	USART_DMACmd(USART6, USART_DMAReq_Tx, ENABLE);

}
