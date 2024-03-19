//------------------------------------------------------------------------------
// 파 일 명 : pxa255.h
// 프로젝트 : ezboot
// 설    명 : ezboot에서 사용하는 pxa255에 관련된 내용
// 작 성 자 : 유영창
// 작 성 일 : 2003년 4월 15일
// 수 정 일 : 
// 주    의 : 이 헤더 화일의 대부분의 내용은 
//            ARM Linux의 헤더 화일 linux/include/asm-arm\arch-pxa250
//            에서 가져 왔다. 
//------------------------------------------------------------------------------

#ifndef _PXA255_HEADER_
#define _PXA255_HEADER_

//-----------------------------------------------------------------------------
//
// 기본 형에 대한 정의  
//
//-----------------------------------------------------------------------------
typedef unsigned char   Byte;
typedef unsigned short  Word16 ;
typedef unsigned int    Word32 ;
typedef Word32          Word ;
typedef Word            Quad [4] ;
typedef void            *Address ;
typedef void            (*ExcpHndlr) (void) ;

//-----------------------------------------------------------------------------
//
// 기본 데이타 처리 매크로 
//
//-----------------------------------------------------------------------------
#define UData(Data)			((unsigned int) (Data))

#define Fld(Size, Shft)		(((Size) << 16) + (Shft))

#define FSize(Field)		((Field) >> 16)
#define FShft(Field)		((Field) & 0x0000FFFF)
#define FMsk(Field)			(((UData (1) << FSize (Field)) - 1) << FShft (Field))
#define FAlnMsk(Field)		((UData (1) << FSize (Field)) - 1)
#define F1stBit(Field)		(UData (1) << FShft (Field))

#define io_p2v(PhAdd)		(PhAdd)

#define __REG(x)			(*((volatile Word *) io_p2v (x))) 
#define __REG2(x,y)			(*(volatile Word *)((Word)&__REG(x) + (y)))

//-----------------------------------------------------------------------------
//
// PXA250 
//
//-----------------------------------------------------------------------------

#define PXA_CS0_PHYS	0x00000000
#define PXA_CS1_PHYS	0x04000000
#define PXA_CS2_PHYS	0x08000000
#define PXA_CS3_PHYS	0x0C000000
#define PXA_CS4_PHYS	0x10000000
#define PXA_CS5_PHYS	0x14000000

//----------------------------------------------
//	
//  Power Manager Sleep Status Register
//
//----------------------------------------------
#define PSSR            __REG(0x40F00004)  	// Power Manager Sleep Status Register
#define PSSR_RDH        (1 << 5)        	// Read Disable Hold 
#define PSSR_PH         (1 << 4)        	// Peripheral Control Hold
#define PSSR_VFS        (1 << 2)        	// VDD Fault Status
#define PSSR_BFS        (1 << 1)        	// Battery Fault Status
#define PSSR_SSS        (1 << 0)        	// Software Sleep Status

//----------------------------------------------
//	
//  Clock Enable Register 
//
//----------------------------------------------

#define CKEN		__REG(0x41300004)  

#define	CKEN_PWM0		(0 <<0 )	// PWM0   Clock Enable
#define	CKEN_PWM1		(0 <<1 )	// PWM1   Clock Enable
#define	CKEN_AC97		(0 <<2 )	// AC97   Clock Enable
#define	CKEN_SSP		(1 <<3 )	// SSP    Clock Enable
#define	CKEN_HWUART		(0 <<4 )	// HWUART Clock Enable
#define	CKEN_STUART		(1 <<5 )	// STUART Clock Enable
#define	CKEN_FFUART		(1 <<6 )	// FFUART Clock Enable
#define	CKEN_BTUART		(1 <<7 )	// BTUART Clock Enable
#define	CKEN_I2S		(1 <<8 )	// I2S	  Clock Enable
#define	CKEN_NSSP		(0 <<9 )	// NSSP	  Clock Enable
#define	CKEN_USB		(0 <<11)	// USB	  Clock Enable
#define	CKEN_MMC		(0 <<12)	// MMC	  Clock Enable
#define	CKEN_FICP		(0 <<13)	// FICP	  Clock Enable
#define	CKEN_I2C		(1 <<14)	// I2C	  Clock Enable
#define	CKEN_LCD		(1 <<16)	// LCD	  Clock Enable



