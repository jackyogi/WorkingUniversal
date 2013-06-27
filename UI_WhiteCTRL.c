#include "main.h"



CTRL_Handle CTLR_init(void *pMemory,const size_t numBytes)
{
	CTRL_Handle ctrlHandle;
	CTRL_Obj *ctrl;


	if(numBytes < sizeof(CTRL_Obj))
	return((CTRL_Handle)NULL);

	// assign the handle
	ctrlHandle = (CTRL_Handle)pMemory;

	// assign the object
	ctrl = (CTRL_Obj *)ctrlHandle;

	//set default debounce time
	//ctrl->debounce_time_x50ms = INPUT_KEY_DEBOUNCE_TIME_X50MS;
	//ctrl->hold_activate_time_x50ms = INPUT_KEY_HOLD_ACTIVATE_TIME_X50MS;
	ctrl->version.major = CTRL_VERSION_MAJOR;
	ctrl->version.minor = CTRL_VERSION_MINOR;
	ctrl->state = CTRL_State_Idle;
	ctrl->state_change = 0;
	ctrl->cu_runing_time = 0;
	ctrl->auto_power_off_delay_time = 60;
	ctrl->blower_speed = 1;
	ctrl->blower_last_speed = 1;

	return(ctrlHandle);
}


void CTRL_set_blowerSpeed(CTRL_Handle ctrlHandle, const uint8_t spd)
{
	CTRL_Obj *obj = (CTRL_Obj *)ctrlHandle;

	switch(spd){
	case 1:
		obj->blower_speed = 1;
		BLOWER_FAN2_OFF;
		BLOWER_FAN3_OFF;
		BLOWER_FAN4_OFF;
		BLOWER_FAN1_ON;
		break;
	case 2:
		obj->blower_speed = 2;
		BLOWER_FAN1_OFF;
		BLOWER_FAN3_OFF;
		BLOWER_FAN4_OFF;
		BLOWER_FAN2_ON;
		break;
	case 3:
		obj->blower_speed = 3;
		BLOWER_FAN1_OFF;
		BLOWER_FAN2_OFF;
		BLOWER_FAN4_OFF;
		BLOWER_FAN3_ON;
		break;
	case 4:
		obj->blower_speed = 4;
		BLOWER_FAN1_OFF;
		BLOWER_FAN2_OFF;
		BLOWER_FAN3_OFF;
		BLOWER_FAN4_ON;
		break;
	default:
		obj->blower_speed = 0;
		BLOWER_FAN1_OFF;
		BLOWER_FAN2_OFF;
		BLOWER_FAN3_OFF;
		BLOWER_FAN4_OFF;
		break;
	}
}

uint8_t CTRL_get_blowerSpeed(CTRL_Handle ctrlHandle)
{
	CTRL_Obj *obj = (CTRL_Obj *)ctrlHandle;

	return (obj->blower_speed);
}

void CTRL_increase_blowerSpeed(CTRL_Handle ctrlHandle)
{
	uint8_t  current_blowerSpeed = CTRL_get_blowerSpeed(ctrlHandle);

	if(current_blowerSpeed ==  4)
		CTRL_set_blowerSpeed(ctrlHandle, 1);
	else
		CTRL_set_blowerSpeed(ctrlHandle, ++current_blowerSpeed);

}

void CTRL_decrease_blowerSpeed(CTRL_Handle ctrlHandle)
{
	uint8_t  current_blowerSpeed = CTRL_get_blowerSpeed(ctrlHandle);

	if(current_blowerSpeed == 1)
		CTRL_set_blowerSpeed(ctrlHandle, 4);
	else
		CTRL_set_blowerSpeed(ctrlHandle, --current_blowerSpeed);
}

void CTRL_set_blowerLastSpeed(CTRL_Handle ctrlHandle, const uint8_t spd)
{
	CTRL_Obj *obj = (CTRL_Obj *)ctrlHandle;
	if(spd<5)
		obj->blower_last_speed = spd;
}

