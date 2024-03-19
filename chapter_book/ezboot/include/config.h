//------------------------------------------------------------------------------
// 파 일 명 : config.h
// 프로젝트 : ezboot
// 설    명 : ezboot에서 사용하는 환경 설정에 관련된 내용
// 작성자 : 유영창 (주)제이닷디엔티 frog@falinux.com
//          오재경 (주)제이닷디엔티 freefrug@falinux.com  -- 푸지
// 작성일 : 2002년 1월 19일
// 수  정 : 2003-06-07	
// 저작권 : (주)제이닷디엔티 
// 주  의 : 
//------------------------------------------------------------------------------

#ifndef _CONFIG_HEADER_
#define _CONFIG_HEADER_

#define CONFIG_MAGIC                    0x89880017
#define CONFIG_CMD_MAGIC                0x20030702
#define DEFAULT_BOOT_WAIT               3
#define DEFAULT_ENTRY_KEY               ' '
#define DEFAULT_SERIAL                  2  

#define DEFAULT_LOCAL_MAC				{0x00, 0xA2, 0x55, 0xF2, 0x26, 0x25}
#define DEFAULT_LOCAL_IP				(192<<0)|(168<<8)|(10<<16)|(155<<24)
#define DEFAULT_HOST_IP                 (192<<0)|(168<<8)|(10<<16)|(24<<24)

#define DEFAULT_TFTP_DIR				""
#define DEFAULT_KERNEL_FILENAME			"zImage.x5"
#define DEFAULT_RAMDISK_FILENAME		"ramdisk.x5-12M.gz"
#define DEFAULT_EZBOOT_FILENAME			"ezboot.x5"

#define CMD_ARCH_NAME                   "EZ-X5"

#define	DEFAULT_NAND_PAR_KERNEL_SZ		1
#define	DEFAULT_NAND_PAR_RAMDISK_SZ		5
#define	DEFAULT_NAND_PAR_APP_SZ			58

#define DEFAULT_KERNEL_COMMAND_A		"initrd=0xa0800000,5M root=/dev/ram ramdisk=12288"
#define DEFAULT_KERNEL_COMMAND_B  		"console=ttyS02,115200"
#define DEFAULT_KERNEL_COMMAND_C  		""

#define	KCMD_BUFF_MAX					80
#define	KCMD_COUNT						3

typedef struct 
{
	Word32	CMD_MagicNumber;        // 커널 커맨드 매직번호 
	char    CMD_Tag[8]; 		// 커널 명령 라인 디폴트 ""
	char    cmd_A[KCMD_BUFF_MAX];
	char    cmd_B[KCMD_BUFF_MAX];
	char    cmd_C[KCMD_BUFF_MAX];	
} __attribute__ ((packed)) TKernelCommandLine;

/*
typedef struct 
{
	Word32	MagicNumber;        	// 매직번호 
	char    Buff[512]; 		// 파라메터 문자열
        
} __attribute__ ((packed)) TParam;
*/

typedef struct SConfig
{
	Word32	MagicNumber;           	// 환경 데이타가 있음의 검증에 대한 매직 번호 0x8988000x
    
	Word32	AutoBootWaitTime;      	// 자동 부트 대기 시간 ( 초 단위 )
	
	char    BootMenuKey;    	// 부트 로더 진입 키 기본 ' '
	char	UseRamdisk;		// 램디스크 사용유무
	char	Watchdog;		// 위치독
	char    Rev1;               	// 예약 영역     

	Byte    Local_MacAddress[8];   	// 보드   MAC Address  [xx:xx:xx:xx:xx:xx] 
	Byte    Host_MacAddress[8];    	// 호스트 MAC Address  [xx:xx:xx:xx:xx:xx] 
                               	    	// 6 바이트만 사용한다.
	Word32  Local_IP;              	// 보드   IP  = 0
	Word32  Host_IP;               	// 호스트 IP  = 0

	char    TFTP_Directory      [64]; // TFTP 디렉토리 명         디폴트 ""
	char    TFTP_zImageFileName [64]; // 커널 이미지      화일 명 디폴트 "zImage" 
	char    TFTP_RamDiskFileName[64]; // 램 디스크 이미지 화일 명 디폴트 "ramdisk.gz" 
	char    TFTP_LoaderFileName [64]; // 부트로더         화일 명 디폴트 "ezboot_x5" 

	Word32	SeriaNumber; 		// 사용할 시리얼 포트 번호  
	Word32  Kernel_ArchNumber;	// 커널에 전달될 커널 아키텍쳐 번호  디폴트 303

	Word16	NandPar[4];		// NAND-Partition 크기

        TKernelCommandLine KCmd; 	// 커널 커맨드 

} __attribute__ ((packed))TConfig;

#ifndef CONFIG_VAR
extern TConfig Cfg;
#endif


extern void   LoadConfig( void );
extern void   SaveConfig( void );
extern int    ModifyCfg(int argc, char **argv);
extern int    GetKCmdStr( char *dest );

#endif // _CONFIG_HEADER_



