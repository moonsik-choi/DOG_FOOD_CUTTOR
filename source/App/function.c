
/* Made by K.S.W. */

/*==[ 필수 include 파일 ]==*/
#include "main.h"


/*==[ 사용자 정의 ]==*/
#define		DN_BR_NOT_OFF			0
#define		DN_BR_OFF				0					// OFF
#define		DN_BR_MIN				0					// 밝기 최소값
#define		DN_BR_MAX_VAR			0					// gu8BrightVar 변수의 최소값.
#define		DN_BR_MAX				gu8Bright			// 밝기 최대값
#define		DN_DUTY_MAX				100	
#define		DF_SPD_SCALE			((float)(1.0))		// 0.1~2.5까지(숫자가 커지면 디밍과 점멸 속도가 느려짐)
#define		DN_SPD_VALUE			60					// 30*100ms = 3s

#define 	DN_ADC_HALL_THRESH		500					// Hall sensor detect threshold//((40950/33)*29)

#define		DT_SHORT_KEY_TIME		30					// ms단위
#define		DT_MIDDLE_KEY_TIME		1000				// ms단위
#define		DT_LONG_KEY_TIME		5000				// ms단위
#define		DV_SHORT_KEY_VALUE		0x10
#define		DV_MIDDLE_KEY_VALUE		0x20
#define		DV_LONG_KEY_VALUE		0x30

/*==[ 매크로 정의 ]==*/
#define M_DIMM_LED_LH(var)			Play_Dimm_L_to_H(&var, &gu32DispLED_SPD_Cnt, &gu32LED_BR, &gu32DispLed_ST_Cnt)
#define M_DIMM_LED_HL(var)			Play_Dimm_H_to_L(&var, &gu32DispLED_SPD_Cnt, &gu32LED_BR, &gu32DispLed_ST_Cnt)
#define M_LED_OFF()					gu32LED_BR = DN_BR_OFF

/*==[ 전역 변수 함수 ]==*/
/*************************************************************************************/


uint32_t	gu32Counter1ms;								// 1ms 카운터
uint32_t	gu32DispTimeCounter;						// 모드별 디스플레이 변화 시간 카운트 변수
uint32_t	gu32KeyTimeCounter;

uint8_t 	Flag_1ms;									// 1ms마다 TIM3에서 Set
uint8_t 	gu8Setp;
uint8_t 	gu8KeyValue;

uint16_t 	gu16ADC_value;

uint8_t 	gu8_ADC_Flag;
uint8_t 	gu8Speed = DN_SPD_VALUE;					// 30*100ms = 3s
uint8_t 	gu8Bright = 100;

uint8_t 	gu8PowerOnFlag;


uint32_t	gu32DispLed_ST_Cnt;							// 연속동작을 위한 Step Counter (사용안함)
uint32_t	gu32DispLED_BR_Cnt;							// LED 밝기 카운트 변수
uint32_t	gu32DispLED_SPD_Cnt;						// LED 디밍 속도 카운트 변수
uint32_t	gu32LED_BR;									// LED 밝기 저장 변수 0~100


/*************************************************************************************/


uint16_t AD_CHK_16(uint8_t);
uint16_t AD_CHK_2(uint8_t);

void Process( void );

void Key_Input(void);

void Play_FMode_main(uint8_t);

void ADC_Control(void);

uint8_t Play_Speed_Higher(uint32_t* SPD_Counter, uint32_t* BRIGHT_Value, uint32_t* CNT_Value);
uint8_t Play_Speed_Lower(uint32_t* SPD_Counter, uint32_t* BRIGHT_Value, uint32_t* CNT_Value);
void Play_Dimm_L_to_H(uint8_t* u8step, uint32_t* SPD_Counter, uint32_t* BRIGHT_Value, uint32_t* CNT_Value);
void Play_Dimm_H_to_L(uint8_t* u8step, uint32_t* SPD_Counter, uint32_t* BRIGHT_Value, uint32_t* CNT_Value);

/*************************************************************************************/

/****************************************************************************************
 * Module	: Main동작 제어
 ****************************************************************************************
 * Function : Process
 * Input	: none
 * Output	: none
 * Note 	: 모드 변환에 따른 디스플레이 변환 지령
 ****************************************************************************************/

