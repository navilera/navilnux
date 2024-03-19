//------------------------------------------------------------------------------
// 화일명 : main.c
// 설  명 : ezBoot의 실제적인 메인 루틴이다. 
// 
// 작성자 : 유영창 (주)제이닷디엔티 frog@falinux.com
//          오재경 (주)제이닷디엔티 freefrug@falinux.com -- 푸지
// 작성일 : 2001년 10월 08일
// 수정일 : 2003년 06월 07일 	ez_x5 용으로 수정되었다.
//          2003년 08월 01일 	eztiny 를 추가    			        -- 푸지
//	    2003년 08월 02일	nand.c 타이밍 수정, 연속된 bad-block 버그 수정  -- 푸지
//	    2004년 08월 14일	AM29LV400BT 플래시 추가  			-- 푸지	
// 저작권 : (주)제이닷디엔티 
// 주  의 : 
//------------------------------------------------------------------------------

#include <pxa255.h>
#include <serial.h>
#include <time.h>
#include <gpio.h>
#include <stdio.h>
#include <string.h>

#include <mem_map.h>
#include <config.h>

#include <cs8900.h>
#include <flash.h>
#include <main.h>

/* 외부함수 정의 --------------------------------------------------------------*/

extern int RamDump      (int argc, char **argv);      // 메모리 데이타 덤프 
extern int RamWriteChar (int argc, char **argv);      // 메모리에 데이타 쓰기 ( 1 Byte )
extern int RamWriteWord (int argc, char **argv);      // 메모리에 데이타 쓰기 ( 2 Byte ) 
extern int RamWriteLong (int argc, char **argv);      // 메모리에 데이타 쓰기 ( 3 Byte ) 

extern int Flash_Memory (int argc, char **argv);      // Flash에 메모리 데이타를 쓴다. 

extern int ZModem_Memory(int argc, char **argv);      // ZMODEM 프로토콜을 이용한 화일 다운로드 ( 메모리에 쓴다. )
extern int ZModem_FlashBoot(int argc, char **argv);   // ZMODEM 프로토콜을 이용한 화일 다운로드한후 플래쉬 부트에 쓴다.
extern int ZModem_Kernel(int argc, char **argv); 
extern int ZModem_RamDisk(int argc, char **argv); 
extern unsigned ZModem_Baudrate;

extern int Arp(int argc, char **argv);                 // Arp   프로토콜을 이용하여 HOST Mac Address를 얻어 온다. 
extern int Ping(int argc, char **argv);                // ICMP  프로토콜을 이용하여 Ping 테스트를 한다. 

extern int Tftp (int argc, char **argv);               // Tftp 프로토콜을 이용하여 데이타를 얻어 온다.                                                     
extern int Tftp_FlashBoot(int argc, char **argv);      // Tftp 프로토콜을 이용하여 부트 화일 다운로드한후  플래쉬에 쓴다. 
extern int Tftp_Kernel(int argc, char **argv);         // Tftp 프로토콜을 이용하여 커널 화일 다운로드한다. 
extern int Tftp_RamDisk(int argc, char **argv);        // Tftp 프로토콜을 이용하여 Ram Disk 이미지 화일을  다운로드한다. 
extern int Tftp_AppImage(int argc, char **argv);       // Tftp 프로토콜을 이용하여 AppImage 화일을  다운로드한다. 

extern int GoKernel(int argc, char **argv);            // 커널로 점프한다. 
extern int GoFunction(int argc, char **argv);	       // 메모리에 다운로드된 사용자 함수로 점프한다.

extern int Help(int argc, char **argv);                // help

extern int  gets_his(char *s);
extern void MemoryHexDump( void *address, int size );
extern int  getc_timed( char cmpKey, int mTimeOver );

/* 내부함수 정의 --------------------------------------------------------------*/

int Soft_Reset(int argc, char **argv);
int TestFunction(int argc, char **argv);        		// 시험용 


