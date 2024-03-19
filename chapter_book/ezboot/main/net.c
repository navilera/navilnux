//------------------------------------------------------------------------------
// 화일명 : net.c
// 설  명 : ezBoot의 Network 정보를 처리하는 루틴이다. 
// 
// 작성자 : 유영창 (주)제이닷디엔티 frog@falinux.com
// 작성일 : 2001년 10월 8일
// 저작권 : (주)제이닷디엔티 
//          
// 주  의 : 
//------------------------------------------------------------------------------

#include <pxa255.h>
#include <stdio.h>
#include <string.h>

#include <time.h>
#include <cs8900.h>
#include <net.h>


//******************************************************************************
//
// 광역 변수 정의
//
//******************************************************************************
static short __IP_ID    = 0;
static Word32 __BootpID = 0;                      // Bootp Transation ID 

//------------------------------------------------------------------------------
// 설명 : 수신한 packet의 checksum을 check.
// 매개 : ptr : IP header의 주소.
//        len : IP header의 길이.
// 반환 : IP header에서 구한 checksum 값.
// 주의 : 
//------------------------------------------------------------------------------
unsigned IPChecksum(char *ptr, int len)
{
	Word32		xsum;

	xsum = 0;
	while (len-- > 0) xsum += *((Word16 *)ptr)++;

	xsum = (xsum & 0xffff) + (xsum >> 16);
	xsum = (xsum & 0xffff) + (xsum >> 16);

	return xsum & 0xffff;
}	

//------------------------------------------------------------------------------
// 역할 : 수신한 packet의 checksum을 check.
// 매개 : ptr : IP header의 주소.
//        len : IP header의 길이.
// 반환 : 0 : failure,		1 : success
// 주의 : 
//------------------------------------------------------------------------------
int IPChksumOK( char *ptr, int len )
{
	return !((IPChecksum(ptr, len) + 1) & 0xfffe);
}	

//------------------------------------------------------------------------------
// 설명 : 수신된 이더넷 패켓이 어떤 프로토콜인가를 얻어온다. 
// 매개 : rxPktBuf : 수신된 Packet 버퍼 주소 
//        len      : 수신된 길이 
// 반환 : 프로토콜 타입 번호 
// 주의 : 
//------------------------------------------------------------------------------
Word16 GetProtocolFromRxPacket( EthernetIP_t *rxPktBuf, int len )
{
        return SWAP16( rxPktBuf->et_protocol ); 
}
//------------------------------------------------------------------------------
// 설명 : 수신된 이더넷 패켓이 Bootp 프로토콜인가를 얻어온다. 
// 매개 : rxPktBuf : 수신된 Packet 버퍼 주소 
//        len      : 수신된 길이 
//        ClientIP : 할당된 IP 
// 반환 : 에러가 없다면 TRUE 있다면 FALSE;
// 주의 : 
//------------------------------------------------------------------------------
BOOL CheckBootpRxPacket( BOOTP_Packet_t *rxPktBuf, int len, Word32 *ClientIP )
{

        // UDP를 포함한 크기보다 작은가를 검사 

	if(len < ( sizeof( EthernetIP_t ) + sizeof( IP_t ) + sizeof( UDP_t ) ) ) return FALSE;	
        if(len < ( sizeof( EthernetIP_t ) + SWAP16( rxPktBuf->IP.ip_len)             ) ) return FALSE;
	
        // ip version이 4인가를 검사한다.  
	if( ( rxPktBuf->IP.ip_hl_v & 0xf0 ) != 0x40 ) return FALSE; 
        
	// Can't deal fragments.
	if( rxPktBuf->IP.ip_off & SWAP16(0x1fff)) return FALSE;
	
    // check checksum.
	if(!IPChksumOK((char *) &rxPktBuf->IP, sizeof( rxPktBuf->IP ) / 2) ) return FALSE ; 
	
    // client의 ip와 받은 packet의 ip 비교.
	if( *ClientIP && memcmp(&rxPktBuf->IP.ip_dst, ClientIP, 4) ) return FALSE; 
	
        // udp인지 check. 17은 udp의 번호. 
	if( rxPktBuf->IP.ip_p != IPPROTO_UDP )                   return FALSE; 

        // BOOTP 포트 번호 검사 	
	if( rxPktBuf->UDP.udp_src != SWAP16( PORT_BOOTPS ) )     return FALSE;
        if( rxPktBuf->UDP.udp_dest!= SWAP16( PORT_BOOTPC ) ) return FALSE; 

        // BOOTP 패켓과 같은 크기인가를 확인한다. 
	if (len != sizeof( BOOTP_Packet_t ) )		             return FALSE;

	// BOOTP 패켓 정보인가를 검사 
	if( rxPktBuf->BOOTP.bh_opcode  !=2 )		             return FALSE;
	if( rxPktBuf->BOOTP.bh_htype   !=1 )			         return FALSE;
	if( rxPktBuf->BOOTP.bh_hlen    !=6 )			         return FALSE;
	if( memcmp(&rxPktBuf->BOOTP.bh_tid, &__BootpID, 4))      return FALSE;

	// get infomation from bootp packet.

        return TRUE;
}