//-----------------------------------------------------------------------------
//
// UARTs
//
//-----------------------------------------------------------------------------

// Full Function UART (FFUART)  
#define FFUART		FFRBR
#define FFRBR		__REG(0x40100000)  // Receive Buffer Register (read only)  
#define FFTHR		__REG(0x40100000)  // Transmit Holding Register (write only)  
#define FFIER		__REG(0x40100004)  // Interrupt Enable Register (read/write)  
#define FFIIR		__REG(0x40100008)  // Interrupt ID Register (read only)  
#define FFFCR		__REG(0x40100008)  // FIFO Control Register (write only)  
#define FFLCR		__REG(0x4010000C)  // Line Control Register (read/write)  
#define FFMCR		__REG(0x40100010)  // Modem Control Register (read/write)  
#define FFLSR		__REG(0x40100014)  // Line Status Register (read only)  
#define FFMSR		__REG(0x40100018)  // Modem Status Register (read only)  
#define FFSPR		__REG(0x4010001C)  // Scratch Pad Register (read/write)  
#define FFISR		__REG(0x40100020)  // Infrared Selection Register (read/write)  
#define FFDLL		__REG(0x40100000)  // Divisor Latch Low Register (DLAB = 1) (read/write)  
#define FFDLH		__REG(0x40100004)  // Divisor Latch High Register (DLAB = 1) (read/write)  

// Bluetooth UART (BTUART)  
#define BTUART		BTRBR
#define BTRBR		__REG(0x40200000)  // Receive Buffer Register (read only)  
#define BTTHR		__REG(0x40200000)  // Transmit Holding Register (write only)  
#define BTIER		__REG(0x40200004)  // Interrupt Enable Register (read/write)  
#define BTIIR		__REG(0x40200008)  // Interrupt ID Register (read only)  
#define BTFCR		__REG(0x40200008)  // FIFO Control Register (write only)  
#define BTLCR		__REG(0x4020000C)  // Line Control Register (read/write)  
#define BTMCR		__REG(0x40200010)  // Modem Control Register (read/write)  
#define BTLSR		__REG(0x40200014)  // Line Status Register (read only)  
#define BTMSR		__REG(0x40200018)  // Modem Status Register (read only)  
#define BTSPR		__REG(0x4020001C)  // Scratch Pad Register (read/write)  
#define BTISR		__REG(0x40200020)  // Infrared Selection Register (read/write)  
#define BTDLL		__REG(0x40200000)  // Divisor Latch Low Register (DLAB = 1) (read/write)  
#define BTDLH		__REG(0x40200004)  // Divisor Latch High Register (DLAB = 1) (read/write)  

// Standard UART (STUART)  
#define STUART		STRBR
#define STRBR		__REG(0x40700000)  // Receive Buffer Register (read only)  
#define STTHR		__REG(0x40700000)  // Transmit Holding Register (write only)  
#define STIER		__REG(0x40700004)  // Interrupt Enable Register (read/write)  
#define STIIR		__REG(0x40700008)  // Interrupt ID Register (read only)  
#define STFCR		__REG(0x40700008)  // FIFO Control Register (write only)  
#define STLCR		__REG(0x4070000C)  // Line Control Register (read/write)  
#define STMCR		__REG(0x40700010)  // Modem Control Register (read/write)  
#define STLSR		__REG(0x40700014)  // Line Status Register (read only)  
#define STMSR		__REG(0x40700018)  // Reserved  
#define STSPR		__REG(0x4070001C)  // Scratch Pad Register (read/write)  
#define STISR		__REG(0x40700020)  // Infrared Selection Register (read/write)  
#define STDLL		__REG(0x40700000)  // Divisor Latch Low Register (DLAB = 1) (read/write)  
#define STDLH		__REG(0x40700004)  // Divisor Latch High Register (DLAB = 1) (read/write)  

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