void Process( void )
{
	HAL_WDT_ReloadTimeCounter();									// WDT kick (3sec timeout)

	if(!gu8_ADC_Flag)
	{
		ADC_Control();
		gu8_ADC_Flag = 1;
	}

	if(Flag_1ms)
	{
		Flag_1ms = 0;
		gu8_ADC_Flag = 0;
		gu32Counter1ms++;
		gu32DispTimeCounter++;

		Key_Input();
		if((gu8KeyValue == DV_SHORT_KEY_VALUE) || (gu8KeyValue == DV_LONG_KEY_VALUE))
		{
			if((gu8PowerOnFlag & 0x01) == 0)				// OFF -> ON
			{
				gu8PowerOnFlag = 0x01;
			}
			else												// ON -> Stop request
			{
				gu8PowerOnFlag |= 0x02;					// bit1: Hall waiting
			}
			gu8KeyValue = 0;
		}

		if((gu8PowerOnFlag & 0x01) == 0x01)			// Motor running
		{
			gu32LED_BR = DN_BR_MAX;						// Motor ON (100%)

			if((gu8PowerOnFlag & 0x02) == 0x02)		// Stop requested
			{
				if(gu16ADC_value > DN_ADC_HALL_THRESH)	// Hall sensor detected
				{
					gu32LED_BR = DN_BR_OFF;				// Motor OFF
					gu8PowerOnFlag = 0;					// Clear all flags
				}
			}
		}
		else
		{
			gu32LED_BR = DN_BR_OFF;						// Motor OFF
			gu8PowerOnFlag = 0;
		}
	}
}



void Variable_Init(void)												// 동작 모드가 바뀔때, 바로 LED동작이 바뀌게 하기 위함.
{
	gu32DispLED_BR_Cnt = 0;
	gu32DispLED_SPD_Cnt = 0;
	gu32LED_BR = 0;
	gu8Setp = 0;
	
}


/****************************************************************************************
 * Module	: ADC 센싱
 ****************************************************************************************
 * Function : ADC_Control
 * Input	: None
 * Output: None
 * Note 	: None
 ****************************************************************************************/
void ADC_Control(void)
{
	gu16ADC_value = AD_CHK_16(DAI_CH_CURRENT);
}

/****************************************************************************************
 * Module	: Key 입력 제어
 ****************************************************************************************
 * Function : Key_Input
 * Input	: DEF_PIN_SW
 * Output	: gu8KeyValue
 * Note 	: Key 1개의 눌린 시간에 따른 키값 저장.
 ****************************************************************************************/
void Key_Input(void)
{
	// push button switch
	if(DPI_KEY == D_KEY_ON_LEVEL)
	{
		gu32KeyTimeCounter++;
		if(gu32KeyTimeCounter == DT_LONG_KEY_TIME)				// Key를 누르고 있을때 동작.
		{
			gu32KeyTimeCounter++;													// 계속 누르고 있을때, 추가 키동작을 막기 위함.
			gu8KeyValue = DV_LONG_KEY_VALUE;
		}
	}
	else
	{
		if(gu32KeyTimeCounter > DT_LONG_KEY_TIME)					// Key를 뗏을때 동작. 롱키
		{
//			gu8KeyValue = DV_LONG_KEY_VALUE;
		}
//		else if(gu32KeyTimeCounter > DT_MIDDLE_KEY_TIME)
//		{
//			gu8KeyValue = DV_MIDDLE_KEY_VALUE;
//		}
		else if(gu32KeyTimeCounter > DT_SHORT_KEY_TIME)		// Key를 뗏을때 동작. 숏키
		{
			gu8KeyValue = DV_SHORT_KEY_VALUE;
		}
		else
		{
			gu8KeyValue = 0;
		}
		gu32KeyTimeCounter = 0;
	}
	
}




// 1ms마다 Speed값(1~100)에 따라 BR값을 올려줌.
uint8_t Play_Speed_Higher(uint32_t* SPD_Counter, uint32_t* BRIGHT_Value, uint32_t* CNT_Value)		// LED Dimming 함수 1ms마다 실행
{
	(*SPD_Counter)++;						// 1ms마다 증가.
	if((*SPD_Counter) >= (gu8Speed))		// Speed 값 1~100
	{
		(*CNT_Value)++;
		(*BRIGHT_Value) = ((*CNT_Value) * DN_BR_MAX) / DN_DUTY_MAX;
		(*SPD_Counter) = 0;
		if((*CNT_Value) >= DN_DUTY_MAX)
		{
			return 1;						// 현재 스텝 종료.
		}
	}
	return 0;								// 현재 스텝 유지.
}