//------------------------------------------------------------------------------
// 설명 : 수신된 이더넷 패켓이 ECHO 프로토콜인가를 얻어온다. 
// 매개 : rxPktBuf : 수신된 Packet 버퍼 주소 
//        len      : 수신된 길이 
//        ClientIP : 할당된 IP 
// 반환 : 에러가 없다면 TRUE 있다면 FALSE;
// 주의 : 
//------------------------------------------------------------------------------
BOOL CheckECHORxPacket( ECHO_Packet_t *rxPktBuf, int len, Word32 *ClientIP )
{

    // UDP를 포함한 크기보다 작은가를 검사 

	if(len < sizeof( ECHO_Packet_t ) ) return FALSE;	
	
    // ip version이 4인가를 검사한다.  
	if( ( rxPktBuf->IP.ip_hl_v & 0xf0 ) != 0x40 ) return FALSE; 
	
        
	// Can't deal fragments.
	if( rxPktBuf->IP.ip_off & SWAP16(0x1fff)) return FALSE;
	
	
    // check checksum.
	if(!IPChksumOK((char *) &rxPktBuf->IP, sizeof( rxPktBuf->IP ) / 2) ) return FALSE ; 
	
	
    // client의 ip와 받은 packet의 ip 비교.
	if( *ClientIP && memcmp(&rxPktBuf->IP.ip_dst, ClientIP, 4) ) return FALSE; 
	
	
    // udp인지 check. 17은 udp의 번호. 
	if( rxPktBuf->IP.ip_p != IPPROTO_ICMP )                  return FALSE; 
	

	// ECHO Request 인가를 검사 
	if( rxPktBuf->ICMP.icmp_type  != ECHO_REPLY )		             return FALSE;
	

    return TRUE;
}


//------------------------------------------------------------------------------
// 설명 : 수신된 이더넷 패켓이 Tftp 프로토콜인가를 얻어온다. 
// 매개 : rxPktBuf : 수신된 Packet 버퍼 주소 
//        len      : 수신된 길이 
//        ClientIP : 할당된 IP 
// 반환 : 에러가 없다면 TRUE 있다면 FALSE;
// 주의 : 
//------------------------------------------------------------------------------
BOOL CheckTftpRxPacket( TFTP_Packet_t *rxPktBuf, int len, Word32 *ClientIP )
{

    // UDP를 포함한 크기보다 작은가를 검사 

	if(len < ( sizeof( EthernetIP_t ) + sizeof( IP_t ) + sizeof( UDP_t ) ) ) return FALSE;	
        if(len < ( sizeof( EthernetIP_t ) + SWAP16( rxPktBuf->IP.ip_len)             ) ) return FALSE;
    // ip version이 4인가를 검사한다.  
	if( ( rxPktBuf->IP.ip_hl_v & 0xf0 ) != 0x40 ) return FALSE; 

	// Can't deal fragments.
	if( rxPktBuf->IP.ip_off & SWAP16(0x1fff)) return FALSE;

    // check checksum.
	if(!IPChksumOK((char *) &rxPktBuf->IP, sizeof( rxPktBuf->IP ) / 2) ) return FALSE ; 

    // client의 ip와 받은 packet의 ip 비교.
	if( *ClientIP && memcmp(&rxPktBuf->IP.ip_dst, ClientIP, 4) ) return FALSE; 
        // udp인지 check. 17은 udp의 번호. 
	if( rxPktBuf->IP.ip_p != IPPROTO_UDP ) return FALSE; 

    // TFTP 포트 인가를 검사한다. 
    // 쏘스 포트는 Listen포트와 데이타 포트가 다르기 때문에 비교하지 않는다. 
	if( rxPktBuf->UDP.udp_dest != SWAP16( TFTP_PORT_CLIENT ) ) return FALSE;

    // TFTP 패켓의 최소 크기 보다 큰가를 확인한다. 
	if (len < sizeof( TFTP_Packet_t ) )		        return FALSE;

        return TRUE;
}


