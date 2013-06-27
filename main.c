/**
  ******************************************************************************
  * @file    Project/main.c
  * @author  MCD Application Team
  * @version V2.1.0
  * @date    18-November-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/

#include "main.h"
//#include "input.h"
/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

static void Cpu_to_default_config(void);
static void Ports_to_default_config(void);
static void Timers_to_default_config(void);
static void Main_test_lcd(void);

//static void Buzzer_to_default_config(void);

//void Delay_blocking(__IO uint32_t nCount);


/* Private functions ---------------------------------------------------------*/
//__IO union _STM8_GPIO_IDR_REG *test1PortIDR = (union _STM8_GPIO_IDR_REG*)(GPIOA_BaseAddress+1);


INPUT_Key_Obj gKeys;
INPUT_Key_Handle keyHandle;

CTRL_Obj		gCtrl;
CTRL_Handle	ctrlHandle;

void main(void)
{
	Cpu_to_default_config();
	Ports_to_default_config();

	Timers_to_default_config();

	LCD_to_default_config();
	//

	keyHandle = INPUT_key_init(&gKeys,sizeof(gKeys));
	ctrlHandle = CTLR_init(&gCtrl, sizeof(gCtrl));
	gCtrl.inputHandle = keyHandle;

	while (1)
	{
		Main_background_loop();
	}

}


void Main_background_loop(void)
{

	CTRL_run(ctrlHandle);
	//Main_test_lcd();

	LCD_flush_buf_to_lcd();

}


void MainISR_125us(void)
{
	Buzzer_sound_tickISR();

}

void MainISR_250us(void)
{

}

void MainISR_500us(void)
{

}

void MainISR_1ms(void)
{

}

void MainISR_50ms(void)
{
	Buzzer_off_timing_tick50ms();
	INPUT_key_ISR50ms(keyHandle);
	CTRL_ISR50ms(ctrlHandle);
}

void MainISR_100ms(void)
{
	LCD_blink_ISR100ms();

}

void MainISR_200ms(void)
{

}

void MainISR_300ms(void)
{

}

void MainISR_500ms(void)
{

}

void MainISR_1s(void)
{
	CTRL_ISR1s(ctrlHandle);
}

void MainISR_3s(void)
{

}


//50ms tick
void Timer2_ISR(void)
{
	static _Bool 	flag_50ms;
	static uint8_t cnt_100ms = 30;

	MainISR_50ms();

	if(flag_50ms == 0){
		flag_50ms = 1;
	}else{
		flag_50ms = 0;

		MainISR_100ms();
		cnt_100ms--;
		if(cnt_100ms==0){
			cnt_100ms = 30;
			MainISR_3s();
		}


		if(cnt_100ms%2 == 0)
			MainISR_200ms();
		if(cnt_100ms%3 == 0)
			MainISR_300ms();
		if(cnt_100ms%5 == 0)
			MainISR_500ms();
		if(cnt_100ms%10 == 0)
			MainISR_1s();
	}
}

//125us tick
void Timer4_ISR(void)
{
	static _Bool flag_125us, flag_250us, flag_500us;

	MainISR_125us();

	if(!flag_125us){
		flag_125us = 1;
	}else{
		flag_125us = 0;
		MainISR_250us();
		if(flag_250us == 0){
			flag_250us = 1;
		}else{
			flag_250us = 0;
			MainISR_250us();
			if(flag_500us == 0){
				flag_500us = 1;
				MainISR_500us();
			}else{
				flag_500us = 0;
				MainISR_1ms();
			}
		}


	}
}


static void Cpu_to_default_config(void)
{
	ITC_SetSoftwarePriority(ITC_IRQ_TIM4_OVF, ITC_PRIORITYLEVEL_3);
	ITC_SetSoftwarePriority(ITC_IRQ_TIM2_OVF, ITC_PRIORITYLEVEL_2);

	CLK_DeInit();
	CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1); // CPU Prescaler = 1.
	CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);  // HSI Prescaler = 1  => 16 MHz.
	CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO,  // Automatically switch
						CLK_SOURCE_HSI,	// Switch to HSI
						DISABLE,			// Disable CLK switch Int
						CLK_CURRENTCLOCKSTATE_DISABLE); // Disable the previous clock.
}