#define IrSR_OFFSET 0x20

#define IrSR_RXPL_NEG_IS_ZERO (1<<4)
#define IrSR_RXPL_POS_IS_ZERO 0x0
#define IrSR_TXPL_NEG_IS_ZERO (1<<3)
#define IrSR_TXPL_POS_IS_ZERO 0x0
#define IrSR_XMODE_PULSE_1_6  (1<<2)
#define IrSR_XMODE_PULSE_3_16 0x0
#define IrSR_RCVEIR_IR_MODE   (1<<1)
#define IrSR_RCVEIR_UART_MODE 0x0
#define IrSR_XMITIR_IR_MODE   (1<<0)
#define IrSR_XMITIR_UART_MODE 0x0

#define IrSR_IR_RECEIVE_ON (\
                IrSR_RXPL_NEG_IS_ZERO | \
                IrSR_TXPL_POS_IS_ZERO | \
                IrSR_XMODE_PULSE_3_16 | \
                IrSR_RCVEIR_IR_MODE   | \
                IrSR_XMITIR_UART_MODE)

#define IrSR_IR_TRANSMIT_ON (\
                IrSR_RXPL_NEG_IS_ZERO | \
                IrSR_TXPL_POS_IS_ZERO | \
                IrSR_XMODE_PULSE_3_16 | \
                IrSR_RCVEIR_UART_MODE | \
                IrSR_XMITIR_IR_MODE)

//
// Fast Infrared Communication Port
//

#define ICCR0		__REG(0x40800000)  // ICP Control Register 0 
#define ICCR1		__REG(0x40800004)  // ICP Control Register 1 
#define ICCR2		__REG(0x40800008)  // ICP Control Register 2 
#define ICDR		__REG(0x4080000c)  // ICP Data Register 
#define ICSR0		__REG(0x40800014)  // ICP Status Register 0 
#define ICSR1		__REG(0x40800018)  // ICP Status Register 1 

//-----------------------------------------------------------------------------
//
// 타이머 
//
//-----------------------------------------------------------------------------


#define OSMR0		__REG(0x40A00000)  //  
#define OSMR1		__REG(0x40A00004)  //  
#define OSMR2		__REG(0x40A00008)  //  
#define OSMR3		__REG(0x40A0000C)  //  
#define OSCR		__REG(0x40A00010)  // OS Timer Counter Register  
#define OSSR		__REG(0x40A00014)  // OS Timer Status Register  
#define OWER		__REG(0x40A00018)  // OS Timer Watchdog Enable Register  
#define OIER		__REG(0x40A0001C)  // OS Timer Interrupt Enable Register  

#define OSSR_M3		(1 << 3)	// Match status channel 3  
#define OSSR_M2		(1 << 2)	// Match status channel 2  
#define OSSR_M1		(1 << 1)	// Match status channel 1  
#define OSSR_M0		(1 << 0)	// Match status channel 0  

#define OWER_WME	(1 << 0)	// Watchdog Match Enable  

#define OIER_E3		(1 << 3)	// Interrupt enable channel 3  
#define OIER_E2		(1 << 2)	// Interrupt enable channel 2  
#define OIER_E1		(1 << 1)	// Interrupt enable channel 1  
#define OIER_E0		(1 << 0)	// Interrupt enable channel 0  



//-----------------------------------------------------------------------------
//
// GPIO
//
//-----------------------------------------------------------------------------
#define GPLR0		__REG(0x40E00000)  // GPIO Pin-Level Register GPIO<31:0>  
#define GPLR1		__REG(0x40E00004)  // GPIO Pin-Level Register GPIO<63:32>  
#define GPLR2		__REG(0x40E00008)  // GPIO Pin-Level Register GPIO<80:64>  

#define GPDR0		__REG(0x40E0000C)  // GPIO Pin Direction Register GPIO<31:0>  
#define GPDR1		__REG(0x40E00010)  // GPIO Pin Direction Register GPIO<63:32>  
#define GPDR2		__REG(0x40E00014)  // GPIO Pin Direction Register GPIO<80:64>  