//------------------------------------------------------------------------------
// 설명 : 수신된 이더넷 Bootp 패켓에서 LocalIP와 HostIP를 얻어온다. 
// 매개 : rxPktBuf : 수신된 Packet 버퍼 주소 
//        LocalIP  : 할당된 로컬 IP 
//        HostIP   : 할당된 호스트 IP
// 반환 : 에러가 없다면 TRUE 있다면 FALSE;
// 주의 : 
//------------------------------------------------------------------------------
BOOL  GetIPFromBootpPacket( BOOTP_Packet_t *rxPktBuf, Word32 *LocalIP, Word32 *HostIP )
{

        *LocalIP = rxPktBuf->BOOTP.bh_yiaddr;
        *HostIP  = rxPktBuf->BOOTP.bh_siaddr;
        return TRUE;
}
//------------------------------------------------------------------------------
// 설명 : 수신된 이더넷 패켓에서 송신 측 맥 어드레스를 얻어온다. 
// 매개 : rxPktBuf       : 수신된 Packet 버퍼 주소 
//        HostMacAddress : 할당된 로컬 IP 
//        HostIP   : 할당된 호스트 IP
// 반환 : 에러가 없다면 TRUE 있다면 FALSE;
// 주의 : 
//------------------------------------------------------------------------------
BOOL  GetHostMacAddressFromBootpPacket( BOOTP_Packet_t *rxPktBuf, char *HostMacAddress )
{
        // Client 맥 어드레스를 설정한다.
	memcpy( HostMacAddress , rxPktBuf->ETHERNET.et_src, sizeof( rxPktBuf->ETHERNET.et_src ) );
        return TRUE;
}

//------------------------------------------------------------------------------
// 설명 : 수신된 이더넷 패켓에서 송신 측 맥 어드레스를 얻어온다. 
// 매개 : ptrTFTP : 수신된 Packet 버퍼 주소 
// 반환 : 호스트의 포트 번호를 얻는다. 
// 주의 : 
//------------------------------------------------------------------------------
Word16 GetHostPortNumberFromTftpPacket( TFTP_Packet_t *ptrTFTP )
{
       return  SWAP16( ptrTFTP->UDP.udp_src );
}


//------------------------------------------------------------------------------
// 설명 : Ethernet Header를 IP 패켓 형식으로 만듬..
// 매개 : ptrEthernetIP : EthenrIP 버퍼 주소 
//        srcMAC   : 송신 측 MAC 어드레스 
//        destMAC  : 수신 측 MAC 어드레스  
// 반환 : 없음. 
// 주의 : 없음.
//------------------------------------------------------------------------------
void SetEthernetIPHeader( EthernetIP_t *ptrEthernetIP, char *srcMAC, char *destMAC )
{
	memcpy((char *) ptrEthernetIP->et_dest, destMAC, sizeof( ptrEthernetIP->et_dest ) );
	memcpy((char *) ptrEthernetIP->et_src , srcMAC , sizeof( ptrEthernetIP->et_src  ) );

	ptrEthernetIP->et_protocol = SWAP16( PROT_IP );	// set 0x0800 (ip).
}	

