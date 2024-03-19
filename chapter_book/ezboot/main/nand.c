//------------------------------------------------------------------------------
// 화일명 : nand.c
// 설  명 : ezBoot의 NAND 칩 제어에 대한 처리 루틴이다. 
// 
// 작성자 : 유영창 (주)제이닷디엔티 frog@falinux.com
// 작성일 : 2003년 3월  5일
// 수  정 : 2003년 6월  9일  -- 푸지
//          2003년 8월 12일  배드블럭처리와 타이밍등을 수정하였다. -- 푸지
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

#include <mem_map.h>
#include <config.h>

#define NAND_VAR   1
#include <nand.h>


/* 광역 변수 정의 -------------------------------------------------------------*/

extern TConfig Cfg;		// config.c 에 정의

TNandInfo       Nand1;
TNandPartition  Nand1Partition[NAND_MAX_PATITION] =
{
    // NAME                  START      	        SIZE    
    { "Kernel ",                 0,                       64 },		// 1-MByte Size:0x100000
    { "Ramdisk",                64,                      192 },		// 3-MByte Size:0x300000
    { "Generel",               256,  NAND_PARTITON_REST_SIZE },		
    { ""       , NAND_PARTITON_END,                        0 },
};

#define	USED_BOOT_BLOCK_COUNT	(Nand1Partition[0].BlockCount+Nand1Partition[1].BlockCount)

#define	USE_PARTITION_MAX	3

//******************************************************************************
// 	함수 정의
//******************************************************************************

#define	BLK_CNT_1M		64

static void NAND_Par_Fixup( void )
{
	Nand1Partition[0].BaseBlock  = 0;
	Nand1Partition[0].BlockCount = Cfg.NandPar[0] * BLK_CNT_1M;
	
	Nand1Partition[1].BaseBlock  = Cfg.NandPar[0] * BLK_CNT_1M;
	Nand1Partition[1].BlockCount = Cfg.NandPar[1] * BLK_CNT_1M;
	
	Nand1Partition[2].BaseBlock  = (Cfg.NandPar[0]+Cfg.NandPar[1]) * BLK_CNT_1M;
	Nand1Partition[2].BlockCount = Cfg.NandPar[2] * BLK_CNT_1M;
}
//------------------------------------------------------------------------------
// 설명 : 지연
// 주의 : 
//------------------------------------------------------------------------------
void	nand_dumy( void )
{
	return;	
}
void	NAND_AddressSetupWait( void )
{
	int	wait = 250;
	
	while( wait-- ) nand_dumy();
}
//------------------------------------------------------------------------------
// 설명 : BLOCK의 배드 블럭 상태를 체크한다. 
// 매계 : 
// 반환 : 에러 -1  정상 0
// 주의 : 없음 
//------------------------------------------------------------------------------
BOOL 	NAND_CheckBadBlock( TNandInfo *NandDev,  int BlockNumber )
{
    	unsigned long Dummy;
    	unsigned long BlockAddr;
    	unsigned char State;
    
    	BlockAddr = (BlockNumber << 5 );
    
    	Dummy = NAND_ACCESS_START;
    	NAND_CMD      =         NAND_CMD_READ_C;     // C Area Read Command               
    	NAND_ADDR     =                   0x05;      // Bad Check Offset      Col                 
    	NAND_ADDR     =  (BlockAddr     & 0xFF);     // Block Page Address L  Row0
    	NAND_ADDR     = ((BlockAddr>>8) & 0xFF);     // Block Page Address M  Row1 
    	if( NandDev->TotalSize >= 64*1024*1024 )     // Block Page Address H  Row2
    	{
        	NAND_ADDR = ((BlockAddr>>16) & 0xFF);
    	}
    	NAND_AddressSetupWait();
    
    	State = NAND_DATA & 0xFF;
    	Dummy = NAND_ACCESS_END;
    
    	NandDev->BadBlock[BlockNumber] = 'O';
    	if( State != 0xFF )
    	{
        	NandDev->BadBlock[BlockNumber] = 'X';
         	return FALSE;
    	}     
    
    	return TRUE;
}