#define GPSR0		__REG(0x40E00018)  // GPIO Pin Output Set Register GPIO<31:0>  
#define GPSR1		__REG(0x40E0001C)  // GPIO Pin Output Set Register GPIO<63:32>  
#define GPSR2		__REG(0x40E00020)  // GPIO Pin Output Set Register GPIO<80:64>  

#define GPCR0		__REG(0x40E00024)  // GPIO Pin Output Clear Register GPIO<31:0>  
#define GPCR1		__REG(0x40E00028)  // GPIO Pin Output Clear Register GPIO <63:32>  
#define GPCR2		__REG(0x40E0002C)  // GPIO Pin Output Clear Register GPIO <80:64>  

#define GRER0		__REG(0x40E00030)  // GPIO Rising-Edge Detect Register GPIO<31:0>  
#define GRER1		__REG(0x40E00034)  // GPIO Rising-Edge Detect Register GPIO<63:32>  
#define GRER2		__REG(0x40E00038)  // GPIO Rising-Edge Detect Register GPIO<80:64>  

#define GFER0		__REG(0x40E0003C)  // GPIO Falling-Edge Detect Register GPIO<31:0>  
#define GFER1		__REG(0x40E00040)  // GPIO Falling-Edge Detect Register GPIO<63:32>  
#define GFER2		__REG(0x40E00044)  // GPIO Falling-Edge Detect Register GPIO<80:64>  

#define GEDR0		__REG(0x40E00048)  // GPIO Edge Detect Status Register GPIO<31:0>  
#define GEDR1		__REG(0x40E0004C)  // GPIO Edge Detect Status Register GPIO<63:32>  
#define GEDR2		__REG(0x40E00050)  // GPIO Edge Detect Status Register GPIO<80:64>  

#define GAFR0_L		__REG(0x40E00054)  // GPIO Alternate Function Select Register GPIO<15:0>  
#define GAFR0_U		__REG(0x40E00058)  // GPIO Alternate Function Select Register GPIO<31:16>  
#define GAFR1_L		__REG(0x40E0005C)  // GPIO Alternate Function Select Register GPIO<47:32>  
#define GAFR1_U		__REG(0x40E00060)  // GPIO Alternate Function Select Register GPIO<63:48>  
#define GAFR2_L		__REG(0x40E00064)  // GPIO Alternate Function Select Register GPIO<79:64>  
#define GAFR2_U		__REG(0x40E00068)  // GPIO Alternate Function Select Register GPIO 80  

// More handy macros.  The argument is a literal GPIO number.  

#define GPIO_bit(x)	(1 << ((x) & 0x1f))
#define GPLR(x)		__REG2(0x40E00000, ((x) & 0x60) >> 3)
#define GPDR(x)		__REG2(0x40E0000C, ((x) & 0x60) >> 3)
#define GPSR(x)		__REG2(0x40E00018, ((x) & 0x60) >> 3)
#define GPCR(x)		__REG2(0x40E00024, ((x) & 0x60) >> 3)
#define GRER(x)		__REG2(0x40E00030, ((x) & 0x60) >> 3)
#define GFER(x)		__REG2(0x40E0003C, ((x) & 0x60) >> 3)
#define GEDR(x)		__REG2(0x40E00048, ((x) & 0x60) >> 3)
#define GAFR(x)		__REG2(0x40E00054, ((x) & 0x70) >> 2)

// GPIO alternate function assignments  

