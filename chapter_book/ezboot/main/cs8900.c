//------------------------------------------------------------------------------
// 화일명 : cs8900.c
// 설  명 : 이더넷칩 cs8900을 제어한다.
// 작성자 : 유영창 (주)제이닷디엔티 frog@falinux.com
// 작성일 : 2001년 10월 8일
// 수  정 : 2003년 05월 20일  pxa255 를 위해 수정
// 저작권 : (주)제이닷디엔티 
// 주  의 : 
//------------------------------------------------------------------------------

//******************************************************************************
//
// 헤더 정의
//
//******************************************************************************
#include <pxa255.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <mem_map.h>
#include <cs8900.h>
#include <config.h>
//******************************************************************************
//
// 광역 변수 정의
//
//******************************************************************************


//------------------------------------------------------------------------------
// 설명 : 설정된 맥 어드레스의 주소를 얻는다. 
// 매개 : 없음
// 반환 : 맥 어드레스 
// 주의 : 없음
//------------------------------------------------------------------------------
Byte *CS8900_GetMacAddress( void ) 
{
  	return Cfg.Local_MacAddress;
}
//------------------------------------------------------------------------------
// 설명 : IO mode의 packet page point에 addr(packet page address)를 쓰고, 
//        IO mode의 packet page data에 value(packet page data)를 써서 값을 바꾼다.
// 매개 : addr  : packet page 주소.
//        value : packet page 주소의 위치에 들어갈 값.
// 반환 : 없음.
// 주의 : 없음.
//------------------------------------------------------------------------------
static void CS8900_WriteToPPR(short addr, short value)
{
	CS8900_PPPTR = addr;
	CS8900_PPDATA = value;
	return;
}	
//------------------------------------------------------------------------------
// 설명 : IO mode의 packet page point에 addr(packet page address)를 쓰고, 
//        IO mode의 packet page data에서 값을 읽어 return한다.
// 매개 : addr  : packet page 주소.
// 반환 : packet page addr에 있는 값.
// 주의 : 없음 
//------------------------------------------------------------------------------
static short CS8900_ReadFromPPR( short addr )
{
	CS8900_PPPTR = addr;
	return CS8900_PPDATA;
}
//------------------------------------------------------------------------------
// 설명 : packet을 전송.
//        전송 가능한 상태인지 확인한 후 IO_RTX_DATA에 packet을 반복적으로 기록.
// 매개 : txPktBuf : 전송할 패켓 어드레스 
//        len      : 패켓 크기  
// 반환 : packet page addr에 있는 값.
// 주의 : 없음 
//------------------------------------------------------------------------------
BOOL CS8900_Transmit( void *txPktBuf, int len )
{
	short   *endPtr, *s;
	unsigned int tick = 0;

	// Ethernet Tx on.
	CS8900_PPPTR = PP_LineCTL;
	CS8900_PPDATA |= SERIAL_TX_ON;

	// packet 전체가 CS8900A에 보내진 후 전송시작.
	CS8900_TXCMD = TX_AFTER_ALL;
	CS8900_TXLEN = len;

	// Tx가 준비될 때까지 대기.
//printf( "TX-Enter ");
	while ((CS8900_ReadFromPPR(PP_BusST) & READY_FOR_TX_NOW)==0)
	{
	        tick++;
	        if ( 0 == tick )
	        {
	                printf( " Error TX BIT\n");
	                return FALSE;
	        }
	}
//printf( " TX-Exit\n");

	// CS8900으로 packet 전송. 이것이 끝난 후 CS8900A가 RJ45로 packet 전송을 시작함.
	for (s=txPktBuf, endPtr=txPktBuf+len; s<endPtr; s++)
        {
		CS8900_RTX_DATA = *s;
                // printf( " %04X", SWAP16(*s) & 0xFFFF );
	}

	return TRUE;
}	
//------------------------------------------------------------------------------
// 설명 : 한 프레임 패킷을 수신한다. 
//        packet 전송을 받을 때는 IO_RTX_DATA에서 state, length, packet의 순서로 읽어 옴.
// 매개 : 
// 반환 : 값을 읽을 경우는 길이 / 없으면 0 
// 주의 : ip의 경우 udp까지 check한 후 각각을 처리할 수 있는 receive 함수 호출.
//------------------------------------------------------------------------------
int CS8900_Resive(void *rxPktBuf, int max_size )
{
	int	i;
	Word16	*s;
	short	isq, status, len, cnt;

        isq=CS8900_ISQ;

	// 수신 에러가 발생하였는가를 감시한다. 
	if( isq & ISQ_RX_MISS_EVENT )
	{
		 printf( "Miss RX\n" );
		 return 0;
	}	 

	// 수신 이벤트가 발생하였는가를 감시한다. 
	if( isq & ISQ_RECEIVER_EVENT )
        {
            
	    // 수신 상태를 얻는다. 
	    status = CS8900_RTX_DATA;
            len = CS8900_RTX_DATA;  // 수신된 길이를 얻는다. 
            
                // printf( "RESIVE EVENT [%04X]\n", status );  
                // printf( "RESIVE LENGTH %d \n", len ); 
           
            // 데이타가 틀리더라도 일단 읽어 온다. 
	    cnt = len/2 + len%2;
	    for (i=0, s=rxPktBuf; i<cnt; i++)
            {
                 *s = CS8900_RTX_DATA;
                  s++;
                        // printf( " [%04X]", SWAP16(*s) );
            }
            if( status & RX_OK ) return len;
	}
	return 0;

}

