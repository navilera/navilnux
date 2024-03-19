//------------------------------------------------------------------------------
// 화일명 : NET.h
// 설  명 : ezBoot의 Network의 정보 처리 루틴이다. 
// 
// 작성자 : 유영창 (주)제이닷디엔티 frog@falinux.com
// 작성일 : 2001년 10월 8일
// 저작권 : (주)제이닷디엔티 
//          
// 주  의 : 이 함수의 대부분은 T&B에서 제작된 부트로더에서 가져 왔으면
//          최초 원 쏘스는 PPC 부트이다. 
//------------------------------------------------------------------------------

#ifndef _NET_HEADER_
#define _NET_HEADER_


typedef Word32		IPaddr_t;


//------------------------------------------------------------------------------
// 프로토콜 헤더 
//------------------------------------------------------------------------------

//
//	Ethernet header
//
typedef struct 
{
	Byte		et_dest[6];	// Destination node		
	Byte		et_src[6];	// Source node			
	Word16		et_protlen;	// Protocol or length		
	Byte		et_dsap;	// 802 DSAP			
	Byte		et_ssap;	// 802 SSAP			
	Byte		et_ctl;		// 802 control			
	Byte		et_snap1;	// SNAP				
	Byte		et_snap2;
	Byte		et_snap3;
	Word16		et_prot;	// 802 protocol			
        char            Data[0];        // 동적 데이타 정의 
} __attribute__ ((packed)) Ethernet_t;


#define ETHER_HDR_SIZE	14		// Ethernet header size		
#define E802_HDR_SIZE	22		// 802 ethernet header size	
#define PROT_IP		0x0800		// IP protocol			
#define PROT_ARP	0x0806		// IP ARP protocol		
#define PROT_RARP	0x8035		// IP ARP protocol		

typedef struct 
{
	Byte		et_dest[6];	    // Destination node		
	Byte		et_src[6];	    // Source node			
	Word16		et_protocol;	// Protocol
    char        Data[0];        // 동적 데이타 정의 
} __attribute__ ((packed)) EthernetIP_t;

#define IPPROTO_ICMP	 1		// Internet Control Message Protocol	
#define IPPROTO_UDP	    17		// User Datagram Protocol		


//
//	Address Resolution Protocol (ARP) header.
//
typedef struct
{
    EthernetIP_t ETHERNET;
	Word16		ar_hrd;		        // Format of hardware address	
	Word16		ar_pro;		        // Format of protocol address	
	Byte		ar_hln;		        // Length of hardware address	
	Byte		ar_pln;		        // Length of protocol address	
	Word16		ar_op;		        // Operation			
    char        SenderMac[6];       // Sender MAC Address
    Word32      SenderIP    ;       // Sender MAC Address
    char        TargetMac[6];       // Target MAC Address
    Word32      TargetIP    ;       // Target MAC Address
} __attribute__ ((packed)) ARP_Packet_t;

#define ARP_ETHER	        1		// Ethernet  hardware address	
#define ARPOP_REQUEST       1		// Request  to resolve  address	
#define ARPOP_REPLY	        2		// Response to previous request	

#define RARPOP_REQUEST      3		// Request  to resolve  address	
#define RARPOP_REPLY	    4		// Response to previous request 

#define HWT_ETHER	 	    1

//
//	Internet Protocol (IP) header.
//
typedef struct 
{
	Byte		ip_hl_v;	// header length and version	
	Byte		ip_tos;		// type of service		
	Word16		ip_len;		// total length			
	Word16		ip_id;		// identification		
	Word16		ip_off;		// fragment offset field	
	Byte		ip_ttl;		// time to live			
	Byte		ip_p;		// protocol
	Word16		ip_sum;		// checksum			
	IPaddr_t	ip_src;		// Source IP address		
	IPaddr_t	ip_dst;		// Destination IP address	
    char        Data[0];    // 동적 데이타 정의
} __attribute__ ((packed)) IP_t;

#define IP_HDR_SIZE_NO_UDP	(sizeof (IP_t) - 8)
#define IP_HDR_SIZE		(sizeof (IP_t))


//
//      icmp header.
//
typedef struct 
{
	Byte	     icmp_type;	
	Word16	     icmp_code;	
	Word16	     icmp_cksum;	
	Word16	     icmp_id;	
    char         icmp_seq;        // 동적 데이타 정의 
    char         icmp_data[32]; 
} __attribute__ ((packed)) ICMP_t;

#define ECHO_REQUEST       8
#define ECHO_REPLY         0

//
//      icmp header.
//
typedef struct 
{
    EthernetIP_t ETHERNET; 
    IP_t         IP;
    ICMP_t       ICMP;
} __attribute__ ((packed)) ECHO_Packet_t;


//
//      udp header.
//
typedef struct 
{
	Word16	udp_src;	// udp source port.
	Word16	udp_dest;	// udp destination port.
	Word16	udp_len;	// length of udp packet.
	Word16	udp_chksum;	// checksum.
     char            Data[0];        // 동적 데이타 정의 
} __attribute__ ((packed)) UDP_t;

#define UDP_HDR_SIZE	8



