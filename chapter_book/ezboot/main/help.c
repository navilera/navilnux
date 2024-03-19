//------------------------------------------------------------------------------
// 화일명 : help.c
// 설  명 : ezBoot의 도움말 파일이다.
// 
// 작성자 : 유영창 (주)제이닷디엔티 frog@falinux.com
//          오재경 (주)제이닷디엔티 freefrug@falinux.com  -- 푸지
// 작성일 : 2003년 6월 7일
// 수  정 : 
// 저작권 : (주)제이닷디엔티 
// 주  의 : 
//------------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>

//------------------------------------------------------------------------------
// 설명 : 명령 리스트를 보여 준다. 
// 매계 : 없음
// 반환 : 없음
// 주의 : 없음
//------------------------------------------------------------------------------
void 	HelpList( void )
{
	printf( 
	
        "\n"
        "  Memory dump        : MD, MWB, MWW, MWL\n"
        "  Download by ZModem : ZFB, ZFK, ZFR, ZMK, ZMR, ZF, ZM\n"
        "  Download by TFTP   : TFB, TFK, TFR, TMK, TMR, TF, TM\n"
	"  Copy               : MFK, MFR, FMK, FMR\n"
	"  Erase Partition    : NEK, NER, NEA\n"
	"  Network            : ARP, PING\n"
	"  System             : RST, GK, GO\n"
	"  Confiruation       : SET\n"
	"\n"
	"  More Help          : HELP [cmd]\n"

	);
}
//------------------------------------------------------------------------------
// 설명 : 도움말 
// 주의 : 
//------------------------------------------------------------------------------
const char *HELP_TABLE[33] = 
{
	"RST   * Software Reset\n"                           ,
	"GK    * Jump Kernel\n"
	"      * Has been copyed the Kernel to SDRAM\n"      ,
	"GO [ADDR]  * Jump Address\n"
	"           * Has been copyed the ADDR to SDRAM\n"
	"           * ex) GO 0xA1200000\n"                   ,
	"SET   * Set the EZBOOT Configuration value\n"
	"      * must be set for Networking\n"               ,

	"MD [ADDR] [COUNT]\n"
	"      * Memory or Register Dump\n"
	"      * ex) MD 0xA0F00000 32\n"                     ,
	"MWB [ADDR] [db] * Write to Memory, Byte-Size\n"
	"                * ex) MWB 0xA1200000 0x12\n"        ,
	"MWW [ADDR] [dw] * Write to Memory, Word-Size\n"
	"                * ex) MWW 0xA1200000 0x1234\n"      ,
	"MWL [ADDR] [dl] * Write to Memory or Register, DWord-Size\n"
	"                * ex) MWB 0xA1200000 0x12345678\n"  ,

	"ZFB   * Download to FLASH on [fixed Boot-Loader Address] -- ZModem\n",
	"ZFK   * Download to FLASH on [fixed Kernel Address] -- ZModem\n"   ,
	"ZFR   * Download to FLASH on [fixed Ramdisk Address] -- ZModem\n"  ,
	"ZMK   * Download to SDRAM on [fixed Kernel Address] -- ZModem\n"   ,
	"ZMR   * Download to SDRAM on [fixed Ramdisk Address] -- ZModem\n"  ,
	"ZF [FLASH-ADDR]  * Download on [FLASH-ADDR] -- ZModem\n",
	"ZM [SDRAM-ADDR]  * Download on [SDRAM-ADDR] -- ZModem\n",
	
	"TFB   * Download to [fixed Boot-Loader FLASH-Address] -- TFTP\n"
	"      * must be set Local-IP, Host-IP, ezboot-name\n"
	"      * and active TFTP server on Linux-Host\n"     ,
	"TFK   * Download to [fixed Kernel FLASH-Address] -- TFTP\n" 
	"      * must be set Local-IP, Host-IP, Kernel-name\n"
	"      * and active TFTP server on Linux-Host\n"     ,
	"TFR   * Download to [fixed Ramdisk FLASH-Address] -- TFTP\n"
	"      * must be set Local-IP, Host-IP, Ramdisk-name\n"
	"      * and active TFTP server on Linux-Host\n"     ,
	"TMK   * Download to [fixed Kernel SDRAM-Address] -- TFTP\n" 
	"      * must be set Local-IP, Host-IP, Kernel-name\n"
	"      * and active TFTP server on Linux-Host\n"     ,
	"TMR   * Download to [fixed Ramdisk SDRAM-Address] -- TFTP\n"
	"      * must be set Local-IP, Host-IP, Ramdisk-name\n"
	"      * and active TFTP server on Linux-Host\n"     ,
	"TF [FLASH-ADDR] [File]\n"
	"      * Download to [FLASH-ADDR] to [File]  -- TFTP\n"
	"      * must be set Local-IP, Host-IP, Ramdisk-name\n"
	"      * and active TFTP server on Linux-Host\n"     
	"      * ex) TF 0x40000, test.img\n"                  ,
	"TM [SDRAM-ADDR] [File]\n"
	"      * Download to [SDRAM-ADDR] to [File]  -- TFTP\n"
	"      * must be set Local-IP, Host-IP, Ramdisk-name\n"
	"      * and active TFTP server on Linux-Host\n"      
	"      * ex) TM 0xA1200000, test.img\n"               ,
	
	"MFK   * Copy from SDRAM to FLASH by Kernel-Address\n"
	"      * and then type command GK\n"                  ,
	"MFR   * Copy from SDRAM to FLASH by Ramdisk-Address\n"
	"      * and then type command MFK, GK\n"             ,      
	"MF [SDRAM-ADDR] [FLASH-ADDR] [COUNT]\n"
	"      * Copy from [SDRAM-ADDR] to [FLASH-ADDR]\n"
	"      * ex) MF 0xA1200000 0x40000 0x100\n"           ,
	"FMK   * Copy from FLASH to SDRAM by Kernel-Address\n"
	"      * used when save to FLASH\n"                   ,
	"FMR   * Copy from FLASH to SDRAM by Ramdisk-Address\n"
	"      * used when save to FLASH\n"                   ,
	"FM [FLASH-ADDR] [SDRAM-ADDR] [COUNT]\n"
	"      * Copy from [FLASH-ADDR] to [SDRAM-ADDR]\n"
	"      * ex) FM 0x40000 0xA1200000 0x100\n"           ,

	"NEK   * Erase NAND-FLASH Kernel partition\n"         ,
	"NER   * Erase NAND-FLASH Ramdisk partition\n"        ,
	"NEA   * Erase NAND-FLASH Application partition\n"    ,
	
	"ARP   * Get Host Mac Address from Host\n"            ,
	"PING [IP] * Send Echo Protocol to [IP]\n"
	"          * PING 192.168.10.20\n"                    
};

