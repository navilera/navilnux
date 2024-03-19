/*------------------------------------------------------------------------------
 파 일 명 : tiny_serial.h
 프로젝트 : ezTiny for PXA255
 설    명 : ez-x5 의 작은 부트로더
 작 성 자 : 푸지
 작 성 일 : 2003년 7월 11일 
 수 정 일 : 
 주    의 : 
------------------------------------------------------------------------------*/

#ifndef _TINY_SERIAL_HEADER_
#define _TINY_SERIAL_HEADER_

#include	<ez_x5.h>


#define	BAUD_4800  	192
#define	BAUD_9600  	 96
#define	BAUD_19200 	 48
#define	BAUD_38400 	 24
#define	BAUD_57600 	 16
#define	BAUD_115200	  8
#define	BAUD_230400	  4
#define	BAUD_307200	  3
#define	BAUD_460800	  2
#define	BAUD_921600	  1

#define	SELECT_BAUD	BAUD_115200	

//----------------------------------------------
//	Clock Enable Register Value
//----------------------------------------------

#define	TINY_CKEN_STUART	(1 <<5 )	// STUART Clock Enable
#define	TINY_CKEN_FFUART	(1 <<6 )	// FFUART Clock Enable
#define	TINY_CKEN_BTUART	(1 <<7 )	// BTUART Clock Enable


//----------------------------------------------
//      GPIO46_RXD 
//      GPIO47_TXD 
//----------------------------------------------

#define GPIO_bit(x)		(1 << ((x) & 0x1f))
#define GPIO_ALT_FN_1		1
#define GPIO_ALT_FN_2		2
#define GPIO_ALT_FN_3		3
#define GPIO_ALT_FN_MASK	3


#define	STUART_GPDR1_MASK	( GPIO_bit(47)|GPIO_bit(46) )
#define	STUART_GPDR1_VALUE	( GPIO_bit(47) )	// TXD

#define	STUART_GAFR1_L_MASK	( (GPIO_ALT_FN_MASK<<30)|(GPIO_ALT_FN_MASK<<28) )
#define	STUART_GAFR1_L_VALUE	( (GPIO_ALT_FN_1   <<30)|(GPIO_ALT_FN_2   <<28) ) // TXD | RXD

//----------------------------------------------
//      STUART register
//----------------------------------------------
#define STUART_BASE	0x40700000
#define RBR		0x00  // Receive Buffer Register (read only)  
#define THR		0x00  // Transmit Holding Register (write only)  
#define IER		0x04  // Interrupt Enable Register (read/write)  
#define IIR		0x08  // Interrupt ID Register (read only)  
#define FCR		0x08  // FIFO Control Register (write only)  
#define LCR		0x0C  // Line Control Register (read/write)  
#define MCR		0x10  // Modem Control Register (read/write)  
#define LSR		0x14  // Line Status Register (read only)  
#define MSR		0x18  // Reserved  
#define SPR		0x1C  // Scratch Pad Register (read/write)  
#define ISR		0x20  // Infrared Selection Register (read/write)  
#define DLL		0x00  // Divisor Latch Low Register (DLAB = 1) (read/write)  
#define DLH		0x04  // Divisor Latch High Register (DLAB = 1) (read/write)  

#define IER_DMAE	(1 << 7)	// DMA Requests Enable  
#define IER_UUE		(1 << 6)	// UART Unit Enable  
#define IER_NRZE	(1 << 5)	// NRZ coding Enable  
#define IER_RTIOE	(1 << 4)	// Receiver Time Out Interrupt Enable  
#define IER_MIE		(1 << 3)	// Modem Interrupt Enable  
#define IER_RLSE	(1 << 2)	// Receiver Line Status Interrupt Enable  
#define IER_TIE		(1 << 1)	// Transmit Data request Interrupt Enable  
#define IER_RAVIE	(1 << 0)	// Receiver Data Available Interrupt Enable  

#define IIR_FIFOES1	(1 << 7)	// FIFO Mode Enable Status  
#define IIR_FIFOES0	(1 << 6)	// FIFO Mode Enable Status  
#define IIR_TOD		(1 << 3)	// Time Out Detected  
#define IIR_IID2	(1 << 2)	// Interrupt Source Encoded  
#define IIR_IID1	(1 << 1)	// Interrupt Source Encoded  
#define IIR_IP		(1 << 0)	// Interrupt Pending (active low)  

#define FCR_ITL2	(1 << 7)	// Interrupt Trigger Level  
#define FCR_ITL1	(1 << 6)	// Interrupt Trigger Level  
#define FCR_RESETTF	(1 << 2)	// Reset Transmitter FIFO  
#define FCR_RESETRF	(1 << 1)	// Reset Receiver FIFO  
#define FCR_TRFIFOE	(1 << 0)	// Transmit and Receive FIFO Enable  
#define FCR_ITL_1	(0)
#define FCR_ITL_8	(FCR_ITL1)
#define FCR_ITL_16	(FCR_ITL2)
#define FCR_ITL_32	(FCR_ITL2|FCR_ITL1)

#define LCR_DLAB	(1 << 7)	// Divisor Latch Access Bit  
#define LCR_SB		(1 << 6)	// Set Break  
#define LCR_STKYP	(1 << 5)	// Sticky Parity  
#define LCR_EPS		(1 << 4)	// Even Parity Select  
#define LCR_PEN		(1 << 3)	// Parity Enable  
#define LCR_STB		(1 << 2)	// Stop Bit  
#define LCR_WLS1	(1 << 1)	// Word Length Select  
#define LCR_WLS0	(1 << 0)	// Word Length Select  

#define LSR_FIFOE	(1 << 7)	// FIFO Error Status  
#define LSR_TEMT	(1 << 6)	// Transmitter Empty  
#define LSR_TDRQ	(1 << 5)	// Transmit Data Request  
#define LSR_BI		(1 << 4)	// Break Interrupt  
#define LSR_FE		(1 << 3)	// Framing Error  
#define LSR_PE		(1 << 2)	// Parity Error  
#define LSR_OE		(1 << 1)	// Overrun Error  
#define LSR_DR		(1 << 0)	// Data Ready  

#define MCR_LOOP	(1 << 4)	//                                  
#define MCR_OUT2	(1 << 3)	// force MSR_DCD in loopback mode  
#define MCR_OUT1	(1 << 2)	// force MSR_RI in loopback mode  
#define MCR_RTS		(1 << 1)	// Request to Send  
#define MCR_DTR		(1 << 0)	// Data Terminal Ready  

#define MSR_DCD		(1 << 7)	// Data Carrier Detect  
#define MSR_RI		(1 << 6)	// Ring Indicator  
#define MSR_DSR		(1 << 5)	// Data Set Ready  
#define MSR_CTS		(1 << 4)	// Clear To Send  
#define MSR_DDCD	(1 << 3)	// Delta Data Carrier Detect  
#define MSR_TERI	(1 << 2)	// Trailing Edge Ring Indicator  
#define MSR_DDSR	(1 << 1)	// Delta Data Set Ready  
#define MSR_DCTS	(1 << 0)	// Delta Clear To Send  


#endif //_TINY_SERIAL_HEADER_

