/*=======================================================================================

  화일명 : tiny_serial.S
  설  명 : 시리얼을 열고 데이타를 받는다.
  
  작성자 : (주)제이닷디앤티  freefrug@falinux.com  -- 푸지
  작성일 : 2003년 7월 11일
  
  비  고 :  
=========================================================================================*/

#include 	<ez_x5.h>
#include 	<tiny_serial.h>

.text

//-----------------------------------------------------------------------------------------		
//
//  시리얼 관련 레지스터를 초기화 한다.
//
//-----------------------------------------------------------------------------------------		
.globl		serial_init
serial_init:
		// STUART 활성화 ================================
              	ldr	r0, =PXA_REG_CKEN
		ldr	r1, [r0]
		orr	r1, r1, #(TINY_CKEN_STUART)
		str	r1, [r0]
		
		// 방향결정 GPIO46_RXD, GPIO47_TXD  
		ldr	r0, =PXA_REG_GP_BASE
		ldr	r1, [r0, #PXA_REG_OFFSET_GPDR1]
		
		bic	r1, r1, #STUART_GPDR1_MASK
		orr	r1, r1, #STUART_GPDR1_VALUE
		str	r1, [r0, #PXA_REG_OFFSET_GPDR1]

		// 함수결정 GPIO46_RXD, GPIO47_TXD  
		ldr	r1, [r0, #PXA_REG_OFFSET_GAFR1_L]
		
		bic	r1, r1, #STUART_GAFR1_L_MASK
		orr	r1, r1, #STUART_GAFR1_L_VALUE
		str	r1, [r0, #PXA_REG_OFFSET_GAFR1_L]

		
		// STUART 레지스터 설정 =========================
		ldr	r0, =STUART_BASE
		
		// 인터럽트를 금지 시킨다. 
		mov	r1, #0
		str	r1, [r0, #IER]

		// FIFO 를 리셋한후 비활성화 한다.
		mov	r1, #(FCR_RESETTF | FCR_RESETRF)
		str	r1, [r0, #FCR]

		// baud, 8BIT 1STOP NO PARITY 로 설정한다.
		mov	r1, #(LCR_WLS1|LCR_WLS0|LCR_DLAB)
		str	r1, [r0, #LCR]

		mov	r1, #SELECT_BAUD
		str	r1, [r0, #DLL]
		mov	r1, #0
		str	r1, [r0, #DLH]

		mov	r1, #(LCR_WLS1|LCR_WLS0)
		str	r1, [r0, #LCR]

    		// UART를 동작 시킨다. 
		mov	r1, #IER_UUE
		str	r1, [r0, #IER]


		mov	pc, lr

//-----------------------------------------------------------------------------------------		
//	send_ack, nak
//-----------------------------------------------------------------------------------------		
send_nak:
		and	r1, r1, #0x7f
send_ack:
		// 전송비트가 있을때까지 대기한다.
10:		ldr	r2, [r0, #LSR]
		ands	r2, r2, #LSR_TDRQ

		beq	10b

		// 데이타를 전송한다.
		str	r1, [r0, #THR]

		b	recv_loop
		
//-----------------------------------------------------------------------------------------		
//	send_char
//-----------------------------------------------------------------------------------------		
send_char:
		// 전송비트가 있을때까지 대기한다.
10:		ldr	r2, [r0, #LSR]
		ands	r2, r2, #LSR_TDRQ

		beq	10b

		// 데이타를 전송한다.
		str	r1, [r0, #THR]
	
		mov	pc, lr

//-----------------------------------------------------------------------------------------		
//
//  	데이타를 시리얼로 받는다.
//
//      호스트가 한바이트를 전송하면 이를 판단하여 처리한후 받은데이타를 
//      ACK 로 전송한다.
//
//      Cmd(4)  Data(4)	: 8-bit
//      1000	xxxx	: 버퍼를 초기화 한다.
//	1001	dddd	: 미사용
//	1010	dddd	: 현재의 주소니블에 데이타를 저장하고 주소니블을 증가한다.
//	1011	0101	: c_main 주소로 점프준비
//	1100	1010	: c_main 주소로 점프
//	0ccc    dddd    : nak 
//
//	r1  :  recv data
//	r6  :  nible count
//	r7  :  SDRAM Address
//	r8  :  long data
//	r10 :  lr save      
//-----------------------------------------------------------------------------------------		
.globl		tiny_download
tiny_download:
		mov	r10, lr
		
		// UART 레지스터 베이스 주소
		ldr	r0, =STUART_BASE
		
		// nible count
		mov	r6, #0
		// SDRAM Address
		ldr	r7, =EZ_X5_RAM_BOOT
		// make data long = 0
		mov	r8, #0

		// send version string
		ldr	r4, =version_table
10:		ldrb	r1, [r4]
		cmp	r1, #0
		beq	recv_loop
		bl	send_char
		add	r4, r4, #1
		b	10b
	
		// 수신된 데이타가 있는가를 확인한다. 
recv_loop:	ldr	r3, [r0, #LSR]	// 라인상태를 얻는다.
		ands	r2, r3, #LSR_DR
		
		beq	recv_loop	// 수신되 데이타가 없다.

		// 데이타를 얻는다.
		ldr	r1, [r0, #RBR]	
		
		// 에러가 있는지 확인한다.
		ands	r2, r3, #(LSR_PE | LSR_FE | LSR_OE)
		
		bne	recv_loop	// 에러가 있다.

		// 수신된 데이타를 분석하여 처리한다.
		ands	r2, r1, #0x80
		beq	send_nak	// 명령어 비트 에러

		// 명령어 분기
		mov	r2, r1, lsr #2
		and	r2, r2, #0x1c	
		
		ldr	r3, =cmd_table
		ldr	pc, [r3, r2]				

//-----------------------------------------------------------------------------------------		
//	명령어 테이블
//-----------------------------------------------------------------------------------------		
		.align	4
cmd_table:
		.long	reset_buff
		.long	not_define
		.long	recv_buff_inc
		.long	check_jump
		.long	jump_c_main
		.long	not_define
		.long	not_define
		.long	not_define

version_table:	.byte	'e','z','t','i','n','y',' ','V','0','2'
		.byte	0

		.align	4
//-----------------------------------------------------------------------------------------		
reset_buff:
		// nible count = 0,  SDRAM Address = EZ_X5_RAM_BOOT, make data long = 0
		mov	r6, #0
		ldr	r7, =EZ_X5_RAM_BOOT
		mov	r8, #0

		b	send_ack
		
//-----------------------------------------------------------------------------------------		
recv_buff_inc:
		// 예전데이타와 함께 long 데이타로 만든다.
		and	r2, r1, #0x0f
		orr	r8, r2, r8, lsl #4

		// 8 개의 니블을 받았으면 SDRAM 에 저장한다.
		add	r6, r6, #1
		ands	r2, r6, #0x7
		
		// 8 개의 니블을 받은후 에크를 전송한다.
		bne	recv_loop	
		
		// SDRAM에 저장한다.
		str	r8, [r7]
		
		mov	r8, #0
		add	r7, r7, #4

		b	send_ack

//-----------------------------------------------------------------------------------------		
check_jump:
		//  데이타가 0x5 이면 정상이다.
		and	r2, r1, #0xf
		cmp	r2, #0x05	
		
		bne	send_nak
		
		// 나머지 데이타를 SDRAM에 저장한다.
		str	r8, [r7]
		
		b	send_ack

//-----------------------------------------------------------------------------------------		
jump_c_main:	
		//  데이타가 0xa 이면 정상이다.
		and	r2, r1, #0xf
		cmp	r2, #0x0a	
		
		bne	send_nak

		// 이곳을 빠져나간다.
		mov	pc, r10

//-----------------------------------------------------------------------------------------		
not_define:

		b	send_nak
