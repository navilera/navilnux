/*================================================================================================

  화일명 : memory.S
  설  명 : EZ BOOT의 리셋 후 램에 관려된 처리 루틴이다.
  
  작성자 : (주)제이닷디앤티  푸지
  작성일 : 2003년 4월 21일

  수  정 : 2003-09-24	오재경(푸지)
  		pcmcia 레지스터 설정방법 수정
  
  비  고 :  

=================================================================================================*/

#include 	<ez_x5.h>

.text

//------------------------------------------------------------------------------------------------
//
//  메모리 설정을 초기화 한다. 
//
//------------------------------------------------------------------------------------------------
.globl mem_config
mem_config:
		// OSCC 설정
		ldr	r0, =PXA_REG_OSCC
		ldr	r1, =OSCC_VALUE
		str	r1, [r0]

		// MSC 0 설정
		ldr	r0, =PXA_REG_MSC0
		ldr	r1, =MSC0_VALUE
		str	r1, [r0]
		ldr	r1, [r0]	// must be read after it is written
		
		// MSC 1 설정
		ldr	r0, =PXA_REG_MSC1
		ldr	r1, =MSC1_VALUE
		str	r1, [r0]
		ldr	r1, [r0]	// must be read after it is written
		
		// MSC 2 설정
		ldr	r0, =PXA_REG_MSC2
		ldr	r1, =MSC2_VALUE
		str	r1, [r0]
		ldr	r1, [r0]	// must be read after it is written

		// PCMCIA reg MECR
		ldr	r0, =PXA_REG_MECR
		ldr	r1, =MECR_VALUE
		str	r1, [r0]
		ldr	r1, [r0]	// write and then read

		// PCMCIA 타이밍 설정
		ldr	r0, =PXA_REG_MCMEM_S0
		
		ldr	r1, =MCMEM_S0_VALUE
		str	r1, [r0, #(PXA_REG_MCMEM_S0-PXA_REG_MCMEM_S0)]
		ldr	r1, [r0, #(PXA_REG_MCMEM_S0-PXA_REG_MCMEM_S0)]	// write and then read
		
		ldr	r1, =MCMEM_S1_VALUE
		str	r1, [r0, #(PXA_REG_MCMEM_S1-PXA_REG_MCMEM_S0)]
		ldr	r1, [r0, #(PXA_REG_MCMEM_S1-PXA_REG_MCMEM_S0)]	// write and then read

		ldr	r1, =MCATT_S0_VALUE
		str	r1, [r0, #(PXA_REG_MCATT_S0-PXA_REG_MCMEM_S0)]
		ldr	r1, [r0, #(PXA_REG_MCATT_S0-PXA_REG_MCMEM_S0)]	// write and then read

		ldr	r1, =MCATT_S1_VALUE
		str	r1, [r0, #(PXA_REG_MCATT_S1-PXA_REG_MCMEM_S0)]
		ldr	r1, [r0, #(PXA_REG_MCATT_S1-PXA_REG_MCMEM_S0)]	// write and then read

		ldr	r1, =MCIO_S0_VALUE
		str	r1, [r0, #(PXA_REG_MCIO_S0 -PXA_REG_MCMEM_S0)]
		ldr	r1, [r0, #(PXA_REG_MCIO_S0 -PXA_REG_MCMEM_S0)]	// write and then read

		ldr	r1, =MCIO_S1_VALUE
		str	r1, [r0, #(PXA_REG_MCIO_S1 -PXA_REG_MCMEM_S0)]
		ldr	r1, [r0, #(PXA_REG_MCIO_S1 -PXA_REG_MCMEM_S0)]	// write and then read

		// CKEN 설정
		ldr	r0, =PXA_REG_CKEN
		ldr	r1, =CKEN_VALUE
		str	r1, [r0]

	// Syncronous Static Memory  설정 ==========================
	
		ldr	r0, =PXA_REG_SXCNFG
		ldr	r1, =SXCNFG_VALUE
		str	r1, [r0]

	// SDRAM Clock 설정 ========================================
		
		ldr	r0, =PXA_REG_MDREFR
		ldr	r1, =MDREFR_VALUE
		
		// diable E1PIN
		bic	r1, r1, #MDREFR_E1PIN
		str	r1, [r0]
		
		// enable E1PIN
		orr	r1, r1, #MDREFR_E1PIN
		str	r1, [r0]
		nop
		nop

	// SDRAM Config 설정 =======================================

		ldr	r0, =PXA_REG_MDCNFG
		ldr	r1, =MDCNFG_VALUE
		
		// SDRAM Partition 0/1 disable
		bic	r1, r1, #(MDCNFG_DE0 | MDCNFG_DE1)
		str	r1, [r0]

		// CBR refresh cycles 8	
		ldr	r3, =EZ_X5_BASE_RAM
		.rept	8	
		str	r3, [r3]
		.endr
		
		// SDRAM Partition 0/1 enable
		ldr	r1, [r0]
		orr	r1, r1, #(MDCNFG_DE0|MDCNFG_DE1)
		str	r1, [r0]

	// SDRAM Burst length 설정 =================================
	
		ldr	r0, =PXA_REG_MDMRS
		ldr	r1, =MDMRS_VALUE
		str	r1, [r0]

		mov	pc, lr

			
//------------------------------------------------------------------------------------------------
//
//  부트 로더가 올려질 메모리 영역을 검사한다. 
//
//------------------------------------------------------------------------------------------------
.globl mem_test
mem_test:
		ldr	r2, =0x55555555
		ldr	r3, =0xAAAAAAAA

10:		ldr	r0, =EZ_X5_RAM_BOOT		// 메모리 베이스 어드레스
		ldr     r1, =EZ_X5_RAM_BOOT_END		// 128Kbyte 를 테스트 한다.
	
		// mem-write loop		
20:		str	r2, [r0], #4
		str	r3, [r0], #4
			
		cmp	r0, r1
		bne	20b
			
		// mem-comp loop		
		ldr	r0, =EZ_X5_RAM_BOOT		// 메모리 베이스 어드레스
			
30:		ldr	r4, [r0], #4
		ldr	r5, [r0], #4
			
		cmp	r2, r4
		bne	mem_error
		cmp	r3, r5
		bne	mem_error
		
		cmp	r0, r1
		bne	30b
		
		mov	r4, #0xAA
		and	r2, r2, #0xff
		cmp	r2, r4
		
		moveq	pc, lr		// return

		ldr	r2, =0xAAAAAAAA
		ldr	r3, =0x55555555
		b	10b
			
mem_error:
		// lr 을 무시하고 호출한다.
		mov	r5, #DEBUG_MEM_ERROR
		bl	led_out

		b	error_loop
			
//------------------------------------------------------------------------------------------------
//
//  부트 로더가 올려질 메모리 영역을 클리어 한다. 
//
//------------------------------------------------------------------------------------------------
.globl mem_clear
mem_clear:
		ldr     r0, =EZ_X5_RAM_BOOT		// 메모리 베이스 어드레스
		ldr     r1, =EZ_X5_RAM_BOOT_END		// 128Kbyte 
		mov     r2, #0x0 	    		// 메모리에 쓸 값
10:		
		str     r2, [r0], #4
		cmp     r0, r1
		bne     10b

		mov	pc, lr

//------------------------------------------------------------------------------------------------
//
//  롬영역의 부트 로더를 램에 올린다. 
//
//------------------------------------------------------------------------------------------------
.globl copy_loader_to_ram
copy_loader_to_ram:

		ldr     r0, =EZ_X5_RAM_BOOT		// 메모리 베이스 어드레스
		ldr     r1, =EZ_X5_RAM_BOOT_END		// 128kbyte 
		mov	r2, #EZ_X5_BASE_ROM	    	// 롬 베이스 어드레스

10:		ldr     r3, [r2], #4
		str	r3, [r0], #4
		cmp     r0, r1
		bne     10b

		// 비교한다.
		ldr     r0, =EZ_X5_BASE_ROM
		ldr     r1, =EZ_X5_RAM_BOOT
		ldr	r4, =EZ_X5_RAM_BOOT_END
		
20:		ldr     r2, [r0], #4
		ldr	r3, [r1], #4
		cmp     r2, r3
		bne     comp_error

		cmp	r1, r4
		bne	20b

		mov	pc, lr


comp_error:	mov	r5, #DEBUG_MEM_ERROR
		bl	led_out

		b	error_loop