//------------------------------------------------------------------------------
// 설명 : NAND의 BAD 블럭을 검출한다. 
// 매계 : BaseAddress : NAND 플레쉬의 베이스 주소 
//        NandDev     : 검출후 플래쉬 정보 저장 주소 
// 반환 : 배드 블럭 수 
// 주의 : 전체블럭을 확인하지 않고 부트로더에서 사용되는 커널,램디스크영역만 확인한다.
//------------------------------------------------------------------------------
int 	NAND_ScanBadBlock( TNandInfo *NandDev )
{
    	int ScanBlock, IndexBlock;
    	int ReplaceBlock,  VirIndex;
    
    	ScanBlock = USED_BOOT_BLOCK_COUNT;  //NandDev->TotalSize/NandDev->EraseSize; 
    	NandDev->BadBlockCount = 0;
    
    	for( IndexBlock = 0; IndexBlock < ScanBlock; IndexBlock++ )
    	{
        	if( NAND_CheckBadBlock( NandDev, IndexBlock ) == FALSE )
        	{
            		NandDev->BadBlockCount++;
        	}
    	}
    	
    	// 배드블럭을 제거한 가상블럭을 정열한다.
    	ReplaceBlock = 0;
    	VirIndex     = 0;
    	for( IndexBlock = 0; IndexBlock < ScanBlock; IndexBlock++ ) 
    	{
        	if ( 'X' == NandDev->BadBlock[IndexBlock] )
        	{
			ReplaceBlock ++;
			continue;
        	}

		if ( ReplaceBlock < ScanBlock )
		{
       			NandDev->VirBlock[VirIndex++] = ReplaceBlock++;
       		}
       		else
       		{
       			NandDev->VirBlock[VirIndex++] = ScanBlock-1;
       		}
    	}
    	
    	return NandDev->BadBlockCount;
}
//------------------------------------------------------------------------------
// 설명 : NAND를 지운다.
// 매계 : NandDev     : NAND 정보 
//        BlockNumber : 블럭 정보 
// 반환 : 에러 FALSE : 정상 TRUE
// 주의 : 없음 
//------------------------------------------------------------------------------
BOOL 	NAND_EraseBlock( TNandInfo *NandDev,  int BlockNumber )
{
    	unsigned long Dummy;
    	unsigned long BlockAddr;
    	unsigned char State;
    
        // 이 함수를 호출하기 전에 아래의 내용을 확인한다
    	// if( NandDev->BadBlock[BlockNumber] == 'X' ) return TRUE; // 배드 블럭이면 지우지 않는다. 

    	BlockAddr = BlockNumber << 5;
    	Dummy         = NAND_ACCESS_START;
    	NAND_CMD      =   NAND_CMD_ERASE_SETUP;      // C Area Read Command
    	NAND_ADDR     =  (BlockAddr     & 0xFF);     // Block Page Address L
    	NAND_ADDR     = ((BlockAddr>>8) & 0xFF);     // Block Page Address M   
    	if( NandDev->TotalSize >= 64*1024*1024 )     // Block Page Address H
    	{
        	NAND_ADDR = ((BlockAddr>>16) & 0xFF);    
    	}
    	NAND_AddressSetupWait();

    	NAND_CMD  = NAND_CMD_ERASE_RUN;              // NAND 지우기 실행
    
    	while( 1 )
    	{
    		NAND_CMD  = NAND_CMD_READ_STATE;                         // 현재 상태 요구 명령
    		State     = NAND_DATA & 0xFF;                            // 현재 상태를 읽는다. 
    		if( ( State & NAND_STATUS_BUSY      ) != 0 ) break;      // Busy 상태이면 계속 돈다. 
    	}

    	Dummy     = NAND_ACCESS_END;

    	if( ( State & NAND_STATUS_ERASE_OK  ) != 0 )
    	{
    		printf( "NAND Erase Block [%d] Fail\n", BlockNumber );
    		if( ( State & NAND_STATUS_PROTECTED ) == 0 )
    		{
        		printf( "NAND Device Lock\n" );
    		}
    		return FALSE;
    	}
	
/*
	//debuging
  
    	NAND_CMD      = NAND_CMD_READ_A;                           // NAND 읽기 시작 
    	NAND_ADDR     =                    0x00;      
    	NAND_ADDR     = (BlockNumber << 5) & 0xff;
    	NAND_ADDR     = (BlockNumber >> 3) & 0xff;
    	if( NandDev->TotalSize >= 64*1024*1024 )     // Page Address H
    	{
        	NAND_ADDR = (BlockNumber >> 11) & 0xff;
    	}
    	NAND_AddressSetupWait();

	printf( "[%d] %02x %02x\n", BlockNumber, NAND_DATA, NAND_DATA );
*/
	
    	return TRUE;
}

