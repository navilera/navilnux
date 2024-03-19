/*================================================================================================

  화일명 : gpio.S
  설  명 : EZ-X5 의 GPIO를 제어 하는 루틴이다.
  
  작성자 : (주)제이닷디앤티  푸지
  작성일 : 2003년 4월 21일
  
  비  고 :  
            
==================================================================================================*/

#include 	"ez_x5.h"
		
.text

//------------------------------------------------------------------------------------------------
//
//  gpio 초기화
//
//------------------------------------------------------------------------------------------------
.globl gpio_init
gpio_init:
		// GPIO Alternate function
		ldr	r0, =PXA_REG_GP_BASE

		ldr	r1, =GAFR0_L_VALUE
		str	r1, [r0, #PXA_REG_OFFSET_GAFR0_L]
		ldr	r1, =GAFR0_U_VALUE
		str	r1, [r0, #PXA_REG_OFFSET_GAFR0_U]
			
		ldr	r1, =GAFR1_L_VALUE
		str	r1, [r0, #PXA_REG_OFFSET_GAFR1_L]
		ldr	r1, =GAFR1_U_VALUE
		str	r1, [r0, #PXA_REG_OFFSET_GAFR1_U]

		ldr	r1, =GAFR2_L_VALUE
		str	r1, [r0, #PXA_REG_OFFSET_GAFR2_L]
		ldr	r1, =GAFR2_U_VALUE
		str	r1, [r0, #PXA_REG_OFFSET_GAFR2_U]

		ldr	r1, =GPDR0_VALUE
		str	r1, [r0, #PXA_REG_OFFSET_GPDR0]
		ldr	r1, =GPDR1_VALUE
		str	r1, [r0, #PXA_REG_OFFSET_GPDR1]
		ldr	r1, =GPDR2_VALUE
		str	r1, [r0, #PXA_REG_OFFSET_GPDR2]

		// PSSR 설정 - GPIO 입력 활성화
		ldr	r0, =PXA_REG_PSSR
		ldr	r1, =PSSR_VALUE
		str	r1, [r0]

		// debug led off
		ldr	r0, =PXA_REG_GP_BASE
		mov	r1, #(_LED_3|_LED_2|_LED_1|_LED_0)
		str	r1, [r0, #PXA_REG_OFFSET_GPSR0]

		mov	pc, lr

//------------------------------------------------------------------------------------------------
//
//   debug LED 를 켜거나 끈다
//
//	r5 : 상태값이 넘어온다
//
//------------------------------------------------------------------------------------------------
.globl led_out
led_out:
		ldr	r0, =PXA_REG_GP_BASE
		
		// led 0,1,2 off
		mov	r1, #(_LED_2|_LED_1|_LED_0)
		str	r1, [r0, #PXA_REG_OFFSET_GPSR0]

		// state on
		mov	r1, #0
		
		and	r4, r5, #0x4
		cmp	r4, #0
		beq	10f
		orr	r1, r1, #_LED_2

10:		and	r4, r5, #0x2
		cmp	r4, #0
		beq	20f
		orr	r1, r1, #_LED_1

20:		and	r4, r5, #0x1
		cmp	r4, #0
		beq	30f
		orr	r1, r1, #_LED_0		

30:		str	r1, [r0, #PXA_REG_OFFSET_GPCR0]
		
		mov	pc, lr

//------------------------------------------------------------------------------------------------
//
//  LED를 점멸 시킨다. 이 루틴은 무한 루틴으로 빠진다. 
//
//------------------------------------------------------------------------------------------------
.globl error_loop
error_loop:           

		ldr	r0, =PXA_REG_GP_BASE
		mov	r1, #ERROR_LED
		
10:		str	r1, [r0, #PXA_REG_OFFSET_GPCR0]

		mov	r4, #0x800000
20:		nop
		nop
		subs	r4, r4, #1
		bne	20b

		str	r1, [r0, #PXA_REG_OFFSET_GPSR0]
		
		mov	r4, #0x800000
30:		nop
		nop
		subs	r4, r4, #1
		bne	30b
		
		b	10b