//------------------------------------------------------------------------------
// 설명 : Ethernet Header를 IP 패켓 형식으로 만듬..
// 매개 : ptrEthernetIP : EthenrIP 버퍼 주소 
//        srcMAC   : 송신 측 MAC 어드레스 
//        destMAC  : 수신 측 MAC 어드레스  
// 반환 : 없음. 
// 주의 : 없음.
//------------------------------------------------------------------------------
void SetEthernetARPHeader( EthernetIP_t *ptrEthernetIP, char *srcMAC, char *destMAC )
{
	memcpy((char *) ptrEthernetIP->et_dest, destMAC, sizeof( ptrEthernetIP->et_dest ) );
	memcpy((char *) ptrEthernetIP->et_src , srcMAC , sizeof( ptrEthernetIP->et_src  ) );

	ptrEthernetIP->et_protocol = SWAP16( PROT_ARP );	// set 0x0806 (ip).
}	
//------------------------------------------------------------------------------
// 설명 : ARP 요구 페켓을 만든다. 
// 매개 : 
// 반환 : 없음. 
// 주의 : 없음.
//------------------------------------------------------------------------------
void SetArpRequestHeader( ARP_Packet_t *ptrARP_Packet, char *srcMAC, Word32 srcIP, Word32 hostIP )
{

    ptrARP_Packet->ar_hrd = SWAP16( HWT_ETHER    );       // Format of hardware address	
    ptrARP_Packet->ar_pro = SWAP16( PROT_IP      );         // Format of protocol address	
    ptrARP_Packet->ar_hln =         6             ;               // Length of hardware address	
    ptrARP_Packet->ar_pln =         4             ;               // Length of protocol address	
    ptrARP_Packet->ar_op  = SWAP16( ARPOP_REQUEST);   // Operation
                                             
    memcpy( ptrARP_Packet->SenderMac, srcMAC, sizeof( ptrARP_Packet->SenderMac ) );  // Sender MAC Address
    memset( ptrARP_Packet->TargetMac,     0 , sizeof( ptrARP_Packet->TargetMac ) );  // Target MAC Address
    ptrARP_Packet->SenderIP  = srcIP;                                                // Sender IP  Address
    ptrARP_Packet->TargetIP = hostIP;                                                // Target IP  Address

}
//------------------------------------------------------------------------------
// 설명 : 수신된 ARP 패켓이 이상이 없는지를 확인하고 응답을 보내야 하는지를 
//        검사 한다. 
// 매개 : rxPktBuf : 수신된 Packet 버퍼 주소 
//        len      : 수신된 길이 
//        ClientIP : 할당된 IP 
// 반환 : 송신 초리가 요구 되념 TRUE 아니면 FALSE;
// 주의 : 
//------------------------------------------------------------------------------
BOOL CheckARPRxPacket( void *rxPktBuf, int len, Word32 *HostIP, Word32 *ClientIP )
{

	ARP_Packet_t *ptrARP = (ARP_Packet_t *)( rxPktBuf );

 
   	if( len < sizeof( ARP_Packet_t )              ) return FALSE;

	if( SWAP16( ptrARP->ar_op  ) != ARPOP_REPLY   ) return FALSE;
	if( SWAP16( ptrARP->ar_hrd ) != ARP_ETHER     ) return FALSE;
	if( SWAP16( ptrARP->ar_pro ) != PROT_IP       ) return FALSE;
	if(         ptrARP->ar_hln   != 6             ) return FALSE;
	if(         ptrARP->ar_pln   != 4             ) return FALSE;

    if( ptrARP->SenderIP != *HostIP   ) return FALSE;
    if( ptrARP->TargetIP != *ClientIP ) return FALSE;

    return TRUE;

}