//------------------------------------------------------------------------------
// 설명 : NAND Page를 읽는다. 
// 매계 : NandDev    : NAND 정보
//        PageNumber : 읽으려는 페이지 번호 
//        pBuf       : 512 바이트 크기의 버퍼 주소 
// 반환 : 없음 
// 주의 : 없음 
//------------------------------------------------------------------------------
void 	NAND_ReadPage( TNandInfo *NandDev, int PageNumber, unsigned char *pBuf )
{
    	unsigned long  Dummy;
    	unsigned long  Loop;
    	unsigned char *pReadBuf;
    	unsigned long  *PtrNandData;
    	
    	PtrNandData   = (unsigned long *)&(NAND_DATA);
    	pReadBuf      = pBuf;
    
    	Dummy         = NAND_ACCESS_START;
    
    	NAND_CMD      = NAND_CMD_READ_A;                           // NAND 읽기 시작 
    	NAND_ADDR     =                    0x00;      
    	NAND_ADDR     =  (PageNumber     & 0xFF);    // Page Address L
    	NAND_ADDR     = ((PageNumber>>8) & 0xFF);    // Page Address M   
    	if( NandDev->TotalSize >= 64*1024*1024 )     // Page Address H
    	{
        	NAND_ADDR = ((PageNumber>>16) & 0xFF);    
    	}

    	NAND_AddressSetupWait();
    	
    	// 512 바이트를 읽는다. 
    	for( Loop = 0; Loop < 512; Loop++ ) *pReadBuf++ = *PtrNandData;  

    	Dummy     = NAND_ACCESS_END;

}    	
//------------------------------------------------------------------------------
// 설명 : NAND Page에 데이타를 써넣는다. 
// 매계 : NandDev    : NAND 정보
//        PageNumber : 써 넣으려는 페이지 번호 
//        pBuf       : 512 바이트 크기의 버퍼 주소 
// 반환 : 에러 -1  정상 0
// 주의 : 없음 
//------------------------------------------------------------------------------
BOOL 	NAND_WritePage( TNandInfo *NandDev, int PageNumber, unsigned char *pBuf )
{	
    	unsigned long  Dummy;
    	unsigned long  Loop;
    	unsigned char  State;
    	unsigned char *pWriteBuf;
    	
    	pWriteBuf = pBuf;	
    
    	Dummy     = NAND_ACCESS_START;
    
    	NAND_CMD      = NAND_CMD_READ_A;             // NAND 읽기 시작 
    	NAND_CMD      = NAND_CMD_WRITE_START;        // NAND 써넣기 시작 
    	NAND_ADDR     =                    0x00;      
    	NAND_ADDR     =  (PageNumber     & 0xFF);    // Page Address L
    	NAND_ADDR     = ((PageNumber>>8) & 0xFF);    // Page Address M   
    	if( NandDev->TotalSize >= 64*1024*1024 )     // Page Address H
    	{
        	NAND_ADDR = ((PageNumber>>16) & 0xFF);    
    	}
    	NAND_AddressSetupWait();
    
    	// 512 바이트를 써 넣는다. 
    	for( Loop = 0; Loop < 512 ; Loop++ ) NAND_DATA = *pWriteBuf++;
    
    	NAND_CMD  = NAND_CMD_WRITE_RUN;								// NAND 써넣기 실행 
    
    	while( 1 )
    	{
        	NAND_CMD  = NAND_CMD_READ_STATE;						// 현재 상태 요구 명령
    		State     = NAND_DATA & 0xFF;							// 현재 상태를 읽는다. 
    		if( ( State & NAND_STATUS_BUSY      ) != 0 ) break;		// Busy 상태이면 계속 돈다. 
    	}
    	Dummy = NAND_ACCESS_END;
    
    	if( ( State & NAND_STATUS_PROGRAM_OK ) == 0 ) return TRUE;
    	printf( "NAND Write  Fail\n" );
    
    	if( ( State & NAND_STATUS_PROTECTED  ) == 0 ) 
    	{
        	printf( "NAND Device Lock\n" );
    	}    

    	return FALSE;
}