uint8_t CTRL_get_blowerLastSpeed(CTRL_Handle ctrlHandle)
{
	CTRL_Obj *obj = (CTRL_Obj *)ctrlHandle;

	return (obj->blower_last_speed);
}


_Bool CTRL_get_lamp_state(CTRL_Handle ctrlHandle)
{
	CTRL_Obj *obj = (CTRL_Obj *)ctrlHandle;

	return(obj->lamp_state);
}

void CTRL_set_lamp(CTRL_Handle ctrlHandle, const _Bool state)
{
	CTRL_Obj *obj = (CTRL_Obj *)ctrlHandle;

	if(state){
		obj->lamp_state = 1;
		LCD_icon_on(LCD_Icon_Light);
		LAMP_ON;
	}else{
		obj->lamp_state = 0;
		LCD_icon_off(LCD_Icon_Light);
		LAMP_OFF;
	}
}

void CTRL_toggle_lamp(CTRL_Handle ctrlHandle)
{
	CTRL_Obj *obj = (CTRL_Obj *)ctrlHandle;

	if(obj->lamp_state)
		CTRL_set_lamp(ctrlHandle, STATE_OFF);
	else
		CTRL_set_lamp(ctrlHandle, STATE_ON);
}

void CTRL_set_led(CTRL_Handle ctrlHandle, const _Bool state)
{
	CTRL_Obj *obj = (CTRL_Obj *)ctrlHandle;

	if(state){
		obj->led_state = 1;
		LED_ON;
	}else{
		obj->led_state = 0;
		LED_OFF;
	}
}

_Bool CTRL_get_led_state(CTRL_Handle ctrlHandle)
{
	CTRL_Obj *obj = (CTRL_Obj *)ctrlHandle;

	return(obj->led_state);
}

CTRL_State_e CTRL_get_state(CTRL_Handle ctrlHandle)
{
	CTRL_Obj *obj = (CTRL_Obj *)ctrlHandle;

	return(obj->state);
}

void CTRL_set_state(CTRL_Handle ctrlHandle, CTRL_State_e state)
{
	CTRL_Obj *obj = (CTRL_Obj *)ctrlHandle;

	if(state == obj->state)
		obj->state_change = 0;
	else
		obj->state_change = 1;

	obj->state = state;
}

_Bool CTRL_check_state_change(CTRL_Handle ctrlHandle)
{
	CTRL_Obj *obj = (CTRL_Obj *)ctrlHandle;

	return(obj->state_change);
}

void CTRL_clear_state_change(CTRL_Handle ctrlHandle)
{
	CTRL_Obj *obj = (CTRL_Obj *)ctrlHandle;

	obj->state_change = 0;
}

uint32_t CTRL_get_cu_running_time(CTRL_Handle ctrlHandle)
{
	CTRL_Obj *obj = (CTRL_Obj *)ctrlHandle;

	return(obj->cu_runing_time);
}

void CTRL_increase_cu_running_time(CTRL_Handle ctrlHandle)
{
	CTRL_Obj *obj = (CTRL_Obj *)ctrlHandle;

	obj->cu_runing_time++;
}

void CTRL_clear_cu_running_time(CTRL_Handle ctrlHandle)
{
	CTRL_Obj *obj = (CTRL_Obj *)ctrlHandle;

	obj->cu_runing_time = 0;
}

uint16_t CTRL_get_auto_power_off_delay_time(CTRL_Handle ctrlHandle)
{
	CTRL_Obj *obj = (CTRL_Obj *)ctrlHandle;

	return(obj->auto_power_off_delay_time);
}

void CTRL_set_auto_power_off_delay_time(CTRL_Handle ctrlHandle, const uint16_t tim)
{
	CTRL_Obj *obj = (CTRL_Obj *)ctrlHandle;

	obj->auto_power_off_delay_time = tim;

}