//------------------------------------------------------------------------------
// 설명 : 수신된 ARP 패켓에서 송신 측 맥 어드레스를 얻어온다. 
// 매개 : rxPktBuf       : 수신된 Packet 버퍼 주소 
//        HostMacAddress : 할당된 로컬 IP 
//        HostIP   : 할당된 호스트 IP
// 반환 : 에러가 없다면 TRUE 있다면 FALSE;
// 주의 : 
//------------------------------------------------------------------------------
BOOL  GetHostMacAddressFromArpPacket( ARP_Packet_t *rxPktBuf, char *HostMacAddress )
{

	memcpy( HostMacAddress , rxPktBuf->SenderMac, sizeof( rxPktBuf->SenderMac ) );
    	return TRUE;

}

//------------------------------------------------------------------------------
// 역할 : IP Header  정보를 설정한다. ( UDP 형식)
// 매개 : ptrIP      : IP 정보 영역 버퍼 주소 
//        srcIP      : 송신 측 IP 주소 
//        destIP     : 수신 측 IP 주소 
//        PacketSize : 상위 packet의 길이.
// 반환 : 없음.
// 주의 : 없음.
//------------------------------------------------------------------------------
void SetIPHeaderByUDP( IP_t *ptrIP, Word32 destIP, Word32 srcIP, Word16 PacketSize )
{

	// 패켓 크기가 홀수라면 체크썸 계산을 위하여 마지막 위치에 0을 넣는다. 
	if ( PacketSize & 1) ptrIP->Data[PacketSize] = 0;

	// IP의 정보 영역을 설정한다. 
	ptrIP->ip_hl_v                   = 0x45;			// 현재 ip version 4, IP_HDR_SIZE / 4 (not including UDP)
	ptrIP->ip_tos                    = 0;				// type of service. 현재 network에서 지원안함. 항상 0.
	ptrIP->ip_len                    = SWAP16( sizeof( IP_t ) + PacketSize );	// total length of ip packet.
	ptrIP->ip_id                     = SWAP16( __IP_ID );		// identifier.
	ptrIP->ip_off                    = SWAP16( 0x0000 );		// No fragmentation.
	ptrIP->ip_ttl                    = 64;				// time to live.
	ptrIP->ip_p                      = IPPROTO_UDP;			// UDP.
	ptrIP->ip_sum                    = 0;

	memcpy( (char *)&(ptrIP->ip_src) , &(srcIP ), 4 );
	memcpy( (char *)&(ptrIP->ip_dst) , &(destIP), 4 );

	ptrIP->ip_sum = ~IPChecksum( (char *) ptrIP, sizeof( IP_t ) / 2);

        __IP_ID++;

}

//------------------------------------------------------------------------------
// 역할 : IP Header  정보를 설정한다. ( UDP 형식)
// 매개 : ptrIP      : IP 정보 영역 버퍼 주소 
//        srcIP      : 송신 측 IP 주소 
//        destIP     : 수신 측 IP 주소 
//        PacketSize : 상위 packet의 길이.
// 반환 : 없음.
// 주의 : 없음.
//------------------------------------------------------------------------------
void SetIPHeaderByICMP( IP_t *ptrIP, Word32 destIP, Word32 srcIP, Word16 PacketSize )
{

	// 패켓 크기가 홀수라면 체크썸 계산을 위하여 마지막 위치에 0을 넣는다. 
	if ( PacketSize & 1) ptrIP->Data[PacketSize] = 0;

	// IP의 정보 영역을 설정한다. 
	ptrIP->ip_hl_v                   = 0x45;			// 현재 ip version 4, IP_HDR_SIZE / 4 (not including UDP)
	ptrIP->ip_tos                    = 0;				// type of service. 현재 network에서 지원안함. 항상 0.
	ptrIP->ip_len                    = SWAP16( sizeof( IP_t ) + PacketSize );	// total length of ip packet.
	ptrIP->ip_id                     = SWAP16( __IP_ID );		// identifier.
	ptrIP->ip_off                    = SWAP16( 0x0000 );		// No fragmentation.
	ptrIP->ip_ttl                    = 64;			         	// time to live.
	ptrIP->ip_p                      = IPPROTO_ICMP;			// UDP.
	ptrIP->ip_sum                    = 0;

	memcpy( (char *)&(ptrIP->ip_src) , &(srcIP ), 4 );
	memcpy( (char *)&(ptrIP->ip_dst) , &(destIP), 4 );

	ptrIP->ip_sum = ~IPChecksum( (char *) ptrIP, sizeof( IP_t ) / 2);

    __IP_ID++;

}
//------------------------------------------------------------------------------
// 역할 : ICMP 체크썸을 구한다. 
// 매개 : ptrIP      : IP 정보 영역 버퍼 주소 
//        srcIP      : 송신 측 IP 주소 
//        destIP     : 수신 측 IP 주소 
//        PacketSize : 상위 packet의 길이.
// 반환 : 없음.
// 주의 : 없음.
//------------------------------------------------------------------------------
Word16 GetCheckSumICMP( Word16 *Data, int Size )
{
	Word16  *w      = Data;	// address of next 16-bit word 
	int      nleft  = Size;	// remaining 16-bit words 
	int      sum    = 0;	// 32-bit accumulator 
	Word16   answer = 0;

	while (nleft > 1)
	{
		sum += *w++;
		nleft -= 2;
	}

	if (nleft == 1)
	{
		*(Byte *)(&answer) = *(Byte *)w;
		sum += answer;
	}

    // Add back carry outs from top 16 bits to low 16 bits.
 
	sum = (sum >> 16) + (sum & 0xFFFF);	// add hi 16 to low 16 
	sum += (sum >> 16);			        // add carry 
	answer = ~sum;				        // truncate to 16 bits 
	return(answer);
}

