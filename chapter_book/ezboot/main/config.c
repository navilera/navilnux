//------------------------------------------------------------------------------
// 화일명 : cfg.c
// 설  명 : ezBoot의 환경과 환경변수를 정의한다.
// 
// 작성자 : 유영창 (주)제이닷디엔티 frog@falinux.com
//          오재경 (주)제이닷디엔티 freefrug@falinux.com  -- 푸지
// 작성일 : 2002년 1월 19일
// 수  정 : 2003-06-07	
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
#include <flash.h>
#include <mem_map.h>


#define CONFIG_VAR   1
#include <config.h>

//******************************************************************************
// 광역 변수 정의
//******************************************************************************

TConfig Cfg;

TConfig Default_Cfg =
{
	MagicNumber         : CONFIG_MAGIC,              	// 환경 데이타가 있음의 검증에 대한 매직 번호 0x89880003
	            
	AutoBootWaitTime    : DEFAULT_BOOT_WAIT,         	// 자동 부트 대기 시간 ( 초 단위 )
	BootMenuKey         : DEFAULT_ENTRY_KEY,         	// 부트 로더 진입 키 기본 ' '
	UseRamdisk	    	: 'Y',			        		// 램디스 복사유무
	Watchdog	    	: 0,							// 위치독
	Rev1                : 0,           					// 예약 영역     

	Local_MacAddress    : DEFAULT_LOCAL_MAC,       		// 보드 MAC Address  [xx:xx:xx:xx:xx:xx]    
	Local_IP            : DEFAULT_LOCAL_IP,				// local IP   xxx.xxx.xxx.xxx
	Host_MacAddress     : {0,0,0,0,0,0},       			// 호스트 MAC Address  [xx:xx:xx:xx:xx:xx]
	Host_IP             : DEFAULT_HOST_IP,              // 호스트 IP  = xxx.xxx.xxx.xxx
						   
	TFTP_Directory      : DEFAULT_TFTP_DIR,            	// TFTP 디렉토리 명   
	TFTP_zImageFileName : DEFAULT_KERNEL_FILENAME,     	// 커널 이미지      화일 명 디폴트
	TFTP_RamDiskFileName: DEFAULT_RAMDISK_FILENAME,     // 램 디스크 이미지 화일 명 디폴트
	TFTP_LoaderFileName : DEFAULT_EZBOOT_FILENAME,      // 부트로더         화일 명 디폴트
	
	SeriaNumber         : DEFAULT_SERIAL,               // 사용할 시리얼 번호  
	Kernel_ArchNumber   : LINUX_MACH_TYPE,				// 커널에 전달될 커널 아키텍쳐 번호
	
														// nand 파티션 크기
	NandPar		    : {DEFAULT_NAND_PAR_KERNEL_SZ,DEFAULT_NAND_PAR_RAMDISK_SZ,DEFAULT_NAND_PAR_APP_SZ,0},
	
	KCmd : {
	         CMD_MagicNumber : CONFIG_CMD_MAGIC,        // 커널 커맨드 매직번호 
	         CMD_Tag         : "CMD=",                  // 커널 명령 라인 디폴트 
	         cmd_A		 : DEFAULT_KERNEL_COMMAND_A,
	         cmd_B		 : DEFAULT_KERNEL_COMMAND_B,
	         cmd_C		 : DEFAULT_KERNEL_COMMAND_C,
	       },
};