#define GPIO1_RST		1	// reset  
#define GPIO6_MMCCLK		6	// MMC Clock  
#define GPIO8_48MHz		7	// 48 MHz clock output  
#define GPIO8_MMCCS0		8	// MMC Chip Select 0  
#define GPIO9_MMCCS1		9	// MMC Chip Select 1  
#define GPIO10_RTCCLK		10	// real time clock (1 Hz)  
#define GPIO11_3_6MHz		11	// 3.6 MHz oscillator out  
#define GPIO12_32KHz		12	// 32 kHz out  
#define GPIO13_MBGNT		13	// memory controller grant  
#define GPIO14_MBREQ		14	// alternate bus master request  
#define GPIO15_nCS_1		15	// chip select 1  
#define GPIO16_PWM0		16	// PWM0 output  
#define GPIO17_PWM1		17	// PWM1 output  
#define GPIO18_RDY		18	// Ext. Bus Ready  
#define GPIO19_DREQ1		19	// External DMA Request  
#define GPIO20_DREQ0		20	// External DMA Request  
#define GPIO23_SCLK		23	// SSP clock  
#define GPIO24_SFRM		24	// SSP Frame  
#define GPIO25_STXD		25	// SSP transmit  
#define GPIO26_SRXD		26	// SSP receive  
#define GPIO27_SEXTCLK		27	// SSP ext_clk  
#define GPIO28_BITCLK		28	// AC97/I2S bit_clk  
#define GPIO29_SDATA_IN		29	// AC97 Sdata_in0 / I2S Sdata_in  
#define GPIO30_SDATA_OUT	30	// AC97/I2S Sdata_out  
#define GPIO31_SYNC		31	// AC97/I2S sync  
#define GPIO32_SDATA_IN1	32	// AC97 Sdata_in1  
#define GPIO33_nCS_5		33	// chip select 5  
#define GPIO34_FFRXD		34	// FFUART receive  
#define GPIO34_MMCCS0		34	// MMC Chip Select 0  
#define GPIO35_FFCTS		35	// FFUART Clear to send  
#define GPIO36_FFDCD		36	// FFUART Data carrier detect  
#define GPIO37_FFDSR		37	// FFUART data set ready  
#define GPIO38_FFRI		38	// FFUART Ring Indicator  
#define GPIO39_MMCCS1		39	// MMC Chip Select 1  
#define GPIO39_FFTXD		39	// FFUART transmit data  
#define GPIO40_FFDTR		40	// FFUART data terminal Ready  
#define GPIO41_FFRTS		41	// FFUART request to send  
#define GPIO42_BTRXD		42	// BTUART receive data  
#define GPIO43_BTTXD		43	// BTUART transmit data  
#define GPIO44_BTCTS		44	// BTUART clear to send  
#define GPIO45_BTRTS		45	// BTUART request to send  
#define GPIO46_ICPRXD		46	// ICP receive data  
#define GPIO46_STRXD		46	// STD_UART receive data  
#define GPIO47_ICPTXD		47	// ICP transmit data  
#define GPIO47_STTXD		47	// STD_UART transmit data  
#define GPIO48_nPOE		48	// Output Enable for Card Space  
#define GPIO49_nPWE		49	// Write Enable for Card Space  
#define GPIO50_nPIOR		50	// I/O Read for Card Space  
#define GPIO51_nPIOW		51	// I/O Write for Card Space  
#define GPIO52_nPCE_1		52	// Card Enable for Card Space  
#define GPIO53_nPCE_2		53	// Card Enable for Card Space  
#define GPIO53_MMCCLK		53	// MMC Clock  
#define GPIO54_MMCCLK		54	// MMC Clock  
#define GPIO54_pSKTSEL		54	// Socket Select for Card Space  
#define GPIO55_nPREG		55	// Card Address bit 26  
#define GPIO56_nPWAIT		56	// Wait signal for Card Space  
#define GPIO57_nIOIS16		57	// Bus Width select for I/O Card Space  
#define GPIO58_LDD_0		58	// LCD data pin 0  
#define GPIO59_LDD_1		59	// LCD data pin 1  
#define GPIO60_LDD_2		60	// LCD data pin 2  
#define GPIO61_LDD_3		61	// LCD data pin 3  
#define GPIO62_LDD_4		62	// LCD data pin 4  
#define GPIO63_LDD_5		63	// LCD data pin 5  
#define GPIO64_LDD_6		64	// LCD data pin 6  
#define GPIO65_LDD_7		65	// LCD data pin 7  
#define GPIO66_LDD_8		66	// LCD data pin 8  
#define GPIO66_MBREQ		66	// alternate bus master req  
#define GPIO67_LDD_9		67	// LCD data pin 9  
#define GPIO67_MMCCS0		67	// MMC Chip Select 0  
#define GPIO68_LDD_10		68	// LCD data pin 10  
#define GPIO68_MMCCS1		68	// MMC Chip Select 1  
#define GPIO69_LDD_11		69	// LCD data pin 11  
#define GPIO69_MMCCLK		69	// MMC_CLK  
#define GPIO70_LDD_12		70	// LCD data pin 12  
#define GPIO70_RTCCLK		70	// Real Time clock (1 Hz)  
#define GPIO71_LDD_13		71	// LCD data pin 13  
#define GPIO71_3_6MHz		71	// 3.6 MHz Oscillator clock  
#define GPIO72_LDD_14		72	// LCD data pin 14  
#define GPIO72_32kHz		72	// 32 kHz clock  
#define GPIO73_LDD_15		73	// LCD data pin 15  
#define GPIO73_MBGNT		73	// Memory controller grant  
#define GPIO74_LCD_FCLK		74	// LCD Frame clock  
#define GPIO75_LCD_LCLK		75	// LCD line clock  
#define GPIO76_LCD_PCLK		76	// LCD Pixel clock  
#define GPIO77_LCD_ACBIAS	77	// LCD AC Bias  
#define GPIO78_nCS_2		78	// chip select 2  
#define GPIO79_nCS_3		79	// chip select 3  
#define GPIO80_nCS_4		80	// chip select 4  