//------------------------------------------------------------------------------
// 설명 : UDP Header를 정보를 설정한다. 
// 매개 : ptrUDP : UDP 정보 영역 버퍼 주소 
//        srcPort      : 송신 측 Port 주소 
//        destPort     : 수신 측 Port 주소 
//        PacketSize   : 상위 packet의 길이.
// 반환 : 없음.
// 주의 : 없음.
//------------------------------------------------------------------------------
void SetUdpHeader( UDP_t *ptrUDP, Word16 descPort, Word16 srcPort, Word16 PacketSize )
{
	ptrUDP->udp_dest    = SWAP16( descPort );
	ptrUDP->udp_src     = SWAP16( srcPort  );
	ptrUDP->udp_len     = SWAP16( sizeof( UDP_t )  + PacketSize );
	ptrUDP->udp_chksum  = 0;
}

//------------------------------------------------------------------------------
// 설명 : Bootp의 값을 설정한다. 
// 매개 : ptrBOOTP : BOOTP 정보 영역 버퍼 주소 
//        srcMAC   : 송신 측 MAC 어드레스 
// 반환 : 없음 
// 주의 : 
//------------------------------------------------------------------------------
void SetBootpHeader( BOOTP_t *ptrBOOTP , char *srcMAC )
{
        
	ptrBOOTP->bh_opcode = OP_BOOTREQUEST;		// 1 : request		2 : reply.
	ptrBOOTP->bh_htype  = HWT_ETHER;		// 10 Base Ethernet : 1.
	ptrBOOTP->bh_hlen   = HWL_ETHER;		// 10 Base Ethernet : 6.
	ptrBOOTP->bh_hops   = 0;			// client에서 0으로 setting. gateway가 사용.
	ptrBOOTP->bh_secs   = SWAP16( TimerGetTime() / TICKS_PER_SECOND ) ;

        // Client 맥 어드레스를 설정한다.
	memcpy( ptrBOOTP->bh_chaddr , srcMAC, sizeof( ptrBOOTP->bh_chaddr ) );

        // 패켓 고유 번호를 설정한다. 
        __BootpID++;  
	memcpy(&( ptrBOOTP->bh_tid ), &(__BootpID), 4);

}