//------------------------------------------------------------------------------
// 설명 : 문자열을 정수형으로 변경한다.
// 매계 : 문자열, 핵사유무
// 반환 :
// 주의 : 
//------------------------------------------------------------------------------
Byte    StrToByte( char *ptr, int hex )
{
        Byte    rtn = 0;
        
        while (1)
        {
                if (*ptr == '\0') break;
                
                if (hex) rtn *= 16;
                else     rtn *= 10; 
                switch (*ptr)
                {
                        case '0' ... '9' : rtn += (Byte)(*ptr)&0x0f;       break;
                        case 'A' ... 'F' : 
                        case 'a' ... 'f' : rtn += ((Byte)(*ptr)&0x0f) + 9; break;
                }
                ptr++;
        }
        return rtn;
}
//------------------------------------------------------------------------------
// 설명 : 문자열을 분리한다.
// 주의 : 
//------------------------------------------------------------------------------
char *delim_ = " .:\n";   //\f\n\r\t\v
int Cfg_parse_args(char *cmdline, char **argv)
{
	char *tok;
	int argc = 0;

	argv[argc] = NULL;
                   
	for (tok = strtok(cmdline, delim_); tok; tok = strtok(NULL, delim_)) 
        {
		argv[argc++] = tok;
	}

	return argc;
}
//------------------------------------------------------------------------------
// 설명 : 사용자로부터 맥주소를 받는다.
// 주의 : 
//------------------------------------------------------------------------------
int     GetMacAddress( const char *msg, Byte *mac, int auto_gen )
{
    int     argc, idx;
    char    rbuff[128];
	char    *argv[128];

	if (auto_gen)
	{
        	mac[3] = (Cfg.Local_IP>>24) & 0xff;
        	mac[4] = (OSCR        >> 8) & 0xff;
        	mac[5] = (OSCR            ) & 0xff;
	}
    else
    {
        	printf( "\n" );
        	printf( msg, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5] );
        	printf( "     Newvalue : " );

        	gets( rbuff );
	
	        argc = Cfg_parse_args( rbuff, argv );        
	        if ( argc == 6 )
	        {
	                for (idx=0; idx<6; idx++)
	                {
	                        mac[idx] = StrToByte( argv[idx], 1 );
	                }        
	        }        
    }
        
    return 0;
}
//------------------------------------------------------------------------------
// 설명 : 사용자로부터 IP를 받는다.
// 주의 : 
//------------------------------------------------------------------------------
int     GetIPAddress( const char *msg, Word32 *ip )
{
        int     argc;
        char    rbuff[128];
        char    *argv[128];
        
        printf( "\n" );
        printf( msg, (*ip>> 0)&0xff, (*ip>> 8)&0xff, (*ip>>16)&0xff, (*ip>>24)&0xff );
        printf( "     Newvalue : " );
        
        gets( rbuff );
        
        argc = Cfg_parse_args( rbuff, argv );        
        if ( argc == 4 )
        {
                *ip =  ( StrToByte(argv[3],0) << 24 )
                      |( StrToByte(argv[2],0) << 16 )
                      |( StrToByte(argv[1],0) << 8  )
                      |( StrToByte(argv[0],0)       );
        }        
        return 0;
}
//------------------------------------------------------------------------------
// 설명 : 사용자로부터 문자열을 받는다.
// 주의 : 
//------------------------------------------------------------------------------
int     GetString( const char *msg, char *ptr )
{
        char    rbuff[1024];
        
        printf( "\n" );
        printf( msg, ptr );
        printf( "     Newvalue : " );
        
        gets( rbuff );

        strcpy( ptr, rbuff );
        return 0;
}        
//------------------------------------------------------------------------------
// 설명 : 사용자로부터 숫자 입력을 받는다.
// 주의 : 
//------------------------------------------------------------------------------
int     GetWord16_arg3( const char *msg, Word16 *ary  )
{
        int     idx, argc;
        int	cnt = 3;
        char    rbuff[128];
        char    *argv[128];
        
        printf( "\n" );
        printf( msg, ary[0],ary[1],ary[2] );
        printf( "     Newvalue : " );
        
        gets( rbuff );

        argc = Cfg_parse_args( rbuff, argv );        
        if ( argc == cnt )
        {
                for (idx=0; idx<cnt; idx++)
                	ary[idx] = strtoul( argv[idx], NULL, 0 );
        }        
        
        return 0;
}        
//------------------------------------------------------------------------------
// 설명 : 사용자로부터 한문자를 받는다.
// 주의 : 
//------------------------------------------------------------------------------
int     GetChar( const char *msg, char *cc )
{
        int     argc;
        char    rbuff[128];
        char    *argv[128];
        
        printf( "\n" );
        printf( msg, *cc );
        printf( "     Newvalue : " );
        
        gets( rbuff );

        argc = Cfg_parse_args( rbuff, argv );        
        if ( argc == 1 )
        {
                *cc = argv[0][0];
        }        
        return 0;
}        
//------------------------------------------------------------------------------
// 설명 : 사용자로부터 숫자를 받는다.
// 주의 : 
//------------------------------------------------------------------------------
int     GetWord32( const char *msg, Word32 *num )
{
        int     argc;
        char    rbuff[128];
        char    *argv[128];
        
        printf( "\n" );
        printf( msg, *num );
        printf( "     Newvalue : " );
        
        gets( rbuff );

        argc = Cfg_parse_args( rbuff, argv );        
        if ( argc == 1 )
        {
                *num = strtoul( argv[0], NULL, 10 );
        }        
        return 0;
}        
//------------------------------------------------------------------------------
// 설명 : 사용자로부터 숫자를 받는다.
// 주의 : 
//------------------------------------------------------------------------------
int     GetByte( const char *msg, Byte *num )
{
        int     argc;
        char    rbuff[128];
        char    *argv[128];
        
        printf( "\n" );
        printf( msg, *num );
        printf( "     Newvalue : " );
        
        gets( rbuff );

        argc = Cfg_parse_args( rbuff, argv );        
        if ( argc == 1 )
        {
                *num = strtoul( argv[0], NULL, 10 );
        }        
        return 0;
}        

