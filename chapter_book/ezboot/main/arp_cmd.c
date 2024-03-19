//------------------------------------------------------------------------------
// 화일명 : arp_cmd.c
// 설  명 : arp 프로토콜을 처리한다.
// 
// 작성자 : 유영창 (주)제이닷디엔티 frog@falinux.com
// 작성일 : 2001년 10월 08일
// 수정일 : 2003년 06월 18일  
//          bootp 지원을 삭제하고 파일이름을 변경하였다.  -- 푸지
// 저작권 : (주)제이닷디엔티 
// 주  의 : 
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

#include <cs8900.h>
#include <net.h>
#include <config.h>


extern int Cfg_parse_args(char *cmdline, char **argv);
extern Byte StrToByte( char *ptr, int hex );

//******************************************************************************
//
// 광역 변수 정의
//
//******************************************************************************
static short __ICMP_SEQ    = 1;

//------------------------------------------------------------------------------
// 설명 : 호스트 Mac Address 를 구한다. 
// 매개 : 
// 반환 : 로컬 IP
// 주의 : 
//------------------------------------------------------------------------------
char *GetHostMacAddress( void )
{
        return Cfg.Host_MacAddress;
}

//------------------------------------------------------------------------------
// 설명 : 설정되어 있는 Local IP를 구한다. 
// 매개 : 
// 반환 : 로컬 IP
// 주의 : 
//------------------------------------------------------------------------------
Word32 GetLocalIP( void )
{
        return Cfg.Local_IP;
}
//------------------------------------------------------------------------------
// 설명 : 설정되어 있는 Host IP를 구한다. 
// 매개 : 
// 반환 : 호스트 IP
// 주의 : 
//------------------------------------------------------------------------------
Word32 GetHostIP( void )
{
        return Cfg.Host_IP;   
}

//------------------------------------------------------------------------------
// 설명 : IP 요청 ARP 패켓을 전송한다. 
// 매개 : 
// 반환 : 
// 주의 : 
//------------------------------------------------------------------------------
BOOL SendArpPacket( void )
{

    Byte	          BroadcastMAC[6] =  { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff } ; // 브로드 캐스팅 맥 어드레스 
    ARP_Packet_t      ARP_Packet; 
    //int lp;

    // 메모리 변수를 초기화 한다. 
    memset( &ARP_Packet, 0, sizeof( ARP_Packet ) );

    // BOOTP 영영을 설정한다. 
    SetArpRequestHeader     ( &ARP_Packet, CS8900_GetMacAddress(), GetLocalIP(), GetHostIP() );
    
    // 이더넷 패켓 정보 영역을 설정한다. 
    SetEthernetARPHeader( ( EthernetIP_t * ) &ARP_Packet , CS8900_GetMacAddress(), BroadcastMAC );

    // 패켓을 전송한다. 
    CS8900_Transmit( (char * ) &ARP_Packet, sizeof( ARP_Packet ) ); 
    return TRUE;
  
}

//------------------------------------------------------------------------------
// 설명 : ARP 응답 패켓을 처리 한다. 
// 매개 : rxPktBuf : 수신된 Packet 버퍼 주소 
//        len      : 수신된 길이 
// 반환 : 없음
// 주의 : 없음
//------------------------------------------------------------------------------
BOOL ARPReply( void *rxPktBuf, int len )
{
    int lp;
    // 수신된 ARP 패켓의 이상 유무를 검사한다. 
    if( CheckARPRxPacket( rxPktBuf, len, &(Cfg.Host_IP), &(Cfg.Local_IP) ) == TRUE )
    {  
        GetHostMacAddressFromArpPacket( ( ARP_Packet_t * ) rxPktBuf, Cfg.Host_MacAddress );
       	printf( "ARP PACKET Resive\n");
        // 호스트측 맥 어드레스를 표시 한다. 
        printf( "HOST MAC : [ " );
        for( lp = 0; lp < 6 ; lp++ ) printf( "%02X ", Cfg.Host_MacAddress[lp] & 0xFF );
        printf( "]\n" );
         
    }
    else
    {
       printf( "ARP PACKET Error\n");
       return FALSE;
    }
    return TRUE;
}