//------------------------------------------------------------------------------
// 설명 : Sector 인덱스를 Page 인덱스로 변경하여 가져 온다. 
// 매계 : NandDev         : 플래쉬 정보 저장 주소 
//        PartitionNumber : 파티션 번호 
//        SectorNumber    : 섹터 번호 
// 반환 : 실제 페이지번호 
// 주의 : 없음 
//------------------------------------------------------------------------------
int 	NAND_GetPageFromSector( TNandInfo *NandDev, int PartitionNumber, int SectorNumber )
{
    	int	VirBlock;	// 배드블럭을 건너띤 가상블럭
    	int 	RealBlock;	// 실제블럭
    
    	if( PartitionNumber >= NandDev->PartitionNR ) return -1;
    	
    	RealBlock = NandDev->pPartition[PartitionNumber].BaseBlock + (SectorNumber / 32);
	VirBlock  = NandDev->VirBlock[RealBlock];
	
	if ( 0xffff == VirBlock ) return -1;

	return ( VirBlock*32 + (SectorNumber%32) );
}
//------------------------------------------------------------------------------
// 설명 : NAND Sector에 데이타를 써넣는다. 
// 매계 : NandDev    : NAND 정보
//        PageNumber : 써 넣으려는 페이지 번호 
//        pBuf       : 512 바이트 크기의 버퍼 주소 
// 반환 : 에러 -1  정상 0
// 주의 : 없음 
//------------------------------------------------------------------------------
BOOL 	NAND_WriteSector( TNandInfo *NandDev, int PartitionNumber, int SectorNumber, unsigned char *pBuf )
{
    	int PageNumber;
    	int BlockNumber;
    
    	PageNumber = NAND_GetPageFromSector( NandDev, PartitionNumber, SectorNumber );
    	if( PageNumber < 0 ) return FALSE;
    
    	BlockNumber = PageNumber / 32;
    
    	// 포함 범위 체크 
    	if( NandDev->pPartition[PartitionNumber].BaseBlock > BlockNumber ) return FALSE;
    	if( (  NandDev->pPartition[PartitionNumber].BaseBlock 
        	 + NandDev->pPartition[PartitionNumber].BlockCount ) <= BlockNumber ) return FALSE;

    	// 데이타를 써 놓고 복귀 한다.    
    	return NAND_WritePage( NandDev, PageNumber, pBuf );
} 
//------------------------------------------------------------------------------
// 설명 : NAND Sector를 읽는다. 
// 매계 : NandDev    : NAND 정보
//        PageNumber : 읽으려는 페이지 번호 
//        pBuf       : 512 바이트 크기의 버퍼 주소 
// 반환 : 없음 
// 주의 : 없음 
//------------------------------------------------------------------------------
BOOL 	NAND_ReadSector( TNandInfo *NandDev, int PartitionNumber, int SectorNumber, unsigned char *pBuf )
{
    	int PageNumber;
    	int BlockNumber;
    
    	PageNumber = NAND_GetPageFromSector( NandDev, PartitionNumber, SectorNumber );
    	if( PageNumber < 0 ) return FALSE;
    
    	BlockNumber = PageNumber / 32;
    
    	// 포함 범위 체크 
    	if( NandDev->pPartition[PartitionNumber].BaseBlock > BlockNumber ) return FALSE;
    	if( (  NandDev->pPartition[PartitionNumber].BaseBlock 
        	 + NandDev->pPartition[PartitionNumber].BlockCount ) <= BlockNumber ) return FALSE;

    	NAND_ReadPage( NandDev, PageNumber, pBuf );
    	

    	return TRUE;
}