/* 광역 변수 정의 -------------------------------------------------------------*/

extern TConfig Cfg;		// config.c 에 정의


/* 명령 리스트 ----------------------------------------------------------------*/

TCommand Cmds[] = { 
        
        { "HELP" , Help            },
        { "?"    , Help            },
        
        { "RST"  , Soft_Reset      },	// main.c
        { "TST"  , TestFunction    },	
        
        { "ARP"  , Arp             },	// arp_cmd.c
        { "PING" , Ping            },
        
        { "GK"   , GoKernel        },	// go_cmd.c
        { "GO"   , GoFunction      },
        
        { "MD"   , RamDump         },   // ram_cmd.c
        { "MWB"  , RamWriteChar    },
        { "MWW"  , RamWriteWord    },
        { "MWL"  , RamWriteLong    },

        { "NEK"  , ErasePartiotion },	// flash_cmd.c
        { "NER"  , ErasePartiotion },
        { "NEA"  , ErasePartiotion },

        { "ZMK"  , ZModem_Kernel   },	// zmodem_cmd.c
        { "ZFK"  , ZModem_Kernel   },
        { "ZMR"  , ZModem_RamDisk  },
        { "ZFR"  , ZModem_RamDisk  },
        { "ZFB"  , ZModem_FlashBoot},
        { "ZM"   , ZModem_Memory   },
        { "ZF"   , ZModem_Memory   },

        { "TMK"  , Tftp_Kernel     },	// tftp_cmd.c
        { "TFK"  , Tftp_Kernel     },
        { "TMR"  , Tftp_RamDisk    },
        { "TFR"  , Tftp_RamDisk    },
        { "TFB"  , Tftp_FlashBoot  },
        { "TM"   , Tftp            },
        { "TF"   , Tftp            },

        { "SET"  , ModifyCfg       },	// config.c
        
        { NULL   , NULL            }
};

/* implimantation =============================================================*/


//------------------------------------------------------------------------------
// 설명 : 소프트 리셋을 수행한다. 
// 매계 : argc    : 토큰 갯수 
//        argv    : 분리된 토큰 문자열 주소가 담겨질 배열 
// 반환 : 에러 -1  정상 0
// 주의 : 워치독 타이머를 이용하여 리부팅을 한다. 
//------------------------------------------------------------------------------
int 	Soft_Reset(int argc, char **argv)
{
	printf("System Soft Reset.......\n");

	SetWatchdog(10);	// 10msec
	
    	return 0;
}
//------------------------------------------------------------------------------
// 설명 : 명령문자를 각 토큰별로 분리하여 반환한다. 
// 매계 : cmdline : 명령라인
//        argv    : 분리된 토큰 문자열 주소가 담겨질 배열 
// 반환 : 토큰수 
// 주의 : 
//------------------------------------------------------------------------------
static const char *delim = " \f\n\r\t\v";
static int parse_args(char *cmdline, char **argv)
{
	char *tok;
	int argc = 0;

	argv[argc] = NULL;
                   
	for (tok = strtok(cmdline, delim); tok; tok = strtok(NULL, delim)) 
        {
		argv[argc++] = tok;
	}

	return argc;
}

//------------------------------------------------------------------------------
// 설명 : 테스트 함수
// 매계 : 
// 반환 : 
// 주의 : 
//------------------------------------------------------------------------------
int 	TestFunction(int argc, char **argv)     // 시험용 
{
    
}

//------------------------------------------------------------------------------
// 설명 : 테스트 함수
// 매계 : 
// 반환 : 
// 주의 : 
//------------------------------------------------------------------------------
int 	LedBlink( void )
{
	int  delay = (Cfg.AutoBootWaitTime >= 2) ? 250:50;

	GPIO_SetLED( 0, LED_ON  ); GPIO_SetLED( 1, LED_OFF ); GPIO_SetLED( 2, LED_ON  ); GPIO_SetLED( 3, LED_OFF );
	
	msleep(delay);
	
	GPIO_SetLED( 0, LED_OFF ); GPIO_SetLED( 1, LED_ON  ); GPIO_SetLED( 2, LED_OFF ); GPIO_SetLED( 3, LED_ON  );
	
	msleep(delay);
	
	GPIO_SetLED( 0, LED_OFF ); GPIO_SetLED( 1, LED_OFF ); GPIO_SetLED( 2, LED_OFF ); GPIO_SetLED( 3, LED_OFF );
}