void CTRL_decrease_auto_power_off_delay_time(CTRL_Handle ctrlHandle)
{
	uint16_t  current_apo_delay_time 
			= CTRL_get_auto_power_off_delay_time(ctrlHandle);

	if(current_apo_delay_time == 60){
		CTRL_set_auto_power_off_delay_time(ctrlHandle, 15*60);
	}else{
		current_apo_delay_time -= 60;
		CTRL_set_auto_power_off_delay_time(ctrlHandle, current_apo_delay_time);
	}
}

void CTRL_increase_auto_power_off_delay_time(CTRL_Handle ctrlHandle)
{
	uint16_t  current_apo_delay_time 
			= CTRL_get_auto_power_off_delay_time(ctrlHandle);

	if(current_apo_delay_time == 15*60){
		CTRL_set_auto_power_off_delay_time(ctrlHandle, 60);
	}else{
		current_apo_delay_time += 60;
		CTRL_set_auto_power_off_delay_time(ctrlHandle, current_apo_delay_time);
	}
}


void CTRL_set_auto_power_off_remain_time(CTRL_Handle ctrlHandle, const uint16_t tim)
{
	CTRL_Obj *obj = (CTRL_Obj *)ctrlHandle;

	obj->auto_power_off_remain_time = tim;
}

uint16_t CTRL_get_auto_power_off_remain_time(CTRL_Handle ctrlHandle)
{
	CTRL_Obj *obj = (CTRL_Obj *)ctrlHandle;

	return(obj->auto_power_off_remain_time);
}

void CTRL_decrease_auto_power_off_remain_time(CTRL_Handle ctrlHandle)
{
	uint16_t  current_apo_remain_time 
			= CTRL_get_auto_power_off_remain_time(ctrlHandle);

	if(current_apo_remain_time != 0)
		CTRL_set_auto_power_off_remain_time(ctrlHandle, --current_apo_remain_time);
}

_Bool CTRL_check_short_tick(CTRL_Handle ctrlHandle)
{
	CTRL_Obj *obj = (CTRL_Obj *)ctrlHandle;

	if(obj->short_tick_cnt == 0){
		obj->short_tick_cnt = CTRL_SHORT_TICK_LENGH_50MS;
		return (1);
	}else{
		return (0);
	}
}

_Bool CTRL_check_long_tick(CTRL_Handle ctrlHandle)
{
	CTRL_Obj *obj = (CTRL_Obj *)ctrlHandle;

	if(obj->long_tick_cnt== 0){
		obj->long_tick_cnt = CTRL_LONG_TICK_LENGH_50MS;
		return (1);
	}else{
		return (0);
	}
}

void CTRL_clear_long_tick(CTRL_Handle ctrlHandle)
{
	CTRL_Obj *obj = (CTRL_Obj *)ctrlHandle;

	obj->long_tick_cnt = CTRL_LONG_TICK_LENGH_50MS;
}

void CTRL_reset_running_cnt(CTRL_Handle ctrlHandle)
{
	CTRL_Obj *obj = (CTRL_Obj *)ctrlHandle;

	obj->running_time_cnt = CTRL_RUNNING_AUTO_POWER_OFF_SEC;
}

void CTRL_ISR1s(CTRL_Handle ctrlHandle)
{
	CTRL_Obj *obj = (CTRL_Obj *)ctrlHandle;
	CTRL_State_e current_state = CTRL_get_state(ctrlHandle);

	if( (current_state == CTRL_State_Blowing)
		|| (current_state == CTRL_State_Blowing_Auto_Power_Off))
	{
		CTRL_increase_cu_running_time(ctrlHandle);
		if(obj->running_time_cnt==0){
			Buzzer_bip();
			CTRL_set_state(ctrlHandle,CTRL_State_Idle);
		}else{
			obj->running_time_cnt--;
		}
	}

	
	
	if(current_state == CTRL_State_Blowing_Auto_Power_Off)
		CTRL_decrease_auto_power_off_remain_time(ctrlHandle);
}