static void Ports_to_default_config(void)
{
	GPIO_Init(BLOWER_FAN1_GPIO,(GPIO_Pin_TypeDef)BLOWER_FAN1_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_Init(BLOWER_FAN2_GPIO,(GPIO_Pin_TypeDef)BLOWER_FAN2_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_Init(BLOWER_FAN3_GPIO,(GPIO_Pin_TypeDef)BLOWER_FAN3_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_Init(BLOWER_FAN4_GPIO,(GPIO_Pin_TypeDef)BLOWER_FAN4_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_Init(BLOWER_FAN1_GPIO,(GPIO_Pin_TypeDef)BLOWER_FAN1_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_Init(LAMP_GPIO,(GPIO_Pin_TypeDef)LAMP_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_Init(BUZZER_GPIO,(GPIO_Pin_TypeDef)BUZZER_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(LED_GPIO,(GPIO_Pin_TypeDef)LED_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);

	GPIO_Init(SPI_DATA_GPIO, (GPIO_Pin_TypeDef)SPI_DATA_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(SPI_CLK_GPIO, (GPIO_Pin_TypeDef)SPI_CLK_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(SPI_STB_GPIO, (GPIO_Pin_TypeDef)SPI_STB_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
	SPI_STB_HIGH;

	GPIO_Init(TP1_GPIO, (GPIO_Pin_TypeDef)TP1_PIN, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(TP2_GPIO, (GPIO_Pin_TypeDef)TP2_PIN, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(TP3_GPIO, (GPIO_Pin_TypeDef)TP3_PIN, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(TP4_GPIO, (GPIO_Pin_TypeDef)TP4_PIN, GPIO_MODE_IN_PU_NO_IT);

	//LED_ON;

}



static void Timers_to_default_config(void)
{
	//Timer 4 used for 125us Tick

	//TIM4CLK = 16MHz/16  = 1000kHz
	//Period = 1000k/freq_hz -1
	TIM4_TimeBaseInit(TIM4_PRESCALER_16,(uint8_t)(1000000/8000-1));
	TIM4_ClearFlag(TIM4_FLAG_UPDATE);
	TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);

	//Timer2 used for 50ms Tick
	TIM2_DeInit();
	TIM2_TimeBaseInit(TIM2_PRESCALER_128,(uint16_t)(125000/20-1));
	TIM2_ClearFlag(TIM2_FLAG_UPDATE);
	TIM2_ITConfig(TIM2_IT_UPDATE,ENABLE);

	enableInterrupts();
	TIM2_Cmd(ENABLE);
	TIM4_Cmd(ENABLE);

}






void Delay_blocking(__IO uint32_t nCount)
{
	while(nCount !=0)
	{
		nCount--;
	}
}


static void Main_test_lcd(void)
{
	static uint8_t	tmpNum=0;

	/*
	if(gKeys.keys[INPUT_Key_Power].hold == INPUT_KEY_ACTIVE)
		Buzzer_bip();
	if(gKeys.keys[INPUT_Key_Light].rising_edge== INPUT_KEY_ACTIVE)
		Buzzer_bip();
	if(gKeys.keys[INPUT_Key_Plus].rising_edge== INPUT_KEY_ACTIVE)
		Buzzer_bip();
	if(gKeys.keys[INPUT_Key_Minus].rising_edge== INPUT_KEY_ACTIVE)
		Buzzer_bip();
*/
	if(gKeys.keys[INPUT_Key_Any].rising_edge == INPUT_KEY_ACTIVE)
		Buzzer_bip();
	if(gKeys.keys[INPUT_Key_Any].hold)
		Buzzer_bip();


	if(gKeys.keys[INPUT_Key_Plus].rising_edge){
		if(tmpNum == 99)
			tmpNum = 0;
		else
			tmpNum++;
	}
	if(gKeys.keys[INPUT_Key_Minus].rising_edge){
		if(tmpNum == 0)
			tmpNum = 99;
		else
			tmpNum--;

	}
	LCD_fill_hour1(tmpNum);
	LCD_fill_hour2(tmpNum);
	LCD_fill_min1(tmpNum);
	LCD_fill_min2(tmpNum);
	if(gKeys.keys[INPUT_Key_Power].hold){
		LCD_icon_fan(LCD_get_fan_cursor_med());
	}

	if(gKeys.keys[INPUT_Key_Light].hold){
		LCD_icon_fan(LCD_get_fan_cursor_slow());
	}
	if(gKeys.keys[INPUT_Key_Plus].hold){
		LCD_icon_fan(LCD_get_fan_cursor_fast());
	}
	if(!LCD_get_blink_cursor()){
		LCD_icon_off(LCD_Icon_Colon1);
		LCD_icon_off(LCD_Icon_Colon2);
	}else{
		LCD_icon_on(LCD_Icon_Colon1);
		LCD_icon_on(LCD_Icon_Colon2);
	}

}


#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