// 1ms마다 Speed값(1~100)에 따라 BR값을 내려줌.
uint8_t Play_Speed_Lower(uint32_t* SPD_Counter, uint32_t* BRIGHT_Value, uint32_t* CNT_Value)		// LED Dimming 함수 1ms마다 실행
{
	(*SPD_Counter)++;						// 1ms마다 증가.
	if((*SPD_Counter) >= (gu8Speed))		// Speed 값 1~100
	{
		(*CNT_Value)--;
		if((*CNT_Value) >= DN_DUTY_MAX)		// 언더플로우방지.
		{
			(*CNT_Value) = 0;
		}
		(*BRIGHT_Value) = ((*CNT_Value) * DN_BR_MAX) / DN_DUTY_MAX;

		(*SPD_Counter) = 0;
		if((*CNT_Value) == (DN_BR_MIN - DN_BR_NOT_OFF))
		{
			return 1;						// 현재 스텝 종료.
		}
	}
	return 0;								// 현재 스텝 유지.
}

//디밍 L to H를 0->100으로 할때
//void Play_Dimm_L_to_H(uint8_t* u8step, uint32_t* SPD_Counter, uint32_t* BRIGHT_Value, uint32_t* CNT_Value)
//{
//	if((*u8step) % 2 == 0)					// 디밍전 초기값 설정.
//	{
//		(*SPD_Counter) = 0;
//		(*CNT_Value) = 0;
//		(*BRIGHT_Value) = DN_BR_MIN;
//		(*u8step)++;
//	}
//	if((*u8step) % 2 == 1)					// 디밍 실행, 밝기 업
//	{
//		if(Play_Speed_Higher(SPD_Counter, BRIGHT_Value, CNT_Value) == 1)
//		{
//			(*u8step)++;
//		}
//	}
//}

void Play_Dimm_L_to_H(uint8_t* u8step, uint32_t* SPD_Counter, uint32_t* BRIGHT_Value, uint32_t* CNT_Value)
{
    if((*u8step) % 2 == 0)                    // 디밍전 초기값 설정.
    {
        (*SPD_Counter) = 0;

        // ★ 여기 두 줄을 0 → 50%에서 디밍을 시작하게 변경
        (*CNT_Value)    = DN_DUTY_MAX / 2;    // 예: 100/2 = 50
        (*BRIGHT_Value) = DN_BR_MAX  / 2;     // 예: 100/2 = 50 (% 기준)

        (*u8step)++;
    }
    if((*u8step) % 2 == 1)                    // 디밍 실행, 밝기 업
    {
        if(Play_Speed_Higher(SPD_Counter, BRIGHT_Value, CNT_Value) == 1)
        {
            (*u8step)++;
        }
    }
}


void Play_Dimm_H_to_L(uint8_t* u8step, uint32_t* SPD_Counter, uint32_t* BRIGHT_Value, uint32_t* CNT_Value)
{
	if(*u8step % 2 == 0)					// 디밍전 초기값 설정.
	{
		(*SPD_Counter) = 0;
		(*CNT_Value) = DN_DUTY_MAX;
		(*BRIGHT_Value) = DN_BR_MAX;
		(*u8step)++;
	}
	else if(*u8step % 2 == 1)				// 디밍 실행, 밝기 다운
	{
		if(Play_Speed_Lower(SPD_Counter, BRIGHT_Value, CNT_Value) == 1)
		{
			(*u8step)++;
		}
	}
}



uint16_t AD_CHK_16(uint8_t ch)
{
	unsigned int AD_v_temp=0;
	unsigned int AD_repeat;

	ADC_SET_CHANNEL(ch);
	for(AD_repeat=16;AD_repeat>0;AD_repeat--)
	{

		ADC_START(1);
		while(ADC_EOCF());
		AD_v_temp += (ADC_GET_DATA())>>0;				// 12bit Use
	}

	return (AD_v_temp >> 4);
}

uint16_t AD_CHK_2(uint8_t ch)
{
	unsigned int AD_v_temp=0;
	unsigned int AD_repeat;
	
	ADC_SET_CHANNEL(ch);
	for(AD_repeat=2;AD_repeat>0;AD_repeat--)
	{

		ADC_START(1);
		while(ADC_EOCF());
		AD_v_temp += (ADC_GET_DATA())>>0;				// 12bit Use
	}

	return (AD_v_temp >> 1);
}


/* End of file */