//------------------------------------------------------------------------------
// 설명 : 환경설정 메뉴 문자열
// 주의 : 
//------------------------------------------------------------------------------
const char CFG_MENU_1_STR[] = "  1. MAC Address            [%02x:%02x:%02x:%02x:%02x:%02x]\n";
const char CFG_MENU_2_STR[] = "  2. LOCAL IP               [%d.%d.%d.%d]\n"                  ;
const char CFG_MENU_3_STR[] = "  3. HOST IP                [%d.%d.%d.%d]\n"                  ;
const char CFG_MENU_4_STR[] = "  4. Host tftp directory    [%s]\n";
const char CFG_MENU_5_STR[] = "  5. zImage      file name  [%s]\n";
const char CFG_MENU_6_STR[] = "  6. ram disk    file name  [%s]\n";
const char CFG_MENU_7_STR[] = "  7. boot loader file name  [%s]\n";

const char CFG_MENU_A_STR[] = "  A. Autoboot wait time     [%d]\n";
const char CFG_MENU_B_STR[] = "  B. Boot Menu Key          [%c]\n";
const char CFG_MENU_C_STR[] = "  C. Copy Ramdisk [Y/N]     [%c]\n";
const char CFG_MENU_D_STR[] = "  D. Architecture Number    [%d]\n";
const char CFG_MENU_E_STR[] = "  E. Serial FF/BT/ST(0/1/2) [%d]\n"; 
const char CFG_MENU_F_STR[] = "  F. NAND-Partition MByte   [%d:%d:%d] (kernel:ramdisk:app)\n";
const char CFG_MENU_W_STR[] = "  W. Watchdog (sec, off=0)  [%d]\n";

const char CFG_MENU_K_STR[] = "  K. Kernel CMD 1st [%s]\n";
const char CFG_MENU_M_STR[] = "  M. Kernel CMD 2nd [%s]\n";
const char CFG_MENU_N_STR[] = "  N. Kernel CMD 3rd [%s]\n";
                                          
const char CFG_MENU_L_STR[] = "  L. Load Default\n";
const char CFG_MENU_S_STR[] = "  S. Save\n"        ;
const char CFG_MENU_P_STR[] = "  P. Apply & Exit\n";
const char CFG_MENU_0_STR[] = "  0. Exit\n"        ;