//------------------------------------------------------------------------------
// 설명 : 프린트 포트로 출력한다. 
// 매계 : vptrMng : JTAG제어 정보 메모리 주소 
// 반환 : 없음 
// 주의 : 현재 아직 DLC5에 대한 고려만 한 상태이다. 
//------------------------------------------------------------------------------
int 	main( void )
{
	int  start_option = *(int *)(DEFAULT_RAM_KERNEL_ZERO_PAGE);
	char ReadBuffer[ 1024 ];
	int   argc;
	char *argv[128];
	int   cmdlp;
	
	// 환경값을 불러온다.
    	LoadConfig();

    	// 타이머를 초기화 한다. 
    	TimerInit();
    
    	// GPIO를 초기화 한다. 
    	GPIOInit();

    	// 시리얼을 초기화 한다. 
    	SerialInit( BAUD_115200 );
    	ZModem_Baudrate = 115200;

    	// 이지부트 시작 메세지를 보여준다. 
    	printf( "\n\n");
    	printf( "WELCOME EZBOOT.X5 V1.8...................for PXA255\n");
    	printf( "Program by You Young-chang, fooji (FALinux Co.,Ltd)\n");
    	printf( "Last Modify %s\n\n", __DATE__);

	// LED 를 깜박여 정상임을 표시한다.
	LedBlink();

    	// 부트 플래쉬 초기화
    	BootFlash_Init();	printf("\n");

    	// NAND 플래쉬 초기화 
	NandFlash_Init();	printf("\n");
    	
    	// CS8900을 초기화 한다. 
    	CS8900_Init();		printf("\n");

	// eztiny 에서 넘어온것이라면 메모리의 내용을 플래시에 복사한다.
	switch ( start_option )
	{
	// [ezboot]
	case 0x0000 : 
    	     	// Autoboot =======================
    	     	printf( "Quickly Autoboot [ENTER] / " );
    	     	if( Cfg.BootMenuKey == ' ' )  printf( "Goto BOOT-MENU press [space bar]");
    	     	else                          printf( "Goto BOOT-MENU press [%c]", Cfg.BootMenuKey ); 
	     	
    	     	if ( getc_timed( Cfg.BootMenuKey, Cfg.AutoBootWaitTime*1000 ) )
    	     	{
       	     		//printf( "\r                                                                \n");  
       	     		printf( "\n");  
             		CopyImage();
             		GoKernel( 1, NULL );
             		// 여기로 움직이지 않는다.
             		// ---
    	     	}
    	     	break;
    	     	
	// [eztiny]
	case 0x0001 : SelfCopy_BootLoader(); 
		break;
	}

    	printf( "\n\n");

	// Boot propmt =====================
	// 명령을 대기한다. 
	while (1)
	{
		printf( "EZBOOT>");
                
        	memset( ReadBuffer, 0 , sizeof( ReadBuffer ) );
        	// 시리얼에서 명령어가 입력될때까지 대기한다.
        	gets_his( ReadBuffer );
        	printf( "\n");

		argc = parse_args( ReadBuffer, argv );
		if (argc)
        	{
			UpperStr( argv[0] );
			cmdlp = 0;
    			while( Cmds[cmdlp].CmdStr )
    			{
				if( strcmp( argv[0], Cmds[cmdlp].CmdStr ) == 0 )
                		{
					Cmds[cmdlp].func( argc, argv );
					printf( "\n");	
					break;
                		}
				cmdlp++;
    			}
		}
	}
	
}

