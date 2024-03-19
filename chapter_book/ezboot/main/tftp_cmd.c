//------------------------------------------------------------------------------
// 화일명 : tftp_cmd.c
// 설  명 : ezBoot의 TFTP를 이용하여 화일 데이타를 얻어오는 루틴이다. 
// 
// 작성자 : 유영창 (주)제이닷디엔티 frog@falinux.com
//          오재경 (주)제이닷디앤티 freefrug@falinux.com
// 작성일 : 2001년 10월 8일
// 수  정 : 2003년 6월 05일
// 저작권 : (주)제이닷디엔티 
// 주  의 : 
// 
//------------------------------------------------------------------------------

//******************************************************************************
//
// 헤더 정의
//
//******************************************************************************
#include <pxa255.h>
#include <serial.h>
#include <time.h>
#include <gpio.h>
#include <stdio.h>
#include <string.h>

#include <mem_map.h>
#include <cs8900.h>
#include <net.h>
#include <config.h>
#include <flash.h>


//******************************************************************************
//
// 광역 변수 정의
//
//******************************************************************************

Word16 __TFTPLastBlockNumber = 0; // 마지막으로 수신한 블럭 번호 
Word16 __TFTPHostPortNumber  = TFTP_PORT_HOST;      // 호스트의 포트 번호 
Word32 __TFTPResiveTotalSize = 0; // 수신된 총 크기 

// config.c 에 정의
extern TConfig Cfg;