//------------------------------------------------------------------------------
// 설명 : 실제 ARP 를 수행하여 IP에 해당하는 MAC 어드레스를 얻어 온다. 
// 매계 : 없음
// 반환 : 에러 FALSE 정상 TRUE
// 주의 : 없음 
//------------------------------------------------------------------------------
BOOL    ARPProcess( void )
{
        char    RxBuff[1500];
        Word16  RxSize ; 
        Word16  Protocol;
        int     lp;

        // ARP Packet을 만들어서 보낸 후 1초간격으로 보내며
        // Reply에 의해서 MAC 어드레스를 
        for( lp = 0; lp < 10 ; lp++ )
        {
                memset( RxBuff, 0, sizeof( RxBuff ) );
                printf( "Send ARP Packet \n");
                SendArpPacket();
        
                ReloadTimer( 0, 1000 );          // set 1-sec
                while( 0 == TimeOverflow(0) )    // 시간 초과 검사 
                {
                        // 패켓 수신을 검사  
                        RxSize = CS8900_Resive( RxBuff, 1500 );
                        if( RxSize  )
                        {
                                // printf( "RESIVE OK [ size : %d ]\n", RxSize );
                                Protocol = GetProtocolFromRxPacket( ( EthernetIP_t * ) RxBuff, RxSize );

                                // printf( "Ethernet Protocol Type : [%04X]\n", Protocol );
                                if( Protocol == PROT_ARP )
                                {
                                        // printf( "Resive Protocol Type : [ARP]\n" ); 
                                        if( ARPReply( ( EthernetIP_t * ) RxBuff, RxSize ) == TRUE ) 
                                        {
                                                FreeTimer( 0 );  // 타이머 해제
                                                return TRUE;
                                        }    
                                } 
                        }
                } 
        }

        FreeTimer( 0 );  // 타이머 해제
        return FALSE;
}

//------------------------------------------------------------------------------
// 설명 : ECHO 패켓을 전송한다. 
// 매개 : 
// 반환 : 
// 주의 : 
//------------------------------------------------------------------------------
BOOL SendEchoPacket( void )
{

    ECHO_Packet_t      ECHO_Packet; 
    //int lp;

    // 메모리 변수를 초기화 한다. 
    memset( &ECHO_Packet, 0, sizeof( ECHO_Packet ) );

	ECHO_Packet.ICMP.icmp_type   = ECHO_REQUEST;	
	ECHO_Packet.ICMP.icmp_code   = 0;	
	ECHO_Packet.ICMP.icmp_cksum  = 0;	
	ECHO_Packet.ICMP.icmp_id     = SWAP16(8988);	
    ECHO_Packet.ICMP.icmp_seq    = __ICMP_SEQ++;   

    ECHO_Packet.ICMP.icmp_cksum  = SWAP16(GetCheckSumICMP(( Word16 * )&(ECHO_Packet.ICMP), sizeof( ECHO_Packet.ICMP ) ));

    // IP 정보 영역을 설정한다. 
    SetIPHeaderByICMP( &ECHO_Packet.IP,
                      GetHostIP(),              // 목적지 IP      
                      GetLocalIP(),             // Local    IP 
                      sizeof( ECHO_Packet.ICMP )
                    );
    
    // 이더넷 패켓 정보 영역을 설정한다. 
    SetEthernetIPHeader( ( EthernetIP_t * ) &ECHO_Packet , CS8900_GetMacAddress(), GetHostMacAddress() );

    // 패켓을 전송한다. 
    CS8900_Transmit( (char * ) &ECHO_Packet, sizeof( ECHO_Packet ) ); 
    return TRUE;
  
}

//------------------------------------------------------------------------------
// 설명 : ECHO 응답 패켓을 처리 한다. 
// 매개 : rxPktBuf : 수신된 Packet 버퍼 주소 
//        len      : 수신된 길이 
// 반환 : TRUE : 성공 / FALSE : 실패 
// 주의 : 없음
//------------------------------------------------------------------------------
BOOL    ECHOHandler( void *rxPktBuf, int len )
{

        if( CheckECHORxPacket( rxPktBuf, len, &(Cfg.Local_IP) ) == TRUE )
        {  
                return TRUE;
        }
        return FALSE;
}