//------------------------------------------------------------------------------
// 설명 : 환경설정 메뉴 루프이다. 
// 주의 : 
//------------------------------------------------------------------------------
int     ModifyCfg(int argc, char **argv)
{
        TConfig SvCfg;  // 환경 작업 변수
        char    rbuff[256];

        // 환경변수를 복사하여 사용한다.
        memcpy( &SvCfg, &Cfg, sizeof(TConfig) );
        
        while (1)
        {
                printf( "\n\n" ); printf( " ^^;\n" );
                
                printf( CFG_MENU_1_STR, SvCfg.Local_MacAddress[0], SvCfg.Local_MacAddress[1], SvCfg.Local_MacAddress[2], SvCfg.Local_MacAddress[3], SvCfg.Local_MacAddress[4], SvCfg.Local_MacAddress[5] );
                printf( CFG_MENU_2_STR, (SvCfg.Local_IP>>0)&0xff, (SvCfg.Local_IP>>8)&0xff, (SvCfg.Local_IP>>16)&0xff, (SvCfg.Local_IP>>24)&0xff ); 
                printf( CFG_MENU_3_STR, (SvCfg.Host_IP >>0)&0xff, (SvCfg.Host_IP >>8)&0xff, (SvCfg.Host_IP >>16)&0xff, (SvCfg.Host_IP >>24)&0xff ); 
                printf( CFG_MENU_4_STR, SvCfg.TFTP_Directory       );
                printf( CFG_MENU_5_STR, SvCfg.TFTP_zImageFileName  );
                printf( CFG_MENU_6_STR, SvCfg.TFTP_RamDiskFileName );
                printf( CFG_MENU_7_STR, SvCfg.TFTP_LoaderFileName  );
                printf( "\n" );        
                printf( CFG_MENU_A_STR, SvCfg.AutoBootWaitTime     );
                printf( CFG_MENU_B_STR, SvCfg.BootMenuKey          );
                printf( CFG_MENU_C_STR, SvCfg.UseRamdisk           );
                printf( CFG_MENU_D_STR, SvCfg.Kernel_ArchNumber    ); 
                printf( CFG_MENU_E_STR, SvCfg.SeriaNumber          ); 
                printf( CFG_MENU_F_STR, SvCfg.NandPar[0],SvCfg.NandPar[1],SvCfg.NandPar[2] );
                printf( CFG_MENU_W_STR, SvCfg.Watchdog             );

                printf( "\n" );        
                printf( CFG_MENU_K_STR, SvCfg.KCmd.cmd_A );
                printf( CFG_MENU_M_STR, SvCfg.KCmd.cmd_B );
                printf( CFG_MENU_N_STR, SvCfg.KCmd.cmd_C );
                
                printf( "\n" );
                printf( CFG_MENU_L_STR );
                printf( CFG_MENU_P_STR );
                printf( CFG_MENU_S_STR );
                printf( CFG_MENU_0_STR );

                printf( "\n  Select >>" );
                
                gets( rbuff ); 
                
                switch ( toupper(rbuff[0]) )
                {
                case '1' : GetMacAddress ( CFG_MENU_1_STR, (Byte *)&(SvCfg.Local_MacAddress), toupper(rbuff[1]) == 'A' ? 1:0 ); break;
                	   
                case '2' : GetIPAddress  ( CFG_MENU_2_STR, &(SvCfg.Local_IP)                 ); break;
                case '3' : GetIPAddress  ( CFG_MENU_3_STR, &(SvCfg.Host_IP )                 ); break;
                case '4' : GetString     ( CFG_MENU_4_STR, SvCfg.TFTP_Directory              ); break;
                case '5' : GetString     ( CFG_MENU_5_STR, SvCfg.TFTP_zImageFileName         ); break;
                case '6' : GetString     ( CFG_MENU_6_STR, SvCfg.TFTP_RamDiskFileName        ); break;
                case '7' : GetString     ( CFG_MENU_7_STR, SvCfg.TFTP_LoaderFileName         ); break;
                                         
                case 'A' : GetWord32     ( CFG_MENU_A_STR, &(SvCfg.AutoBootWaitTime  )       ); break;
                case 'B' : GetChar       ( CFG_MENU_B_STR, &(SvCfg.BootMenuKey)              ); break;     
                case 'C' : GetChar       ( CFG_MENU_C_STR, &(SvCfg.UseRamdisk)               ); break;     
                case 'D' : GetWord32     ( CFG_MENU_D_STR, &(SvCfg.Kernel_ArchNumber)        ); break;     
                case 'E' : GetWord32     ( CFG_MENU_E_STR, &(SvCfg.SeriaNumber)              ); break;     
                case 'F' : GetWord16_arg3( CFG_MENU_F_STR, SvCfg.NandPar                     ); break;
                case 'W' : GetByte       ( CFG_MENU_W_STR, &(SvCfg.Watchdog)                 ); break;
                

                case 'K' : GetString     ( CFG_MENU_K_STR, SvCfg.KCmd.cmd_A ); break;
                case 'M' : GetString     ( CFG_MENU_M_STR, SvCfg.KCmd.cmd_B ); break;
                case 'N' : GetString     ( CFG_MENU_N_STR, SvCfg.KCmd.cmd_C ); break;
                
                case 'P' : // 적용한다.
                           memcpy( &Cfg, &SvCfg, sizeof(TConfig) );
                           printf( "\n  ...Applyed" );
                           printf( "\n" ); 
                           return 0;
                           
                case 'S' : // 저장한다.  
                           memcpy( &Cfg, &SvCfg, sizeof(TConfig) );
                           printf( "\n" );
                           SaveConfig();
                           printf( "\n  ...Saved" );
                           break;
                           
                case 'L' : // 기본환경값으로 변경한다.
        		   
        		   if ( toupper(rbuff[1]) == 'F' )
			   {
				   strcpy( SvCfg.KCmd.cmd_A, "noinitrd root=/dev/mtdblock2" );
			   }
			   else
			   {
	        		   memcpy( &SvCfg, &Default_Cfg, sizeof(TConfig) );
		           }						   
                           printf( "\n  ...Loaded default" );
                           break;
                           
                case '0' : // exit
                           printf( "\n" ); 
                           return 0;    
                }
        }      
}