//------------------------------------------------------------------------------
// 설명 : TFTP  패켓을 전송한다. 
// 매개 : ptrTFTP  : 전송 Tftp 패켓 주소 
//        HostPortNumber : 호스트의 포트 번호    
//        tftpsize : Tftp 데이타의 크기 
// 반환 : 
// 주의 : tftpsize가 전송되는 프레임 전체 크기가 아님 
//------------------------------------------------------------------------------
BOOL SendTFTPPacket( TFTP_Packet_t *ptrTFTP, Word16 HostPortNumber, int tftpsize )
{

    // UDP 정보 영역을 설정한다. 
    SetUdpHeader( &(ptrTFTP->UDP), HostPortNumber, TFTP_PORT_CLIENT, tftpsize  );

    // IP 정보 영역을 설정한다. 
    SetIPHeaderByUDP( &(ptrTFTP->IP),          
                      GetHostIP(),         // Host IP      
                      GetLocalIP(),        // Local IP 
                      sizeof( ptrTFTP->UDP ) + tftpsize 
                    );
 
    // 이더넷 패켓 정보 영역을 설정한다. 
    SetEthernetIPHeader( &(ptrTFTP->ETHERNET), CS8900_GetMacAddress(), GetHostMacAddress() );
   
    // 패켓을 전송한다. 
    CS8900_Transmit( ptrTFTP, sizeof( ptrTFTP->ETHERNET ) 
                            + sizeof( ptrTFTP->IP ) 
                            + sizeof( ptrTFTP->UDP ) + tftpsize 
                   ); 

    return TRUE;
}
//------------------------------------------------------------------------------
// 설명 : Tftp 응답 패켓을 처리 한다. 
// 매개 : ptrTFTP   : 수신된 TFTP Packet 버퍼 주소 
//        len       : 수신된 길이 
//        LoadAddr  : 저장해야할 메모리 선두 번지
// 반환 : 수신 결과 
// 주의 : 없음
//------------------------------------------------------------------------------
int TftpResiveHandler( TFTP_Packet_t *ptrTFTP, int len, Word32 LoadAddr )
{
    Word16  *ptrBlockNumber;
    Word16  *ptrErrorNumber;
    char    *SrcDataStr;   // 패켓의 데이타 번지 주소 
    char    *DestDataStr;  // 저장 위치 데이타 번지 주소 
    int      DataSize;
    Word16   Opcode;
    int      lp,opcnt;
    

    DataSize = len - sizeof( TFTP_Packet_t ) - 2;

    ptrBlockNumber = (Word16 *) ptrTFTP->Data; 
    ptrErrorNumber = (Word16 *) ptrTFTP->Data; 

    SrcDataStr = ptrTFTP->Data + 2;  // 데이타 부분 

    Opcode = SWAP16( ptrTFTP->OPCODE );
//    printf( "OPCODE [%d]\n", Opcode ); 

    switch ( Opcode )
    { 
    case TFTP_RRQ : // 처리하지 않을 내용에 대하여 무시한다. 
                    
    case TFTP_WRQ :
    case TFTP_ACK :
    default       : 
                    break;
    
    case TFTP_ERROR : // error 처리.
		      printf( "\nTFTP Error : [ %d ] %s \n", SWAP16( *ptrErrorNumber ), SrcDataStr );

                      return TFTP_STATE_FAILURE;
    case TFTP_DATA  : // 수신된 데이타 처리 
             
                      // 블럭 크기가 0이면  
                      if( SWAP16( *ptrBlockNumber ) == 0 )
                      {
                         printf( "\nTFTP Error : Block Number Zero\n" );
                         return TFTP_STATE_FAILURE;    
                      }
                      
                      // 데이타 블럭 번호를 검사 한다. 
                      // 블럭 번호가 큰것만 처리  
                      if( __TFTPLastBlockNumber < SWAP16( *ptrBlockNumber ) )
                      {
                          __TFTPLastBlockNumber = SWAP16( *ptrBlockNumber );
                          // 저장할 메모리 위치를 계산한다. 
                          DestDataStr = ( char * ) ( LoadAddr + ( ( __TFTPLastBlockNumber-1) * 512 ) );
			  // 메모리를 복사한다. 
			  memcpy( DestDataStr, SrcDataStr, DataSize );
	
                          __TFTPResiveTotalSize += DataSize;
                      }
                      
                      // 데이타의 크기가 512가 아니라면 모든 데이타를 받은 것이다.                       
                      if( DataSize < 512 ) return TFTP_STATE_END;
                      return TFTP_STATE_DATA_OK;
                      
    case TFTP_OACK  : // 옵션 붙은 수신 데이타 처리    // 타임 아웃 추가 사항 
                      opcnt = 0; 
                      printf( "OPTION [" );
                      for( lp = 0; lp < 64; lp++ )
                      {
                        if( ptrTFTP->Data[lp] == 0 ) 
                        {
                            opcnt++;
                            if( opcnt == 4 ) break;        
                            printf( " " );
                        }         
                        else
                        {
                            printf( "%c", ptrTFTP->Data[lp] );
                        }    
                      }
                      printf( "]\n" );
                      return TFTP_STATE_DATA_OK;
                      
    } 

    return TFTP_NONE;
}
//------------------------------------------------------------------------------
// 설명 : Bootp를 이용하여 화일 데이타를 얻어 온다. 
// 매계 : mHostIP  : 접근할 Host IP
//        mLocalIP : 보드 IP
//        LoadAddr : 저장할 어드레스  
// 반환 : 에러 -1  정상 0
// 주의 : 없음 
//------------------------------------------------------------------------------
BOOL 	TftpProcess( Word32 mHostIP, Word32 mLocalIP, Word32 LoadAddr, char *FileName  )
{
    	Word32         IPAddr; 
    	char           TxBuff[1500];
    	char           RxBuff[1500];
    	Word16         RxSize ; 
    	Word16         Protocol;

    	TFTP_Packet_t *ptrTFTP;
    	int            size;
    
    	//CS8900_SwReset();

    	if( AutoRebuildNetworkInfo( )  == FALSE )
    	{
        	printf( "- Fail Network Infomation\n" );
        	return FALSE;
    	}

    	// HOST 와 LOCAL IP를 표출한다. 
    	IPAddr = SWAP32( mHostIP );
    	printf( "HOST  IP : [%d.%d.%d.%d]\n", ( IPAddr >> 24 ) & 0xff,
                                              ( IPAddr >> 16 ) & 0xff,
                                              ( IPAddr >>  8 ) & 0xff,
                                              ( IPAddr >>  0 ) & 0xff );
        
    	IPAddr = SWAP32( mLocalIP );
    	printf( "LOCAL IP : [%d.%d.%d.%d]\n", ( IPAddr >> 24 ) & 0xff,
                                              ( IPAddr >> 16 ) & 0xff,
                                              ( IPAddr >>  8 ) & 0xff,
                                              ( IPAddr >>  0 ) & 0xff );

   	// 데이타를 저장할 어드레스를 표현한다. 
   	printf( "Resive Address  : %04X-%04X\n", ( LoadAddr >> 16 ) & 0xFFFF, LoadAddr & 0xFFFF );

   	// 전송 요구 패켓을 전송한다. 
   	printf( "TFTP Request Send\n" );

   	// 메모리 변수를 초기화 한다. 
   	memset( TxBuff, 0, sizeof( TxBuff ) );
   	ptrTFTP = ( TFTP_Packet_t * ) TxBuff;

   	size = SetTFTP_RRQPacket( ptrTFTP , FileName );
   	SendTFTPPacket( ptrTFTP, TFTP_PORT_HOST, size );

   	__TFTPLastBlockNumber = 0; // 블럭 번호를 초기화 한다. 
   	__TFTPResiveTotalSize = 0; // 수신된 총 크기 
 
   	ReloadTimer( 0, 5000 ); // 타이머0  5초
   	while( 1 )   
   	{
            // 패켓 수신을 검사  
            RxSize = CS8900_Resive( RxBuff, 1500 );
            if( RxSize  )
            {
                // printf( "RESIVE OK [ size : %d ]\n", RxSize );
                Protocol = GetProtocolFromRxPacket( ( EthernetIP_t * ) RxBuff, RxSize );

                // printf( "Ethernet Protocol Type : [%04X]\n", Protocol );
        	if (Protocol == PROT_ARP) 
                {
                    ARP_Packet_t *pARP = (ARP_Packet_t *)RxBuff;
                    if (SWAP16(pARP->ar_op) == ARPOP_REQUEST) 
                    {                                   
                            ARP_Packet_t arp_pk;
                            Word32  local_ip = Cfg.Local_IP;
                            Word32  host_ip  = Cfg.Host_IP;
                                
                            memset(&arp_pk, 0, sizeof(ARP_Packet_t));                                 
                            arp_pk.ar_hrd = SWAP16(HWT_ETHER); 
                            arp_pk.ar_pro = SWAP16(PROT_IP);                                          
                            arp_pk.ar_hln = 6;                                                        
                            arp_pk.ar_pln = 4;                                                        
                            arp_pk.ar_op  = SWAP16(ARPOP_REPLY); // Operation                         
                            arp_pk.ar_hrd = SWAP16(HWT_ETHER);
                            memcpy(arp_pk.SenderMac, CS8900_GetMacAddress(), sizeof(arp_pk.SenderMac));            
                            memcpy(arp_pk.TargetMac, pARP->SenderMac, sizeof(arp_pk.TargetMac));                   
                            arp_pk.SenderIP = local_ip;                                               
                            arp_pk.TargetIP = host_ip;
                                
                            memcpy((char *) arp_pk.ETHERNET.et_dest, pARP->SenderMac, sizeof(arp_pk.ETHERNET.et_dest));                         
                            memcpy((char *) arp_pk.ETHERNET.et_src,  CS8900_GetMacAddress(), sizeof(arp_pk.ETHERNET.et_src));                          
                            arp_pk.ETHERNET.et_protocol = SWAP16(PROT_ARP);                           
                            CS8900_Transmit((char * )&arp_pk, sizeof(ARP_Packet_t));                  
                    }
                            
                    continue;
                }

		if( Protocol != PROT_IP ) continue;
        	
                // printf( "Resive Protocol Type : [IP]\n" );  
                if( CheckTftpRxPacket( ( TFTP_Packet_t * ) RxBuff, RxSize, &mLocalIP ) )
                {
                     __TFTPHostPortNumber = GetHostPortNumberFromTftpPacket( ( TFTP_Packet_t * ) RxBuff ); 
                     // printf( "Resive TFTP Packet [ Host Port : %d ] \n", __TFTPHostPortNumber ); 
                     // HOST의 포트 번호를 얻는다. 
                   

                     // 처리한다. 
                     switch( TftpResiveHandler( ( TFTP_Packet_t * ) RxBuff, RxSize, LoadAddr ) )
                     {
                     case TFTP_STATE_FAILURE    : // 에러 발생 
                                                  // 모든 진행을 중지한다. 
                                                  printf( "FAILURE BREAK\n" ); 
                                                  break;
                     case TFTP_STATE_DATA_OK    : // 데이타 정상 수신 
             
                                                  size = SetTFTP_ACKDATAPacket( ptrTFTP , __TFTPLastBlockNumber );
                                                  SendTFTPPacket( ptrTFTP, __TFTPHostPortNumber, size );
                                                  
                                                  // printf( "DATA ACK [ Block Number : %d ]\n", __TFTPLastBlockNumber ); 
                                                  if( ( __TFTPLastBlockNumber % 10 ) == 0 )
                                                      printf( "\rSize:%d KB", __TFTPResiveTotalSize / 1024 ); 
                                                  
                                                  ReloadTimer( 0, 5000 ); // 타이머0  5초
                                                  continue;
                     case TFTP_STATE_DATA_FAILE : // 데이타 에러 발생 
                                                  printf( "DATA FAIL\n" );  
                                                  break;
                     case TFTP_STATE_END        : // 모든 데이타를 받음 
                                                  size = SetTFTP_ACKDATAPacket( ptrTFTP , __TFTPLastBlockNumber );
                                                  SendTFTPPacket( ptrTFTP, __TFTPHostPortNumber, size );
                                                  
                                                  printf( "\rALL DATA RESIVE OK [ %d bytes ]\n", __TFTPResiveTotalSize ); 
                                                  FreeTimer( 0 );
                                                  return TRUE; 
                     } 
                }  
            }

            // 시간 초과 검사 
            if( TimeOverflow(0) )
            {
                 printf( "Time Overflow\n" );
                 break;
            }
            
   	} 

    	FreeTimer( 0 );
    	return FALSE;

}
//------------------------------------------------------------------------------
// 설명 : Tftp를 이용하여 화일 데이타를 얻어 온다. 
// 매계 : argc    : 토큰 갯수 
//        argv    : 분리된 토큰 문자열 주소가 담겨질 배열 
// 반환 : 에러 -1  정상 0
// 주의 : 없음 
//------------------------------------------------------------------------------
int     Tftp (int argc, char **argv)
{
        unsigned long  To;
        char           *Filename;

        printf( "tftp Command\n");
        if( argc < 3 ) 
        {
                printf( "Argument Count Error!\n");
                return -1; 
        }

        // 화일이름을 얻는다. 
        Filename = argv[2];
        // 써넣을 주소를 얻는다.  
        // 주소에 대한 검증은 하지 않았다.
        To = strtoul( argv[1], NULL, 0);

        // TFTP로 데이타를 받는다.
        if( TftpProcess( GetHostIP(), GetLocalIP(), DEFAULT_RAM_WORK_START, Filename ) == TRUE )
        {
                if ( 'F' == argv[0][1] ) // 플래시 영역에 쓴다. 
                {
                        printf( "아직 하지 않았다\n" );
                        //To &= 0x0fffffff;
                        //Flash_WriteFull( To, DEFAULT_RAM_WORK_START, __TFTPResiveTotalSize );
                }
                else
                {
                        memcpy( (void *)To, (void *)DEFAULT_RAM_WORK_START, __TFTPResiveTotalSize );
                }
        } 

        return 0;
}
//------------------------------------------------------------------------------
// 설명 : TFTP로 데이타를 받은후 부트에 써 넣기 
// 매계 : argc    : 토큰 갯수 
//        argv    : 분리된 토큰 문자열 주소가 담겨질 배열 
// 반환 : 에러 -1  정상 0
// 주의 : 없음 
//------------------------------------------------------------------------------
int     Tftp_FlashBoot(int argc, char **argv)
{
        int     size = 0;
        char    FileName[256];

        // 부트로더 파일이름
        strcpy( FileName, Cfg.TFTP_Directory );
        strcat( FileName, Cfg.TFTP_LoaderFileName );
        printf( "Receive %s\n", FileName );

        // TFTP로 데이타를 받는다.
        if( TftpProcess( GetHostIP(), GetLocalIP(), DEFAULT_RAM_WORK_START, FileName ) == TRUE )
        {
        	// 부트 영역에 츨래쉬를 쓴다. 
        	CopyTo_BootFlash( DEFAULT_FLASH_BOOT, DEFAULT_RAM_WORK_START, __TFTPResiveTotalSize );
        } 
        
        return size;
}