void CTRL_ISR50ms(CTRL_Handle ctrlHandle)
{
	CTRL_Obj *obj = (CTRL_Obj *)ctrlHandle;

	if(obj->short_tick_cnt != 0){
		obj->short_tick_cnt--;
	}
	if(obj->long_tick_cnt != 0){
		obj->long_tick_cnt--;
	}
}

void CTRL_run(CTRL_Handle ctrlHandle)
{
	CTRL_Obj *obj = (CTRL_Obj *)ctrlHandle;
	//INPUT_Key_Obj *keyObj = (INPUT_Key_Obj *)(obj->inputHandle);
	//last speed is the blowing speed of previous run
	uint8_t current_blowerSpeed = CTRL_get_blowerSpeed(ctrlHandle);
	uint8_t last_blowerSpeed	   = CTRL_get_blowerLastSpeed(ctrlHandle);
	_Bool 	short_tick	   = CTRL_check_short_tick(ctrlHandle);
	_Bool	long_tick		   = CTRL_check_long_tick(ctrlHandle);

	INPUT_key_run(obj->inputHandle);
	
	//control of main lamp
	if((CTRL_get_state(ctrlHandle)!= CTRL_State_Clk_Adj)
		&& (INPUT_key_check(obj->inputHandle, INPUT_Key_Light)))
	{
		Buzzer_bip();
		CTRL_toggle_lamp(ctrlHandle);
	}

	//control of Backlight LED
	if((CTRL_get_state(ctrlHandle) == CTRL_State_Idle)
		&& (CTRL_get_lamp_state(ctrlHandle)== STATE_OFF)
		&& (!INPUT_key_check_holding(obj->inputHandle,INPUT_Key_Any)))
	{
		CTRL_set_led(ctrlHandle, STATE_OFF);
	}else{
		CTRL_set_led(ctrlHandle, STATE_ON);
	}

	//reset running time counter if anykey is touched
	if(INPUT_key_check_touching(obj->inputHandle,INPUT_Key_Any)){
		CTRL_reset_running_cnt(ctrlHandle);
	}
	
	//control state
	switch(CTRL_get_state(ctrlHandle)){

	case CTRL_State_Auto_Power_Off_Delay_Time_Adj:
		CTRL_set_blowerSpeed(ctrlHandle, 0);

		LCD_fill_min2(obj->auto_power_off_delay_time%60);
		if(LCD_get_blink_cursor()
			|| (INPUT_key_check_touching(obj->inputHandle,INPUT_Key_Any)))
		{
			LCD_fill_hour2(obj->auto_power_off_delay_time/60);
			LCD_icon_on(LCD_Icon_Clock);
		}else{
			LCD_clear_hour2();
			LCD_icon_off(LCD_Icon_Clock);
		}
		LCD_icon_on(LCD_Icon_Colon2);

		LCD_clear_hour1();
		LCD_clear_min1();
		LCD_clear_speed();
		LCD_icon_off(LCD_Icon_Colon1);
		LCD_clear_fan();

		//***** clear long tick if any key touched
		if(INPUT_key_check_touching(obj->inputHandle,INPUT_Key_Any))
			CTRL_clear_long_tick(ctrlHandle);

		//***** check timeout then change to idle
		if(long_tick
			&&(!CTRL_check_state_change(ctrlHandle)))
		{
			Buzzer_bip();
			CTRL_set_state(ctrlHandle, CTRL_State_Idle);
		}
		
		//*****check keys
		//touch Plus key, increase apo delay time
		if(INPUT_key_check(obj->inputHandle,INPUT_Key_Plus)){
			Buzzer_bip();
			CTRL_increase_auto_power_off_delay_time(ctrlHandle);
		}
		
		//Hold Plus key
		if(INPUT_key_check_holding(obj->inputHandle,INPUT_Key_Plus)
			&& short_tick)
		{
			Buzzer_bip();
			CTRL_increase_auto_power_off_delay_time(ctrlHandle);
		}
		
		//touch Minus key, decrease apo time
		if(INPUT_key_check(obj->inputHandle,INPUT_Key_Minus)){
			Buzzer_bip();
			CTRL_decrease_auto_power_off_delay_time(ctrlHandle);
		}

		//hold Minus key, decrease APO time
		if((INPUT_key_check_holding(obj->inputHandle,INPUT_Key_Minus))
			&& (!CTRL_check_state_change(ctrlHandle))
			&& short_tick)
		{
			Buzzer_bip();
			CTRL_decrease_auto_power_off_delay_time(ctrlHandle);
		}

		//touch Power Key, change to idle
		if(INPUT_key_check(obj->inputHandle, INPUT_Key_Power)){
			Buzzer_bip();
			CTRL_set_state(ctrlHandle, CTRL_State_Idle);
		}
		
		
		

		//*****finalize
		if(INPUT_key_check_up(obj->inputHandle,INPUT_Key_Minus)
			&& CTRL_check_state_change(ctrlHandle))
		{
			CTRL_clear_state_change(ctrlHandle);
			CTRL_clear_long_tick(ctrlHandle); //begin counting long tick
		}
		break;
	case CTRL_State_Idle:
		CTRL_set_blowerSpeed(ctrlHandle, 0);

		LCD_fill_hour1(obj->cu_runing_time/3600);
		LCD_fill_min1(obj->cu_runing_time/60);
		LCD_fill_hour2(obj->auto_power_off_delay_time/60);
		LCD_fill_min2(obj->auto_power_off_delay_time%60);
		LCD_icon_on(LCD_Icon_Colon1);

		//LCD_clear_min2();
		//LCD_clear_hour2();
		LCD_clear_speed();
		LCD_icon_off(LCD_Icon_Colon2);
		LCD_icon_off(LCD_Icon_Clock);
		LCD_clear_fan();

		//*****check keys
		//touch Power Key, change to blowing
		if(INPUT_key_check(obj->inputHandle, INPUT_Key_Power)){
			Buzzer_bip();
			CTRL_set_state(ctrlHandle, CTRL_State_Blowing);
			CTRL_set_blowerSpeed(ctrlHandle, last_blowerSpeed);
		}
		//hold Plus key, clear cummulative running time & change to blowing
		if(INPUT_key_check_holding(obj->inputHandle, INPUT_Key_Plus)){
			Buzzer_bip();
			CTRL_clear_cu_running_time(ctrlHandle);
			CTRL_set_state(ctrlHandle, CTRL_State_Blowing);
			CTRL_set_blowerSpeed(ctrlHandle, last_blowerSpeed);
		}
		//hold Minus key, change to adjust Auto Power Off Time
		if(INPUT_key_check_holding(obj->inputHandle, INPUT_Key_Minus)){
			Buzzer_bip();
			CTRL_set_state(ctrlHandle, CTRL_State_Auto_Power_Off_Delay_Time_Adj);
			
		}
		break;

	case CTRL_State_Blowing:
		if((obj->cu_runing_time<108000)
			|| LCD_get_blink_cursor() )
		{
			LCD_fill_min1((obj->cu_runing_time/60)%60);
			LCD_fill_hour1(obj->cu_runing_time/3600);
		}else{
			LCD_clear_min1();
			LCD_clear_hour1();
		}
		LCD_fill_speed(current_blowerSpeed);
		LCD_icon_rotate_fan(current_blowerSpeed);

		LCD_clear_min2();
		LCD_clear_hour2();
		LCD_icon_off(LCD_Icon_Colon2);
		LCD_icon_off(LCD_Icon_Clock);

		if(LCD_get_blink_cursor()){
			LCD_icon_on(LCD_Icon_Colon1);
		}else{
			LCD_icon_off(LCD_Icon_Colon1);
		}

		

		//*****check keys
		//touch Plus key, increase blowerSpeed
		if(INPUT_key_check(obj->inputHandle,INPUT_Key_Plus)){
			Buzzer_bip();
			CTRL_increase_blowerSpeed(ctrlHandle);
		}

		//touch Minus key, decrease blowerSpeed
		if(INPUT_key_check(obj->inputHandle,INPUT_Key_Minus)){
			Buzzer_bip();
			CTRL_decrease_blowerSpeed(ctrlHandle);
		}

		//release Power key, change to idle
		if((INPUT_key_check_up(obj->inputHandle,INPUT_Key_Power))
			&& (!CTRL_check_state_change(ctrlHandle)))
		{
			Buzzer_bip();
			CTRL_set_state(ctrlHandle,CTRL_State_Idle);
		}

		//hold Power key, change to Blower Auto Power Off
		if((INPUT_key_check_holding(obj->inputHandle,INPUT_Key_Power))
			&& (!CTRL_check_state_change(ctrlHandle)))
		{
			Buzzer_bip();
			CTRL_set_state(ctrlHandle,CTRL_State_Blowing_Auto_Power_Off);
			CTRL_set_auto_power_off_remain_time(ctrlHandle, 
										CTRL_get_auto_power_off_delay_time(ctrlHandle));
		}

		//*****finalize
		if(INPUT_key_check_up(obj->inputHandle,INPUT_Key_Power)
			&& CTRL_check_state_change(ctrlHandle))

		{
			CTRL_clear_state_change(ctrlHandle);
		}
		//update  last speed to current speed
		CTRL_set_blowerLastSpeed(ctrlHandle,current_blowerSpeed);
		break;

	case CTRL_State_Blowing_Auto_Power_Off:
		if((obj->cu_runing_time<108000)
			|| LCD_get_blink_cursor() )
		{
			LCD_fill_min1((obj->cu_runing_time/60)%60);
			LCD_fill_hour1(obj->cu_runing_time/3600);
		}else{
			LCD_clear_min1();
			LCD_clear_hour1();
		}
		LCD_fill_hour2(obj->auto_power_off_remain_time/60);
		LCD_fill_min2(obj->auto_power_off_remain_time%60);
		LCD_fill_speed(current_blowerSpeed);
		LCD_icon_rotate_fan(current_blowerSpeed);
		LCD_icon_on(LCD_Icon_Clock);
		
		if(LCD_get_blink_cursor()){
			LCD_icon_on(LCD_Icon_Colon1);
			LCD_icon_on(LCD_Icon_Colon2);
		}else{
			LCD_icon_off(LCD_Icon_Colon1);
			LCD_icon_off(LCD_Icon_Colon2);
		}

		//check time out
		if(CTRL_get_auto_power_off_remain_time(ctrlHandle)==0){
			Buzzer_bip();
			CTRL_set_state(ctrlHandle,CTRL_State_Idle);
		}
			
		//*****check keys
		//touch Plus key, increase blowerSpeed
		if(INPUT_key_check(obj->inputHandle,INPUT_Key_Plus)){
			Buzzer_bip();
			CTRL_increase_blowerSpeed(ctrlHandle);
		}

		//touch Minus key, decrease blowerSpeed
		if(INPUT_key_check(obj->inputHandle,INPUT_Key_Minus)){
			Buzzer_bip();
			CTRL_decrease_blowerSpeed(ctrlHandle);
		}

		//release Power key, change to idle
		if((INPUT_key_check_up(obj->inputHandle,INPUT_Key_Power))
			&& (!CTRL_check_state_change(ctrlHandle)))
		{
			Buzzer_bip();
			CTRL_set_state(ctrlHandle,CTRL_State_Idle);
		}




		//*****finalize
		if(INPUT_key_check_up(obj->inputHandle,INPUT_Key_Power)
			&& CTRL_check_state_change(ctrlHandle))

		{
			CTRL_clear_state_change(ctrlHandle);
		}
		//update  last speed to current speed
		CTRL_set_blowerLastSpeed(ctrlHandle,current_blowerSpeed);
		break;

	default:
		CTRL_set_state(ctrlHandle, CTRL_State_Idle);
		break;
	};


}