//------------------------------------------------------------------------------
// 역할 : CS8900이 네트워크에 링크 되어 있는가를 확인한다. 
// 매개 : 
// 반환 : 링크 되어 있으면 TRUE 실패하면 FALSE를 반환한다. 
// 주의 : 
//------------------------------------------------------------------------------
BOOL  CS8900_CheckLink( void )
{
        Word16 LineState;

        LineState = CS8900_ReadFromPPR( PP_LineST  );
                //printf( "CS8900 Line Link Check\n" ); // 읽은값 

	// check Link on.
	if( ! ( LineState & LINK_OK ) )
        {
 		return FALSE; //printf("Ethernet Link failed. Check line.\n");
 	}
        return TRUE;  //printf("Ethernet Link Ok\n");
}

//------------------------------------------------------------------------------
// 역할 : CS8900을 인터럽트를 쓰지 않는 IO모드로 초기화 한다. 
// 매개 : 
// 반환 : 성공하면 TRUE 실패하면 FALSE를 반환한다. 
// 주의 : 
//------------------------------------------------------------------------------
BOOL CS8900_Init( void )
{

        int lp;
	
        Word32 DectectID;
        Word16 ChipIDLow;
        Word16 ChipIDHigh;
        
        printf( "CS8900 Init............................\n" );

        printf( "  Mac Address  : " );
        
        printf( "[%02X %02X %02X %02X %02X %02X]\n", Cfg.Local_MacAddress[0],Cfg.Local_MacAddress[1],Cfg.Local_MacAddress[2],
                                                     Cfg.Local_MacAddress[3],Cfg.Local_MacAddress[4],Cfg.Local_MacAddress[5] ); 

	// 포인터 레지스터의 값을 읽어 CS8900 이 존재하는지 확인한다.
	
        CS8900_ENABLE_WORD;        // WORD 억세스를 이네이블 시킨다.  SHBE pin 토클 
        
        DectectID = CS8900_PPPTR;  // ID를 얻어 온다. 

        printf( "  Detect value : [%04X:%04X]\n", CS8900_DECTECT_MASK, DectectID & CS8900_DECTECT_MASK );
        
        if( ( DectectID & CS8900_DECTECT_MASK ) != CS8900_DECTECT_MASK )
        {
		    printf( "  Can't access to Memory of CS8900A.\n");
		    return FALSE;
        }

        // CS8900의 Chip ID 를 확인한다. 
        ChipIDLow  = CS8900_ReadFromPPR( PP_ChipID_LOW  );
        ChipIDHigh = CS8900_ReadFromPPR( PP_ChipID_HIGH );

        // 바이트 정렬을 수행한다. 
        ChipIDHigh  = SWAP16( ChipIDHigh  );
        ChipIDLow   = SWAP16( ChipIDLow   );

        printf( "  Chip ID      : [%04X:%04X]\n", ChipIDHigh, ChipIDLow ); // 읽은값 
        //printf( "       PRODUCT ID    : [%04X]\n", ChipIDHigh );                 // 칩 EISA에 등록된 제품 번호  
        //printf( "       VERSION       : [%04X]\n", ChipIDLow & 0x1F );           // 칩 버전 

	if ( ChipIDHigh != CHECK_CHIP_ID_HIGH )
        {
		printf("  Ethernet Chip is not CS8900A. Are you use CS8900A.\n");
		return FALSE;
	}

	// 송수신을 불가능하게 한다. 
	CS8900_WriteToPPR( PP_LineCTL, 0x0000 );  

        // 모든 인터럽트를 금지 시킨다. 
	CS8900_WriteToPPR(PP_CS8900_ISAINT, INTRQ_HIGH_IMPEDENCE );   // Interrupt number.  
	CS8900_WriteToPPR(PP_CS8900_ISADMA, DMRQ_HIGH_IMPEDENCE  );   // DMA Channel Number.

        // 폴링 방식으로 송수신이 가능하도록 상태 및 제어 레지스터를 초기화 한다. 
        CS8900_WriteToPPR(PP_RxCFG,	RX_OK_ENBL | RX_CRC_ERROR_ENBL);  
        CS8900_WriteToPPR(PP_RxCTL,	RX_OK_ACCEPT | RX_MULTICAST_ACCEPT | RX_IA_ACCEPT | RX_BROADCAST_ACCEPT);
	CS8900_WriteToPPR(PP_TxCFG,	TX_LOST_CRS_ENBL | TX_SQE_ERROR_ENBL | TX_OK_ENBL | TX_LATE_COL_ENBL | TX_JBR_ENBL | TX_ANY_COL_ENBL);
	CS8900_WriteToPPR(PP_TxCMD,	TX_START_ALL_BYTES);
	CS8900_WriteToPPR(PP_BufCFG,	0x0000);
	CS8900_WriteToPPR(PP_BusCTL,	ENABLE_IRQ | IO_CHANNEL_READY_ON);
	CS8900_WriteToPPR(PP_TestCTL,	0x0000);

        // 전송 명령 초기화 
	CS8900_WriteToPPR(PP_TxCommand,0x00c0);

        // MAC 어드레스 로직 필터 마스크 설정 
        // 여기서는 모든 패켓을 허가하게 설정한다. 
	for ( lp = 0 ; lp < 4 ; lp++ ) CS8900_WriteToPPR( PP_LAF+lp*2, 0xFFFF );

        // MAC 어드레스를 설정한다.  
        for ( lp = 0 ; lp < 3 ; lp++ ) CS8900_WriteToPPR( PP_IA+lp*2, *((Word16 *)&( Cfg.Local_MacAddress[lp*2])));

        // 송수신이 가능하게 만든다. 
        CS8900_WriteToPPR(PP_LineCTL, SERIAL_RX_ON | SERIAL_TX_ON);

   
        //printf( "  INIT OK!\n\n" );

	return TRUE;

}	


void  CS8900_SwReset( void )
{
        CS8900_Init();
        // 전송 명령 초기화 
//	CS8900_WriteToPPR(PP_TxCommand,0x00c0);
}