#define GPIO_IN				0x000
#define GPIO_OUT			0x080
#define GPIO_ALT_FN_1_IN		0x100
#define GPIO_ALT_FN_1_OUT		0x180
#define GPIO_ALT_FN_2_IN		0x200
#define GPIO_ALT_FN_2_OUT		0x280
#define GPIO_ALT_FN_3_IN		0x300
#define GPIO_ALT_FN_3_OUT		0x380
#define GPIO_MD_MASK_NR			0x07f
#define GPIO_MD_MASK_DIR		0x080
#define GPIO_MD_MASK_FN			0x300


#define GPIO01_RTS_MD			( 1 | GPIO_ALT_FN_1_IN)
#define GPIO06_MMCCLK_MD		( 6 | GPIO_ALT_FN_1_OUT)
#define GPIO08_48MHz_MD			( 8 | GPIO_ALT_FN_1_OUT)
#define GPIO08_MMCCS0_MD		( 8 | GPIO_ALT_FN_1_OUT)
#define GPIO09_MMCCS1_MD		( 9 | GPIO_ALT_FN_1_OUT)
#define GPIO10_RTCCLK_MD		(10 | GPIO_ALT_FN_1_OUT)
#define GPIO11_3_6MHz_MD		(11 | GPIO_ALT_FN_1_OUT)
#define GPIO12_32KHz_MD			(12 | GPIO_ALT_FN_1_OUT)
#define GPIO13_MBGNT_MD			(13 | GPIO_ALT_FN_2_OUT)
#define GPIO14_MBREQ_MD			(14 | GPIO_ALT_FN_1_IN)
#define GPIO15_nCS_1_MD			(15 | GPIO_ALT_FN_2_OUT)
#define GPIO16_PWM0_MD			(16 | GPIO_ALT_FN_2_OUT)
#define GPIO17_PWM1_MD			(17 | GPIO_ALT_FN_2_OUT)
#define GPIO18_RDY_MD			(18 | GPIO_ALT_FN_1_IN)
#define GPIO19_DREQ1_MD			(19 | GPIO_ALT_FN_1_IN)
#define GPIO20_DREQ0_MD			(20 | GPIO_ALT_FN_1_IN)
#define GPIO23_SCLK_md			(23 | GPIO_ALT_FN_2_OUT)
#define GPIO24_SFRM_MD			(24 | GPIO_ALT_FN_2_OUT)
#define GPIO25_STXD_MD			(25 | GPIO_ALT_FN_2_OUT)
#define GPIO26_SRXD_MD			(26 | GPIO_ALT_FN_1_IN)
#define GPIO27_SEXTCLK_MD		(27 | GPIO_ALT_FN_1_IN)
#define GPIO28_BITCLK_AC97_MD		(28 | GPIO_ALT_FN_1_IN)
#define GPIO28_BITCLK_I2S_MD		(28 | GPIO_ALT_FN_2_IN)
#define GPIO29_SDATA_IN_AC97_MD		(29 | GPIO_ALT_FN_1_IN)
#define GPIO29_SDATA_IN_I2S_MD		(29 | GPIO_ALT_FN_2_IN)
#define GPIO30_SDATA_OUT_AC97_MD	(30 | GPIO_ALT_FN_2_OUT)
#define GPIO30_SDATA_OUT_I2S_MD		(30 | GPIO_ALT_FN_1_OUT)
#define GPIO31_SYNC_AC97_MD		(31 | GPIO_ALT_FN_2_OUT)
#define GPIO31_SYNC_I2S_MD		(31 | GPIO_ALT_FN_1_OUT)
#define GPIO32_SDATA_IN1_AC97_MD	(32 | GPIO_ALT_FN_1_IN)
#define GPIO33_nCS_5_MD			(33 | GPIO_ALT_FN_2_OUT)	
#define GPIO34_FFRXD_MD			(34 | GPIO_ALT_FN_1_IN) 	
#define GPIO34_MMCCS0_MD		(34 | GPIO_ALT_FN_2_OUT)	
#define GPIO35_FFCTS_MD			(35 | GPIO_ALT_FN_1_IN) 	
#define GPIO36_FFDCD_MD			(36 | GPIO_ALT_FN_1_IN) 	
#define GPIO37_FFDSR_MD			(37 | GPIO_ALT_FN_1_IN) 	
#define GPIO38_FFRI_MD			(38 | GPIO_ALT_FN_1_IN) 	
#define GPIO39_MMCCS1_MD		(39 | GPIO_ALT_FN_1_OUT)	
#define GPIO39_FFTXD_MD			(39 | GPIO_ALT_FN_2_OUT)	
#define GPIO40_FFDTR_MD			(40 | GPIO_ALT_FN_2_OUT)	
#define GPIO41_FFRTS_MD			(41 | GPIO_ALT_FN_2_OUT)	
#define GPIO42_BTRXD_MD			(42 | GPIO_ALT_FN_1_IN) 	
#define GPIO43_BTTXD_MD			(43 | GPIO_ALT_FN_2_OUT)	
#define GPIO44_BTCTS_MD			(44 | GPIO_ALT_FN_1_IN) 	
#define GPIO45_BTRTS_MD			(45 | GPIO_ALT_FN_2_OUT)	
#define GPIO46_ICPRXD_MD		(46 | GPIO_ALT_FN_1_IN) 	
#define GPIO46_STRXD_MD			(46 | GPIO_ALT_FN_2_IN) 	
#define GPIO47_ICPTXD_MD		(47 | GPIO_ALT_FN_2_OUT)	
#define GPIO47_STTXD_MD			(47 | GPIO_ALT_FN_1_OUT)	
#define GPIO48_nPOE_MD			(48 | GPIO_ALT_FN_2_OUT)	
#define GPIO49_nPWE_MD			(49 | GPIO_ALT_FN_2_OUT)	
#define GPIO50_nPIOR_MD			(50 | GPIO_ALT_FN_2_OUT)	
#define GPIO51_nPIOW_MD			(51 | GPIO_ALT_FN_2_OUT)	
#define GPIO52_nPCE_1_MD		(52 | GPIO_ALT_FN_2_OUT)	
#define GPIO53_nPCE_2_MD		(53 | GPIO_ALT_FN_2_OUT)	
#define GPIO53_MMCCLK_MD		(53 | GPIO_ALT_FN_1_OUT)	
#define GPIO54_MMCCLK_MD		(54 | GPIO_ALT_FN_1_OUT)	
#define GPIO54_pSKTSEL_MD		(54 | GPIO_ALT_FN_2_OUT)	
#define GPIO55_nPREG_MD			(55 | GPIO_ALT_FN_2_OUT)	
#define GPIO56_nPWAIT_MD		(56 | GPIO_ALT_FN_1_IN) 	
#define GPIO57_nIOIS16_MD		(57 | GPIO_ALT_FN_1_IN) 	
#define GPIO58_LDD_0_MD			(58 | GPIO_ALT_FN_2_OUT)	
#define GPIO59_LDD_1_MD			(59 | GPIO_ALT_FN_2_OUT)	
#define GPIO60_LDD_2_MD			(60 | GPIO_ALT_FN_2_OUT)	
#define GPIO61_LDD_3_MD			(61 | GPIO_ALT_FN_2_OUT)	
#define GPIO62_LDD_4_MD			(62 | GPIO_ALT_FN_2_OUT)	
#define GPIO63_LDD_5_MD			(63 | GPIO_ALT_FN_2_OUT)	
#define GPIO64_LDD_6_MD			(64 | GPIO_ALT_FN_2_OUT)	
#define GPIO65_LDD_7_MD			(65 | GPIO_ALT_FN_2_OUT)	
#define GPIO66_LDD_8_MD			(66 | GPIO_ALT_FN_2_OUT)	
#define GPIO66_MBREQ_MD			(66 | GPIO_ALT_FN_1_IN) 	
#define GPIO67_LDD_9_MD			(67 | GPIO_ALT_FN_2_OUT)	
#define GPIO67_MMCCS0_MD		(67 | GPIO_ALT_FN_1_OUT)							
#define GPIO68_LDD_10_MD		(68 | GPIO_ALT_FN_2_OUT)							
#define GPIO68_MMCCS1_MD		(68 | GPIO_ALT_FN_1_OUT)							
#define GPIO69_LDD_11_MD		(69 | GPIO_ALT_FN_2_OUT)							
#define GPIO69_MMCCLK_MD		(69 | GPIO_ALT_FN_1_OUT)							
#define GPIO70_LDD_12_MD		(70 | GPIO_ALT_FN_2_OUT)							
#define GPIO70_RTCCLK_MD		(70 | GPIO_ALT_FN_1_OUT)							
#define GPIO71_LDD_13_MD		(71 | GPIO_ALT_FN_2_OUT)							
#define GPIO71_3_6MHz_MD		(71 | GPIO_ALT_FN_1_OUT)							
#define GPIO72_LDD_14_MD		(72 | GPIO_ALT_FN_2_OUT)							
#define GPIO72_32kHz_MD			(72 | GPIO_ALT_FN_1_OUT)							
#define GPIO73_LDD_15_MD		(73 | GPIO_ALT_FN_2_OUT)							
#define GPIO73_MBGNT_MD			(73 | GPIO_ALT_FN_1_OUT)							
#define GPIO74_LCD_FCLK_MD		(74 | GPIO_ALT_FN_2_OUT)							
#define GPIO75_LCD_LCLK_MD		(75 | GPIO_ALT_FN_2_OUT)							
#define GPIO76_LCD_PCLK_MD		(76 | GPIO_ALT_FN_2_OUT)
#define GPIO77_LCD_ACBIAS_MD		(77 | GPIO_ALT_FN_2_OUT)
#define GPIO78_nCS_2_MD			(78 | GPIO_ALT_FN_2_OUT)
#define GPIO79_nCS_3_MD			(79 | GPIO_ALT_FN_2_OUT)
#define GPIO80_nCS_4_MD			(80 | GPIO_ALT_FN_2_OUT)


//-----------------------------------------------------------------------------
//
// INTERRUPT CONTROLLER
//
//-----------------------------------------------------------------------------

#define ICIP		__REG(0x40D00000)  // Interrupt Controller IRQ Pending Register  
#define ICMR		__REG(0x40D00004)  // Interrupt Controller Mask Register  
#define ICLR		__REG(0x40D00008)  // Interrupt Controller Level Register  
#define ICFP		__REG(0x40D0000C)  // Interrupt Controller FIQ Pending Register  
#define ICPR		__REG(0x40D00010)  // Interrupt Controller Pending Register  
#define ICCR		__REG(0x40D00014)  // Interrupt Controller Control Register  


#endif //_PXA250_HEADER_
