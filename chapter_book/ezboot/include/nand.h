//------------------------------------------------------------------------------
// 파 일 명 : nand.h
// 프로젝트 : ezboot
// 설    명 : ezboot에서 사용하는 nand에 관련된 헤더 
// 작 성 자 : 유영창
// 작 성 일 : 2003년  5월 23일
// 수    정 : 2003년 6월 9일
//            오재경 (주)제이닷디엔티 freefrug@falinux.com  --푸지
// 저 작 권 : (주)제이닷디엔티 
// 주    의 : 
//------------------------------------------------------------------------------

#ifndef _NAND_HEADER_
#define _NAND_HEADER_

#define 	NAND_BASE	DEFAULT_nCS_NAND_BASE

#define NAND_ACCESS_START       *((volatile short *)(NandDev->BaseAddress + 0x000 ))
#define NAND_DATA               *((volatile short *)(NandDev->BaseAddress + 0x000 ))
#define NAND_CMD                *((volatile short *)(NandDev->BaseAddress + 0x100 ))  
#define NAND_ADDR               *((volatile short *)(NandDev->BaseAddress + 0x200 ))  
#define NAND_ACCESS_END         *((volatile short *)(NandDev->BaseAddress + 0x300 ))  

#define	PARTITION_NUM_KERNEL	0
#define	PARTITION_NUM_RAMDISK	1
#define	PARTITION_NUM_APPIMAGE	2


// 각파티션의 0 번페이지는 정보영역으로 사용한다.
typedef struct 
{
	int	data_size;
} __attribute__ ((packed)) TParPageInfo;

// 파티션의 정보
typedef struct 
{
    unsigned char  Name[128];        
    int            BaseBlock;         // 최초 선두 블록 
    int            BlockCount;        // 점유 블록 수 
} __attribute__ ((packed)) TNandPartition;

#define NAND_MAX_PATITION           8

#define NAND_PARTITON_REST_SIZE    -1 
#define NAND_PARTITON_END          -1 

typedef struct 
{
    unsigned long  Type;        
    unsigned long  BaseAddress;         // 위치 주소 
    unsigned long  TotalSize;           // 총 사용 가능 용량
    unsigned long  EraseSize;           // 삭제 단위 블록

    char           BadBlock[1024*8];    // 배드 블록 표시 'X' : 배드블록 / 'O' : 정상
    unsigned short VirBlock[1024*8];    //
    int            BadBlockCount;

    TNandPartition *pPartition;         // 파티션 정보 주소 
    int            PartitionNR;         // 파티션 갯수  
} __attribute__ ((packed)) TNandInfo;


#define NAND_CMD_READ_ID           0x90             // ID 읽기 커맨드 
#define NAND_CMD_READ_STATE        0x70             // 상태 읽기 커맨드 
#define NAND_CMD_ERASE_SETUP       0x60             // 지우기 셋업 커맨드
#define NAND_CMD_ERASE_RUN         0xD0             // 지우기 실행 커맨드
#define NAND_CMD_WRITE_START       0x80             // 쓰기 시작 커맨드
#define NAND_CMD_WRITE_RUN         0x10             // 쓰기 실행 커맨드
#define NAND_CMD_READ_A            0x00             // A 영역 읽기 
#define NAND_CMD_READ_B            0x01             // B 영역 읽기 
#define NAND_CMD_READ_C            0x50             // C 영역 읽기 

#define NAND_STATUS_ERASE_OK       0x01             // bit = 0 일경우 OK 
#define NAND_STATUS_PROGRAM_OK     0x01             // bit = 0 일경우 OK 
#define NAND_STATUS_BUSY           0x40             // bit = 0 일경우 BUSY 
#define NAND_STATUS_PROTECTED      0x80             // bit = 0 일경우 LOCK 

#define NAND_STATUS_NORMAL_MASK    ( NAND_STATUS_ERASE_OK || NAND_STATUS_PROGRAM_OK || NAND_STATUS_BUSY || NAND_STATUS_PROTECTED )

#define NAND_TYPE_UNKNOW             0
#define NAND_TYPE_SAMSUNG_16M      0x73
#define NAND_TYPE_SAMSUNG_32M      0x75
#define NAND_TYPE_SAMSUNG_64M      0x76
#define NAND_TYPE_SAMSUNG_128M     0x79

#ifndef NAND_VAR
  extern TNandInfo Nand1;
  extern TNandPartition  Nand1Partition[NAND_MAX_PATITION];
#endif


extern int  NandFlash_Init( void );
extern BOOL Nand_ErasePartition( int pnb, BOOL NoEraseBadBlock );
extern int  CopyTo_NandFlash_Kernel ( unsigned int src, int size );
extern int  CopyTo_NandFlash_Ramdisk( unsigned int src, int size );
extern int  CopyTo_NandFlash_AppImage( unsigned int src, int size );
extern int  CopyTo_SDRAM_Kernel( unsigned int dst );
extern int  CopyTo_SDRAM_Ramdisk( unsigned int dst );
#endif //_NAND_HEADER_