//------------------------------------------------------------------------------
// 설명 : TFTP 패켓을 데이타 전송요구 패켓으로 만든다. 
// 매개 : ptrTFTP  : 패켓 버퍼 주소 
//        filename : 요구할 화일명 
// 반환 : int 패켓 크기 
// 주의 : 없음.
//------------------------------------------------------------------------------
int  SetTFTP_RRQPacket( TFTP_Packet_t *ptrTFTP , char *filename  )
{
        int PacketSize = 0;  
 
        // 자료 요구 명령 
        ptrTFTP->OPCODE = SWAP16(TFTP_RRQ);       
        PacketSize += 2;

        // 요구할 화일명을 복사한다.  
        strcpy( ptrTFTP->Data, filename );        
        PacketSize += ( strlen( filename ) + 1 );

        // 화일 모드를 복사한다. 
        strcpy( ptrTFTP->Data + PacketSize -2, TFTP_FILE_MODE );  
        PacketSize += ( strlen( TFTP_FILE_MODE ) + 1 );


// 시간 초과 처리 추가한 내용임 
        
        // 타임아웃 값을 설정한다. 
        strcpy( ptrTFTP->Data + PacketSize -2, "timeout" );  
        PacketSize += ( strlen( "timeout" ) + 1 );

        strcpy( ptrTFTP->Data + PacketSize -2, "120" );  
        PacketSize += ( strlen( "120" ) + 1 );
        
#if 0	// 호환성 문제로 추가하지 않는다.
        // 블럭크기를 설정
        strcpy( ptrTFTP->Data + PacketSize -2, "blksize" );  
        PacketSize += ( strlen( "blksize" ) + 1 );

        strcpy( ptrTFTP->Data + PacketSize -2, "1024" );  
        PacketSize += ( strlen( "1024" ) + 1 );
#endif
        // 크기를 요구한다. 
        strcpy( ptrTFTP->Data + PacketSize -2, "tsize" );  
        PacketSize += ( strlen( "tsize" ) + 1 );

        strcpy( ptrTFTP->Data + PacketSize -2, "0" );  
        PacketSize += ( strlen( "0" ) + 1 );

// 추가한 내용끝         

	return PacketSize;
}

//------------------------------------------------------------------------------
// 설명 : TFTP 패켓을 데이타 수신 정상 응답 패켓으로 만든다. 
// 매개 : ptrTFTP     : 패켓 버퍼 주소 
//        BlockNumber : 응답 블럭 번호 
// 반환 : int 패켓 크기 
// 주의 : 없음.
//------------------------------------------------------------------------------
int  SetTFTP_ACKDATAPacket( TFTP_Packet_t *ptrTFTP , Word16 BlockNumber  )
{
        int      PacketSize = 0;  
        Word16  *ptrBlockNumber;

        ptrBlockNumber =  (Word16  *) ptrTFTP->Data;
 
        // 자료 요구 명령 
        ptrTFTP->OPCODE = SWAP16(TFTP_ACK);       
        PacketSize += 2;

        // 블럭 넘버를 설정한다. 
        *ptrBlockNumber = SWAP16( BlockNumber );
        PacketSize += 2; 

	return PacketSize;
}

//------------------------------------------------------------------------------
// 설명 : TFTP 패켓을 에러 응답 패켓으로 만든다. 
// 매개 : ptrTFTP  : 패켓 버퍼 주소 
//        errornum : 요구할 화일명 
// 반환 : int 패켓 크기 
// 주의 : 없음.
//------------------------------------------------------------------------------
int  SetTFTP_ErrorPacket( TFTP_Packet_t *ptrTFTP , int errornum )
{
        int      PacketSize = 0;  
        Word16  *ptrErrorNumber;
        char    *ptrErrStr;

        ptrErrorNumber =  (Word16  *) ptrTFTP->Data;
 
        // 자료 요구 명령 
        ptrTFTP->OPCODE = SWAP16(TFTP_ERROR);       
        PacketSize += 2;

        // 블럭 넘버를 설정한다. 
        *ptrErrorNumber = SWAP16(errornum);
        PacketSize += 2; 

        // 에러 스트링을 넣는다. 
        ptrErrStr = NULL;

        switch( errornum )
        {
        case 2 : ptrErrStr = "File has bad magic";  break;
        case 3 : ptrErrStr = "File too large" ;      break;
        }  
        
        if( ptrErrStr != NULL )
        {
           strcpy( ptrTFTP->Data + 2, ptrErrStr );
           PacketSize += strlen( ptrErrStr ) + 1; 
        } 

	return PacketSize;
}