//
// BOOTP header.
//
typedef struct 
{
	Byte		bh_opcode;	// Operation.
	Byte		bh_htype;	// Hardware type.
	Byte		bh_hlen;	// Hardware address length.
	Byte		bh_hops;	// Hop count (gateway thing).
	Word32		bh_tid;		// Transaction ID.
	Word16		bh_secs;	// Seconds since boot.
	Word16		bh_res1;	// Reserved(alignment).
	Word32		bh_ciaddr;	// Client IP address.
	Word32		bh_yiaddr;	// Your (client) IP address.
	Word32		bh_siaddr;	// Server IP address(Server가 setting).
	Word32		bh_giaddr;	// Gateway IP address.
	Byte		bh_chaddr[16];	// Client hardware address(Mac Address).
	char		bh_sname[64];	// Server host name.
	char		bh_file[128];	// Boot file name.
	char		bh_vend[64];	// Vendor information.
} __attribute__ ((packed)) BOOTP_t;

#define BOOTP_HDR_SIZE		300
#define BOOTP_SIZE		(ETHER_HDR_SIZE + IP_HDR_SIZE + UDP_HDR_SIZE + BOOTP_HDR_SIZE)
#define OP_BOOTREQUEST		1
#define OP_BOOTREPLY	 	2
#define HWL_ETHER		0x06



#define PORT_BOOTPS		67		// BOOTP server UDP port.
#define PORT_BOOTPC		68		// BOOTP client UDP port.

//
// BOOTP Packet
//
typedef struct 
{
   EthernetIP_t ETHERNET; 
   IP_t         IP;
   UDP_t        UDP;
   BOOTP_t      BOOTP;
} __attribute__ ((packed)) BOOTP_Packet_t;



#define TFTP_PORT_HOST		69		// Well known TFTP port #
#define TFTP_PORT_CLIENT        6666         
#define TFTP_TIMEOUT		5		// Seconds to tftpTimeout for a lost pkt
#define TIMEOUT_COUNT		10		// # of tftpTimeouts before giving up

// TFTP operations.
#define TFTP_RRQ		1
#define TFTP_WRQ		2
#define TFTP_DATA		3
#define TFTP_ACK		4
#define TFTP_ERROR		5


#define TFTP_NONE		0
#define TFTP_STATE_DATA_OK	1
#define TFTP_STATE_DATA_FAILE	2
#define TFTP_STATE_FAILURE	3
#define TFTP_STATE_END          4
#define TFTP_ERROR		5
#define TFTP_OACK		6


#define TFTP_FILE_MODE          "octet"


//
// TFTP Packet
//
typedef struct 
{
   EthernetIP_t ETHERNET; 
   IP_t         IP;
   UDP_t        UDP;
   Word16       OPCODE;
   char         Data[0];        // 동적 데이타 정의   
} __attribute__ ((packed)) TFTP_Packet_t;


extern void SetEthernetIPHeader ( EthernetIP_t *ptrEthernetIP, char *srcMAC, char *destMAC );
extern void SetEthernetARPHeader( EthernetIP_t *ptrEthernetIP, char *srcMAC, char *destMAC );
extern void SetIPHeaderByUDP    ( IP_t *ptrIP, Word32 destIP, Word32 srcIP, Word16 PacketSize );
extern void SetIPHeaderByICMP   ( IP_t *ptrIP, Word32 destIP, Word32 srcIP, Word16 PacketSize );
extern void SetArpRequestHeader ( ARP_Packet_t *ptrARP_Packet, char *srcMAC, Word32 srcIP, Word32 hostIP );
extern void SetUdpHeader        ( UDP_t *ptrUDP, Word16 descPort, Word16 srcPort, Word16 PacketSize );
extern void SetBootpHeader      ( BOOTP_t *ptrBOOTP , char *srcMAC );
extern int  SetSendTFTPPacket   ( TFTP_Packet_t *ptrTFTP , int type ); 

extern Word16 GetProtocolFromRxPacket( EthernetIP_t *rxPktBuf, int len );
extern BOOL   CheckARPRxPacket       ( void *rxPktBuf, int len, Word32 *HostIP, Word32 *ClientIP );
extern BOOL   CheckBootpRxPacket     ( BOOTP_Packet_t *rxPktBuf, int len, Word32 *ClientIP );
extern BOOL   CheckECHORxPacket      ( ECHO_Packet_t *rxPktBuf, int len, Word32 *ClientIP );
extern BOOL   CheckTftpRxPacket      ( TFTP_Packet_t  *rxPktBuf, int len, Word32 *ClientIP );
extern BOOL   GetIPFromBootpPacket   ( BOOTP_Packet_t *rxPktBuf, Word32 *LocalIP, Word32 *HostIP );

extern BOOL  GetHostMacAddressFromArpPacket( ARP_Packet_t *rxPktBuf, char *HostMacAddress );
extern BOOL  GetHostMacAddressFromBootpPacket( BOOTP_Packet_t *rxPktBuf, char *HostMacAddress );

extern Word16 GetCheckSumICMP( Word16 *Data, int Size );

extern char  *GetHostMacAddress ( void );
extern Word32 GetLocalIP        ( void );
extern Word32 GetHostIP         ( void );
extern Word16 GetHostPortNumberFromTftpPacket( TFTP_Packet_t *ptrTFTP );

extern BOOL   BootpProcess      ( void );

extern BOOL AutoRebuildNetworkInfo( void );

extern int SetTFTP_RRQPacket    ( TFTP_Packet_t *ptrTFTP , char *filename      ); 
extern int SetTFTP_ACKDATAPacket( TFTP_Packet_t *ptrTFTP , Word16 BlockNumber  );
extern int SetTFTP_ErrorPacket  ( TFTP_Packet_t *ptrTFTP , int errornum        ); 

#endif		// _NET_HEADER_