//------------------------------------------------------------------------------
// 설명 : NAND를 검출한다. 
// 매계 : BaseAddress : NAND 플레쉬의 베이스 주소 
//        NandDev     : 검출후 플래쉬 정보 저장 주소 
//        
// 반환 : 에러 FALSE 정상 TRUE
// 주의 : 없음 
//------------------------------------------------------------------------------
BOOL 	NAND_DETECT( unsigned long BaseAddress , TNandInfo *NandDev , TNandPartition *par )
{
    	unsigned char  MakerID, SizeID;
    	unsigned short Dummy;
    	int            Loop;
    	int            RestBlockCount;
    	
    	printf( "NAND Chip Check .......................\n" );
    	//printf( "  Base Address [%08X]\n", (unsigned int)BaseAddress );
    
    	NandDev->BaseAddress = BaseAddress;
  
    	Dummy     = NAND_ACCESS_START;
    	NAND_CMD  = NAND_CMD_READ_ID;                             // ID 읽기 명령
    	NAND_ADDR = 0x00;                                         // 명령어 이후 부가적인 행동

    	MakerID   = NAND_DATA & 0xFF;                             // 제조사 읽기 
    	SizeID    = NAND_DATA & 0xFF;                             // 크기 읽기 
    	Dummy     = NAND_ACCESS_END;
    
    	//printf( "  NAND Maker ID [%02X]  ", MakerID );
    	//printf( "  NAND Size  ID [%02X]\n", SizeID );
    
    	NandDev->Type      = NAND_TYPE_UNKNOW;
    	switch( MakerID )
    	{
    	case 0xEC : 
                switch( SizeID )
                {
                case 0x73 : NandDev->Type = NAND_TYPE_SAMSUNG_16M ; 
                            NandDev->TotalSize =  16 * 1024 * 1024; 
                            NandDev->EraseSize =  16 * 1024; 
                            break;
                case 0x75 : NandDev->Type = NAND_TYPE_SAMSUNG_32M ; 
                            NandDev->TotalSize =  32 * 1024 * 1024; 
                            NandDev->EraseSize =  16 * 1024; 
                            break;
                case 0x76 : NandDev->Type = NAND_TYPE_SAMSUNG_64M ; 
                            NandDev->TotalSize =  64 * 1024 * 1024; 
                            NandDev->EraseSize =  16 * 1024; 
                            break;
                case 0x79 : NandDev->Type = NAND_TYPE_SAMSUNG_128M; 
                            NandDev->TotalSize = 128 * 1024 * 1024; 
                            NandDev->EraseSize =  16 * 1024; 
                            break;
                }
                break;
    	} 
    
    	switch( NandDev->Type )
    	{
    	case NAND_TYPE_SAMSUNG_16M  : printf( "  Detect SAMSUNG [%02x:%02x] 16MByte\n",  MakerID, SizeID ); break;
    	case NAND_TYPE_SAMSUNG_32M  : printf( "  Detect SAMSUNG [%02x:%02x] 32MByte\n",  MakerID, SizeID ); break;
    	case NAND_TYPE_SAMSUNG_64M  : printf( "  Detect SAMSUNG [%02x:%02x] 64MByte\n",  MakerID, SizeID ); break;
    	case NAND_TYPE_SAMSUNG_128M : printf( "  Detect SAMSUNG [%02x:%02x] 128MByte\n", MakerID, SizeID ); break;
    	default                     : printf( "  Unknown Type\n"      ); break; 
    	}
    
    	if( NandDev->Type == NAND_TYPE_UNKNOW ) return FALSE;

    	// 파티션 정보를 연결한다.
    	NandDev->pPartition  = par;
    	NandDev->PartitionNR = 0;
    
    	// BAD 블럭 스캔 
    	printf( "  BAD BLOCK SCAN ->" );
    	printf( "  Kernel, Ramdisk Bad Block [%d]\n", NAND_ScanBadBlock( NandDev ) );

    	// 파티션 정보를 체크 한다. 
    	RestBlockCount = NandDev->TotalSize / NandDev->EraseSize;
    	for( Loop = 0; Loop < NAND_MAX_PATITION; Loop++ )
    	{
	        if( NandDev->pPartition[Loop].BaseBlock == NAND_PARTITON_END ) break;

        	NandDev->PartitionNR++;
        	if( NandDev->pPartition[Loop].BlockCount == NAND_PARTITON_REST_SIZE )
        	{
            		NandDev->pPartition[Loop].BlockCount = RestBlockCount;  
            		break;
        	} 
        	else
        	{
            		RestBlockCount -= NandDev->pPartition[Loop].BlockCount;
        	}
    	}
/*
    	// 파티션 정보를표출한다. 
    	printf( " ====================================================================\n" );
    	for( Loop = 0; Loop < NandDev->PartitionNR; Loop++ )
    	{
        	printf( "  NAND Partition %d -> START %6d, SIZE %6d, Name : %s\n",
                     Loop,
                     NandDev->pPartition[Loop].BaseBlock ,
                     NandDev->pPartition[Loop].BlockCount ,
                     NandDev->pPartition[Loop].Name );
    	}
    	printf( " --------------------------------------------------------------------\n" );
*/      
    
    	return TRUE;
}
//------------------------------------------------------------------------------
// 설명 : 진행상태를 표시 한다. 
// 매계 : title : 상태젬고 
//        cur   : 현재 진행도 
//        max   : 최대값
// 반환 : 
// 주의 : 없음 
//------------------------------------------------------------------------------
static int dpywidth  = 50;
static char bar[]    = "===============================================================";
static char spaces[] = "                                                               "
		       "                                                               ";
