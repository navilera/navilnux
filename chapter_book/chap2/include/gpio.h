//------------------------------------------------------------------------------
// 파 일 명 : gpio.h
// 프로젝트 : ezboot
// 설    명 : ezboot에서 사용하는 gpio에 관련된 헤더 
// 작 성 자 : 유영창
// 작 성 일 : 2001년 11월 3일
// 수 정 일 : 
// 주    의 : 
//------------------------------------------------------------------------------

#ifndef _GPIO_HEADER_
#define _GPIO_HEADER_

#define	LED_0			GPIO_bit(2)
#define	LED_1			GPIO_bit(3)
#define	LED_2			GPIO_bit(4)
#define	LED_3			GPIO_bit(5)	// 

#define	LED_OFF		0
#define	LED_ON			1

// GPIO0 Switch 관련
#define GPIO_SWITCH			GPIO_bit(0)		// GPIO0

#define MASK_GPIO_SWITCH	( 1 << GPIO_SWITCH )

#define GPIO_INPUT_MASK		( GPIO_SWITCH )
#define GPIO_GAFR0_MASK		( 0 | 1 )		
//


void  set_GPIO_mode(int gpio_mode);
void  GPIOInit(void); 				// GPIO 상태 초기화
void  GPIO_SetLED( int LedIndex, int value ); 	// GPIO 에 연결된 LED를 제어한다.


#endif //_GPIO_HEADER_