//------------------------------------------------------------------------------
// 설명 : TFTP로 데이타를 받은후 커널에 써 넣기 
// 매계 : argc    : 토큰 갯수 
//        argv    : 분리된 토큰 문자열 주소가 담겨질 배열 
// 반환 : 에러 -1  정상 0
// 주의 : 없음 
//------------------------------------------------------------------------------
int     Tftp_Kernel(int argc, char **argv)
{
        char    FileName[256];

        // 커널이미지 파일이름
        strcpy( FileName, Cfg.TFTP_Directory );
        strcat( FileName, Cfg.TFTP_zImageFileName );
        printf( "Receive %s\n", FileName );

        // TFTP로 데이타를 받는다.
        if( TftpProcess( GetHostIP(), GetLocalIP(), DEFAULT_RAM_WORK_START, FileName ) == TRUE )
        {
                if ( 'F' == argv[0][1] ) // 플래시 영역에 쓴다. 
                {
                	CopyTo_NandFlash_Kernel( DEFAULT_RAM_WORK_START, __TFTPResiveTotalSize );
                }
                else
                {
                 	memcpy( (void *)(DEFAULT_RAM_KERNEL_START), (void *)DEFAULT_RAM_WORK_START, __TFTPResiveTotalSize );
                }
        } 

        return 0;
}
//------------------------------------------------------------------------------
// 설명 : TFTP로 데이타를 받은후 램디스크 영역에 써 넣기 
// 매계 : argc    : 토큰 갯수 
//        argv    : 분리된 토큰 문자열 주소가 담겨질 배열 
// 반환 : 에러 -1  정상 0
// 주의 : 없음 
//------------------------------------------------------------------------------
int     Tftp_RamDisk(int argc, char **argv)
{
        char    FileName[256];

        // 램디스크이미지 파일이름
        strcpy( FileName, Cfg.TFTP_Directory );
        strcat( FileName, Cfg.TFTP_RamDiskFileName );
        printf( "Receive %s\n", FileName );

        // TFTP로 데이타를 받는다.
        if( TftpProcess( GetHostIP(), GetLocalIP(), DEFAULT_RAM_WORK_START, FileName ) == TRUE )
        {
                if ( 'F' == argv[0][1] ) // 플래시 영역에 쓴다. 
                {
                        CopyTo_NandFlash_Ramdisk( DEFAULT_RAM_WORK_START, __TFTPResiveTotalSize );
                }
                else
                {
                        memcpy( (void *)(DEFAULT_RAM_RAMDISK_START), (void *)DEFAULT_RAM_WORK_START, __TFTPResiveTotalSize );
                }
        } 
        
        return 0;
}