//------------------------------------------------------------------------------
// 설명 : 실제 Bootp 를 수행하여 IP를 얻어 온다. 
// 매계 : 없음
// 반환 : 에러 FALSE 정상 TRUE
// 주의 : 없음 
//------------------------------------------------------------------------------
BOOL    EChoProcess( void )
{
        char    RxBuff[1500];
        Word16  RxSize ; 
        Word16  Protocol;
        int     lp;

        // Bootp Packet을 만들어서 보낸 후 1초간격으로 보내며
        // Reply에 의해서 IP를 얻는다. 
        for( lp = 0; lp < 10 ; lp++ )
        {
                memset( RxBuff, 0, sizeof( RxBuff ) );
                printf( "Send Echo Packet \n");
                SendEchoPacket();
        
                ReloadTimer( 0, 1000 );            // set 1-sec
                while( 0 == TimeOverflow(0) )    // 시간 초과 검사 
                {
                        // 패켓 수신을 검사  
                        RxSize = CS8900_Resive( RxBuff, 1500 );
                        if( RxSize  )
                        {
                                // printf( "RESIVE OK [ size : %d ]\n", RxSize );
                                Protocol = GetProtocolFromRxPacket( ( EthernetIP_t * ) RxBuff, RxSize );

                                // printf( "Ethernet Protocol Type : [%04X]\n", Protocol );
                                if( Protocol == PROT_IP )
                                {
                                        // printf( "Resive Protocol Type : [IP]\n" ); 
                                        if( ECHOHandler( RxBuff, RxSize ) == TRUE ) 
                                        {
                                                printf( "PING TEST GOOD\n" );
                                                FreeTimer( 0 );  // 타이머 해제
                                                return TRUE;
                                        }    
                                } 
                        }
                } 
        }

        FreeTimer( 0 );  // 타이머 해제
        printf( "PING TEST FAIL\n" ); 
        return FALSE;
}

//------------------------------------------------------------------------------
// 설명 : 필요한 네트워크 정보를 재 설정한다. 
// 매계 : 
// 반환 : 
// 주의 : 없음 
//------------------------------------------------------------------------------
BOOL AutoRebuildNetworkInfo( void )
{
	if(   ( GetLocalIP() == 0          )
           || ( GetLocalIP() == 0xffffffff )
           || ( GetHostIP () == 0          ) 
           || ( GetHostIP () == 0xffffffff ) )
      	{
		printf( "- Invalid [LOCAL IP] or [HOST IP], Type [SET] command\n" );
		return FALSE;
      	}

      	return ARPProcess();
}
//------------------------------------------------------------------------------
// 설명 : Arp를 이용하여 Host의 Mac 어드레스를 얻어온다. 
// 매계 : argc    : 토큰 갯수 
//        argv    : 분리된 토큰 문자열 주소가 담겨질 배열 
// 반환 : 에러 -1  정상 0
// 주의 : 없음 
//------------------------------------------------------------------------------
int     Arp (int argc, char **argv)
{
        
        // 이더넷이 링크 되어 있는가를 확인한다. 
        if( CS8900_CheckLink() == FALSE )
        {
                printf( "Ethernet Link failed. Check line.\n" );
                return -1;
        }
        
        // 기존 정보를 없앤다.   
        memset( Cfg.Host_MacAddress, 0, sizeof( Cfg.Host_MacAddress ) );
        ARPProcess();
        
        return TRUE; 
}
//------------------------------------------------------------------------------
// 설명 : ICMPp를 ECHO 처리 
// 매계 : argc    : 토큰 갯수 
//        argv    : 분리된 토큰 문자열 주소가 담겨질 배열 
// 반환 : 에러 -1  정상 0
// 주의 : 없음 
//------------------------------------------------------------------------------
int     Ping( int argc, char **argv )
{
        char    *str[128];
        int     adrcnt;
        Word32  TargetIP;
        Word32  OldHostIP;

        if ( argc < 2 )
        {
                printf( "Error argument count\n" );
                return -1;
        }

        // 사용자로부터 IP 를 얻어온다.
        adrcnt = Cfg_parse_args( argv[1], str );

        if ( adrcnt == 4 )
        {
          TargetIP =   ( StrToByte(str[3],0) << 24 )
                   |   ( StrToByte(str[2],0) << 16 )
                   |   ( StrToByte(str[1],0) << 8  )
                   |   ( StrToByte(str[0],0)       );
        }
        else
        {
                printf( "Error IP Address\n" );
                return -1;
        }


        OldHostIP = Cfg.Host_IP;
        Cfg.Host_IP = TargetIP;

        // 이더넷이 링크 되어 있는가를 확인한다. 
        if( CS8900_CheckLink() == FALSE )
        {
                printf( "Ethernet Link failed. Check Line.\n" );
                return -1;
        }

        if( AutoRebuildNetworkInfo() == FALSE ) return FALSE;

        EChoProcess();

        Cfg.Host_IP = OldHostIP;
        
        return TRUE; 
}