static int progress_pos = 0;

void 	NandFlash_UpdateProgress(const char *title, unsigned long cur, unsigned long max)
{
	const char spinner[] = "\\|/-";
	unsigned int percent;
	int i;

	progress_pos = (progress_pos+1) & 3;
	percent = (cur * 100) / max;
	i = ((percent * dpywidth) + 50) / 100;

	printf("%s: |%s%s", title,
		bar + (sizeof(bar) - (i+1)),
		spaces + (sizeof(spaces) - (dpywidth - i + 1)));
		
	if (percent == 100)
	     putc('|');
	else putc(spinner[progress_pos & 3]);
		
	printf(" %4d%%   \r", percent);
	if (percent == 100)	// clear prog bar 
	{
		printf("%s\r", spaces + (sizeof(spaces) - 80));
	}
}
//------------------------------------------------------------------------------
// 설 명 : NAND 플래쉬 지우기 
// 인 자 : pnb - 파티션 번호
// 주 의 : 
//------------------------------------------------------------------------------
BOOL 	Nand_ErasePartition( int pnb, BOOL NoEraseBadBlock )
{
	int   	From, MaxCount,Loop;
	int	BadBlockCnt = 0;
   	
   
   	From     =   Nand1.pPartition[pnb].BaseBlock;
   	MaxCount =   Nand1.pPartition[pnb].BlockCount;

   	for( Loop = 0; Loop < MaxCount; Loop++ )
   	{
		NandFlash_UpdateProgress( "    Erasing", Loop+1, MaxCount );
       		// 배드 블럭이면 지우지 않는다. 
       		if ( NoEraseBadBlock == TRUE )
       		{
       			if (Nand1.BadBlock[From] != 'X')
       			{
       				NAND_EraseBlock( &Nand1, From );
       			}
       			else
       			{
       				BadBlockCnt ++;
       			}
       		}
       		else
       		{
       			if ( NAND_EraseBlock( &Nand1, From ) == FALSE ) 
       			{
       				BadBlockCnt ++;
       			}
       		}
       		
       		From ++;
   	}
	NandFlash_UpdateProgress( "    Erasing", 100, 100 );
	printf( "    Erase : OK\n" );
   
   	return TRUE;
}
//------------------------------------------------------------------------------
// 설 명 : NAND 플래쉬 쓰기
// 인 자 : pnb - 파티션 번호
// 주 의 : 
//------------------------------------------------------------------------------
int 	Nand_ProgramPartition(int pnb, unsigned int src, int size )
{
	int	page_cnt, page;
	unsigned char *pbuf = (unsigned char *)src;
	
	TParPageInfo  *pParInfo;
	unsigned char page_buff[512];

   	if ( ( 0 > pnb ) || ( pnb > USE_PARTITION_MAX ) )
   	{
   		return -1;
   	}

	// 데이타정보 헤더를 처음페이지에 넣는다.
	pParInfo = (TParPageInfo *)page_buff;
	pParInfo->data_size = size;
	NAND_WriteSector( &Nand1, pnb, 0, page_buff );

	// 데이타를 써넣는다.
	page_cnt = (size + 511) / 512;
	for (page=0; page<page_cnt; page++)
	{
		if ( 0 == (page%128) ) NandFlash_UpdateProgress( "    Writing", page+1, page_cnt );
		
		// 0 번페이지는 정보영역으로 사용되고 있다.
		NAND_WriteSector( &Nand1, pnb, page+1, pbuf );
		
		pbuf += 512;
	}
	NandFlash_UpdateProgress( "    Writing", 100, 100 );
	printf( "    Write : OK\n" );
	
	return size;
}
//------------------------------------------------------------------------------
// 설 명 : NAND 플래쉬 데이타 비교
// 인 자 : pnb - 파티션 번호
// 주 의 : 
//------------------------------------------------------------------------------
BOOL	Nand_VerifyPartition( int pnb, unsigned int src, int size )
{
	unsigned char pbuf[512];
	unsigned int  *cmp_src, *cmp_flh;
	int	page_cnt, page; 
	int	loop;
	

	page_cnt = (size + 511) / 512;
	cmp_src  = (unsigned int *)src;
	
	for (page=0; page<page_cnt; page++)
	{
		if ( 0 == (page%128) ) NandFlash_UpdateProgress( "    Verifing", page+1, page_cnt );
		
		// 0 번페이지는 정보영역으로 사용되고 있다.
		NAND_ReadSector( &Nand1, pnb, page+1, pbuf );
		
		cmp_flh  = (unsigned int *)pbuf;
		for(loop=0; loop<512/4; loop++)
		{
			if ( *cmp_src != *cmp_flh )
			{
  				printf( "Verify Error page-%d: %08x/%08x\n", page, *cmp_src, *cmp_flh );
  				return FALSE;
			}
			cmp_src++; cmp_flh++;
		}
	}
	NandFlash_UpdateProgress( "    Verifing", 100, 100 );
	printf( "    Verify: OK\n" );
	
	return TRUE;	
}
//------------------------------------------------------------------------------
// 설 명 : NAND 플래쉬 쓰기
// 인 자 : pnb - 파티션 번호
// 주 의 : 
//------------------------------------------------------------------------------
int 	Nand_ReadPartition(int pnb, unsigned int dst, int size )
{
	int	page_cnt, page, max_page;
	unsigned char *pbuf = (unsigned char *)dst;

	// 0 번페이지는 정보영역으로 사용되고 있다.
	// 이곳에서 크기를 얻어온다.
	TParPageInfo  *pParInfo;
	unsigned char page_buff[512];

	NAND_ReadSector( &Nand1, pnb, 0, page_buff );
	
	pParInfo = (TParPageInfo *)page_buff;
	if ( size > pParInfo->data_size  ) 
	{
		size = pParInfo->data_size;
	}

	page_cnt = (size + 511) / 512;
	max_page = Nand1.pPartition[pnb].BlockCount*32;
	if ( page_cnt > max_page) page_cnt = max_page;
	
	for (page=0; page<page_cnt; page++)
	{
		// 0 번페이지는 정보영역으로 사용되고 있다.
		NAND_ReadSector( &Nand1, pnb, page+1, pbuf );
		pbuf += 512;
	}

	return size;
}
//------------------------------------------------------------------------------
// 설명 : NAND-Flash 를 찾는다.
// 주의 : 없음
//------------------------------------------------------------------------------
int	NandFlash_Init( void )
{
	NAND_Par_Fixup();
	NAND_DETECT( NAND_BASE, &Nand1, &Nand1Partition[0] );

	return 0;
}
//------------------------------------------------------------------------------
// 설명 : NAND-Flash 에 커널 데이타를 써 넣는다.
// 주의 : 없음
//------------------------------------------------------------------------------
int 	CopyTo_NandFlash_Kernel( unsigned int src, int size )
{
	if ( TRUE != Nand_ErasePartition  ( PARTITION_NUM_KERNEL, TRUE      ) )  return -1;
	if ( size != Nand_ProgramPartition( PARTITION_NUM_KERNEL, src, size ) )  return -1;
        if ( TRUE != Nand_VerifyPartition ( PARTITION_NUM_KERNEL, src, size ) )  return -1;

	return size;	
}
//------------------------------------------------------------------------------
// 설명 : NAND-Flash 에 램디스크 데이타를 써 넣는다.
// 주의 : 없음
//------------------------------------------------------------------------------
int 	CopyTo_NandFlash_Ramdisk( unsigned int src, int size )
{
	if ( TRUE != Nand_ErasePartition  ( PARTITION_NUM_RAMDISK, TRUE      ) )  return -1;
	if ( size != Nand_ProgramPartition( PARTITION_NUM_RAMDISK, src, size ) )  return -1;
        if ( TRUE != Nand_VerifyPartition ( PARTITION_NUM_RAMDISK, src, size ) )  return -1;

	return size;
}	
//------------------------------------------------------------------------------
// 설명 : NAND-Flash 의 커널 데이타를 메모리에 복사한다.
// 주의 : 없음
//------------------------------------------------------------------------------
int 	CopyTo_SDRAM_Kernel( unsigned int dst )
{
	int	readcnt;
	
	readcnt = (Nand1.pPartition[PARTITION_NUM_KERNEL].BlockCount-4)*32*512;
	Nand_ReadPartition( PARTITION_NUM_KERNEL, dst, readcnt );

	return readcnt;
}
//------------------------------------------------------------------------------
// 설명 : NAND-Flash 의 램디스크 데이타를 메모리에 복사한다.
// 주의 : 없음
//------------------------------------------------------------------------------
int 	CopyTo_SDRAM_Ramdisk( unsigned int dst )
{
	int	readcnt;

	readcnt = (Nand1.pPartition[PARTITION_NUM_RAMDISK].BlockCount-12)*32*512;
	Nand_ReadPartition( PARTITION_NUM_RAMDISK, dst, readcnt );

	return readcnt;
}