//------------------------------------------------------------------------------
// 설명 : 명령의 자세한 사용법을 보여 준다. 
// 주의 : 
//------------------------------------------------------------------------------
void 	HelpItem( char *helpcmd )
{
	int	ItemIdex, len;

	// 대분자로 만든다.
	len = 0;
	while( 0 != helpcmd[len] )
	{
		if ( 0x60 < helpcmd[len] ) helpcmd[len] -= 0x20;
		len ++;
	}

	
	if      ( 0 == strcmp( "RST", helpcmd ) ) ItemIdex = 0;
	else if ( 0 == strcmp( "GK",  helpcmd ) ) ItemIdex = 1;
	else if ( 0 == strcmp( "GO",  helpcmd ) ) ItemIdex = 2;
	else if ( 0 == strcmp( "SET", helpcmd ) ) ItemIdex = 3;
	else if ( 0 == strcmp( "MD" , helpcmd ) ) ItemIdex = 4;
	else if ( 0 == strcmp( "MWB", helpcmd ) ) ItemIdex = 5;
	else if ( 0 == strcmp( "MWW", helpcmd ) ) ItemIdex = 6;
	else if ( 0 == strcmp( "MWL", helpcmd ) ) ItemIdex = 7;
	else if ( 0 == strcmp( "ZFB", helpcmd ) ) ItemIdex = 8;
	else if ( 0 == strcmp( "ZFK", helpcmd ) ) ItemIdex = 9;
	else if ( 0 == strcmp( "ZFR", helpcmd ) ) ItemIdex = 10;
	else if ( 0 == strcmp( "ZMK", helpcmd ) ) ItemIdex = 11;
	else if ( 0 == strcmp( "ZMR", helpcmd ) ) ItemIdex = 12;
	else if ( 0 == strcmp( "ZF",  helpcmd ) ) ItemIdex = 13;
	else if ( 0 == strcmp( "ZM",  helpcmd ) ) ItemIdex = 14;
	else if ( 0 == strcmp( "TFB", helpcmd ) ) ItemIdex = 15;
	else if ( 0 == strcmp( "TFK", helpcmd ) ) ItemIdex = 16;
	else if ( 0 == strcmp( "TFR", helpcmd ) ) ItemIdex = 17;
	else if ( 0 == strcmp( "TMK", helpcmd ) ) ItemIdex = 18;
	else if ( 0 == strcmp( "TMR", helpcmd ) ) ItemIdex = 19;
	else if ( 0 == strcmp( "TF",  helpcmd ) ) ItemIdex = 20;
	else if ( 0 == strcmp( "TM",  helpcmd ) ) ItemIdex = 21;
	else if ( 0 == strcmp( "MFK", helpcmd ) ) ItemIdex = 22;
	else if ( 0 == strcmp( "MFR", helpcmd ) ) ItemIdex = 23;
	else if ( 0 == strcmp( "MF",  helpcmd ) ) ItemIdex = 24;
	else if ( 0 == strcmp( "FMK", helpcmd ) ) ItemIdex = 25;
	else if ( 0 == strcmp( "FMR", helpcmd ) ) ItemIdex = 26;
	else if ( 0 == strcmp( "FM ", helpcmd ) ) ItemIdex = 27;
	else if ( 0 == strcmp( "NEK", helpcmd ) ) ItemIdex = 28;
	else if ( 0 == strcmp( "NER", helpcmd ) ) ItemIdex = 29;
	else if ( 0 == strcmp( "NEA", helpcmd ) ) ItemIdex = 30;
	else if ( 0 == strcmp( "ARP", helpcmd ) ) ItemIdex = 31;
	else if ( 0 == strcmp( "PING",helpcmd ) ) ItemIdex = 32;
	else return;

	printf( "\n" );
	printf( HELP_TABLE[ItemIdex] );

}

//------------------------------------------------------------------------------
// 설명 : 명령 리스트를 보여 준다. 
// 매계 : argc    : 토큰 갯수 
//        argv    : 분리된 토큰 문자열 주소가 담겨질 배열 
// 반환 : 에러 -1  정상 0
// 주의 : 없음 
//------------------------------------------------------------------------------
int 	Help(int argc, char **argv)
{
	if( argc == 1 ) HelpList();
  	else            HelpItem( argv[1] );

  	return 0;
}






      