//------------------------------------------------------------------------------
// 설명 : 환경을 불러온다.
// 주의 : 
//------------------------------------------------------------------------------
void    LoadConfig( void )
{
        if ( CONFIG_MAGIC == *(Word32 *)(DEFAULT_BOOT_PARAM) )
        {
                memcpy( &Cfg, (unsigned char *)(DEFAULT_BOOT_PARAM), sizeof(TConfig) );
        }
        else
        {
        	// 기본환경값을 적용한후 저장한다.
        	memcpy( &Cfg, &Default_Cfg, sizeof(TConfig) );
        }
}
//------------------------------------------------------------------------------
// 설명 : 환경을 저장한다.
// 주의 : 
//------------------------------------------------------------------------------
void    SaveConfig( void )
{	
	int lp;
	
	if( CopyTo_BootFlash( DEFAULT_BOOT_PARAM, (unsigned int)&Cfg, sizeof(TConfig) ) == -1 )
	{
		for( lp = 0; lp < 10; lp++ )
		{	
			if( CopyTo_BootFlashEx( DEFAULT_BOOT_PARAM, (unsigned int)&Cfg, sizeof(TConfig), 0 ) == 0 )
			{
				printf( "    Verify: Ok\n" );
				break;
			}	
		}
	}
}      

//------------------------------------------------------------------------------
// 설명 : 커널 커맨드 문자열을 돌려준다.
// 주의 : 
//------------------------------------------------------------------------------
int     GetKCmdStr( char *dest )
{
     char *ptr;
     char nand_cmd[32];
     char ip_cmd[32];
     char arch_cmd[32];

	 memcpy( dest, &(Cfg.KCmd), 12 );
     ptr = &dest[12];
 
     sprintf( ip_cmd,   "ip0=%d.%d.%d.%d"   , (Cfg.Local_IP>>0)&0xff, (Cfg.Local_IP>>8)&0xff, (Cfg.Local_IP>>16)&0xff, (Cfg.Local_IP>>24)&0xff );
     sprintf( nand_cmd, "nandparts=%d,%d,%d", Cfg.NandPar[0],Cfg.NandPar[1],Cfg.NandPar[2]);
     sprintf( arch_cmd, "arch=%s", CMD_ARCH_NAME );
 
     sprintf( ptr, " %s %s %s %s %s %s\0", Cfg.KCmd.cmd_A, Cfg.KCmd.cmd_B, Cfg.KCmd.cmd_C, ip_cmd, nand_cmd, arch_cmd );
 
     return ( strlen( ptr ) + 12 );
}
                                                                                    
