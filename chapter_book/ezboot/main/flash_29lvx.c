//------------------------------------------------------------------------------
// 화일명   : flash_29lvx.c
// 프로젝트 : ezboot_x5
// 설  명   : ezboot의 FB29LVxflash 처리 루틴이다. 
// 
// 작성자   : 오재경 (주)제이닷디앤티 freefrug@falinux.com --푸지
// 작성일   : 2003년 5월 23일
//            2003년 6월 05일
//	      2004년 8월 14일   AMD,MXIC 플래시 등 29LV 타입을 지원한다.
// 저작권   : FA리눅스
// 주  의 : 
//------------------------------------------------------------------------------
//******************************************************************************
//
// 헤더 정의
//
//******************************************************************************
#include <pxa255.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <flash_29lvx.h>
#include <mem_map.h>

// flash block
const int FB29LV400T_block_addr[] = 
		{ 0x00000, 0x10000, 0x20000, 0x30000, 
		  0x40000, 0x50000, 0x60000, 0x70000,
		  0x78000, 0x7A000, 0x7C000,
		  -1
		};

const int FB29LV400B_block_addr[] = 
		{ 0x00000, 0x04000, 0x06000, 0x08000, 
		  0x10000, 0x20000, 0x30000, 0x40000,
		  0x50000, 0x60000, 0x70000,
		  -1
		};
		
const int FB29LV800T_block_addr[] = 
		{ 0x00000, 0x10000, 0x20000, 0x30000, 
		  0x40000, 0x50000, 0x60000, 0x70000,
		  0x80000, 0x90000, 0xA0000, 0xB0000,
		  0xC0000, 0xD0000, 0xE0000, 0xF0000,
		  0xF8000, 0xFA000, 0xFC000,
		  -1
		};

const int FB29LV800B_block_addr[] = 
		{ 0x00000, 0x04000, 0x06000, 0x08000, 
		  0x10000, 0x20000, 0x30000, 0x40000,
		  0x50000, 0x60000, 0x70000, 0x80000,
		  0x90000, 0xA0000, 0xB0000, 0xC0000,
		  0xD0000, 0xE0000, 0xF0000,
		  -1
		};

const int FB29LV160T_block_addr[] = 
		{ 0x000000, 0x010000, 0x020000, 0x030000, 
		  0x040000, 0x050000, 0x060000, 0x070000,
		  0x080000, 0x090000, 0x0A0000, 0x0B0000,
		  0x0C0000, 0x0D0000, 0x0E0000, 0x0F0000,
		  0x100000, 0x110000, 0x120000, 0x130000, 
		  0x140000, 0x150000, 0x160000, 0x170000,
		  0x180000, 0x190000, 0x1A0000, 0x1B0000,
		  0x1C0000, 0x1D0000, 0x1E0000, 0x1F0000,
		  0x1F8000, 0x1FA000, 0x1FC000,
		  -1
		};

const int FB29LV160B_block_addr[] = 
		{ 0x000000, 0x004000, 0x006000, 0x008000,
		  0x010000, 0x020000, 0x030000, 0x040000,
		  0x050000, 0x060000, 0x070000, 0x080000,
		  0x090000, 0x0A0000, 0x0B0000, 0x0C0000,
		  0x0D0000, 0x0E0000, 0x0F0000, 0x100000,
		  0x110000, 0x120000, 0x130000, 0x140000, 
		  0x150000, 0x160000, 0x170000, 0x180000,
		  0x190000, 0x1A0000, 0x1B0000, 0x1C0000,
		  0x1D0000, 0x1E0000, 0x1F0000,
		  -1
		};		

//-----------------------------------------------------------------------------
// 플래시 관련 변수
//-----------------------------------------------------------------------------

int	Flash_PID   = 0;	// 플래시를 구분한다.
int	BlockCount  = 0;
int	FlashSize   = 0;
int	*pBlockAddr = (int *)FB29LV400B_block_addr;


//-----------------------------------------------------------------------------
// 	const, macro 
//-----------------------------------------------------------------------------
#define	RETRY_COUNT			100
#define	WAIT_LOOP_COUNT		200


#define FB29LVx_WriteMemoryShort(adr,data)   	*(unsigned short *)(adr) = ( data & 0xffff ); FB29LVx_Wait();
#define FB29LVx_ReadMemoryShort(adr)         	( *(unsigned short *)(adr) & 0xffff );



//=============================================================================

//-----------------------------------------------------------------------------
//  설 명 : Wait
//  반 환 :
//-----------------------------------------------------------------------------
void	dumy( void )
{
	return;	
}
void	FB29LVx_Wait( void )
{
	int	loop = WAIT_LOOP_COUNT;
	
	while( loop-- ) { dumy(); };
}
//-----------------------------------------------------------------------------
//  설 명 : RESET 시키는 명령
//  반 환 :
//------------------------------------------------------------------------------
void 	FB29LVx_CMD_RESET( Word32 vBase )
{
   	FB29LVx_WriteMemoryShort( vBase, 0xF0 ); 	
}
//------------------------------------------------------------------------------
//  설 명 : ID 를 읽는 명령
//  반 환 :
//-----------------------------------------------------------------------------
void 	FB29LVx_CMD_READID_SETUP( Word32 vBase )
{
    	FB29LVx_WriteMemoryShort( vBase + (0x555<<1), 0xAA );
    	FB29LVx_WriteMemoryShort( vBase + (0x2AA<<1), 0x55 );
    	FB29LVx_WriteMemoryShort( vBase + (0x555<<1), 0x90 );
}
//------------------------------------------------------------------------------
//  설 명 : 데이타를 써 넣는다.
//  반 환 :
//-----------------------------------------------------------------------------
void 	FB29LVx_CMD_WRITE( unsigned long vBase, unsigned long vOffset, Word16 vData)
{
    	FB29LVx_WriteMemoryShort( vBase+(0x555<<1), 0xAA  ); 
    	FB29LVx_WriteMemoryShort( vBase+(0x2AA<<1), 0x55  ); 
    	FB29LVx_WriteMemoryShort( vBase+(0x555<<1), 0xA0  ); 
    	FB29LVx_WriteMemoryShort( vBase+vOffset   , vData ); 
}
//------------------------------------------------------------------------
// 설 명 : 명령 수행 상태의 결과를 얻는다. 
// 반 환 : 
//------------------------------------------------------------------------
int 	FB29LVx_GetStatus( unsigned long vBase )
{
    	unsigned char status, XorStatus;
	
    	while(1)
    	{
 		status     = FB29LVx_ReadMemoryShort( vBase );
 		XorStatus  = status ^ FB29LVx_ReadMemoryShort( vBase );
 		XorStatus &= ((1<<6)|(1<<2));	// Q6, Q2
	 	
 		// 시간초과를 확인한다.
 		if ( status & (1<<5) ) 		// Q5
 		{
 			return FB29LVx_STATUS_TIMEOUT;  
 		}
 		
 		// suspend 
 		if ( status & (1<<7) ) 
 		{
 			return FB29LVx_STATUS_ERSUSP; 
 		}
 		
 		// 변화가 없다면 끝난것이다. 
 		if ( XorStatus == 0 ) 
 		{
 			return FB29LVx_STATUS_READY; 
 		}
    	}

    	return FB29LVx_STATUS_ERROR;                         // 에러가 났다.  
}
//------------------------------------------------------------------------
//  설 명 : Flash의 정보를 얻는다. 
//  반 환 : 
//------------------------------------------------------------------------
int 	FB29LVx_Detect( Word32 vBase )
{
    	unsigned short	mid, pid;
    	char     mid_str[32];

    	FB29LVx_CMD_RESET( vBase ); 
    	FB29LVx_CMD_READID_SETUP(vBase);  
    	
    	// 제조자 회사코드
    	mid = FB29LVx_ReadMemoryShort( vBase + 0      );
    	// 플래시 제품코드
    	pid = FB29LVx_ReadMemoryShort( vBase + (1<<1) );

	FB29LVx_CMD_RESET( vBase ); 

    	switch (mid)
    	{
    	case MXIC_MID : strcpy( mid_str, "MX" );	break;
    	case AMD_MID  : strcpy( mid_str, "AM" );	break;
		case ES_MID   : strcpy( mid_str, "ES" );	break;
    	default       : strcpy( mid_str, "xx" );	break;
    	//default : printf( "> Error Detect Code : %02x\n", mid );
    	//	  return -1; 
    	}
    	
    	switch (pid)
    	{
    	case FB_DID_400T:

			Flash_PID   = pid;	// 플래시를 구분한다.
			BlockCount  = FB_400x_BLOCK_COUNT;
			FlashSize   = 512*1024;
			pBlockAddr  = (int *)FB29LV400T_block_addr;
		
    			printf( "  Detect %s29LV400 (TOP)Flash : %04X\n", mid_str, pid );
    			printf( "  SIZE 4M-BIT [512Kbyte]\n");
			break;

		case FB_DID_400B:

			Flash_PID   = pid;	// 플래시를 구분한다.
			BlockCount  = FB_400x_BLOCK_COUNT;
			FlashSize   = 512*1024;
			pBlockAddr  = (int *)FB29LV400B_block_addr;
		
	    		printf( "  Detect %s29LV400 (BOTTOM)Flash : %04X\n", mid_str, pid );
    			printf( "  SIZE 4M-BIT [512Kbyte]\n");
			break;
			
		case FB_DID_800T:

			Flash_PID   = pid;	// 플래시를 구분한다.
			BlockCount  = FB_800x_BLOCK_COUNT;
			FlashSize   = 1024*1024;
			pBlockAddr  = (int *)FB29LV800T_block_addr;
		
	    		printf( "  Detect %s29LV800T (TOP)Flash : %04X\n", mid_str, pid );
    			printf( "  SIZE 8M-BIT [1Mbyte]\n");
			break;

		case FB_DID_800B:

			Flash_PID   = pid;	// 플래시를 구분한다.
			BlockCount  = FB_800x_BLOCK_COUNT;
			FlashSize   = 1024*1024;
			pBlockAddr  = (int *)FB29LV800B_block_addr;
		
	    		printf( "  Detect %s29LV800T (BOTTOM)Flash : %04X\n", mid_str, pid );
    			printf( "  SIZE 8M-BIT [1Mbyte]\n");
			break;

    	case FB_DID_160T:

			Flash_PID   = pid;	// 플래시를 구분한다.
			BlockCount  = FB_160x_BLOCK_COUNT;
			FlashSize   = 2048*1024;
			pBlockAddr  = (int *)FB29LV160T_block_addr;
		
	    		printf( "  Detect %s29LV160T (TOP)Flash : %04X\n", mid_str, pid );
    			printf( "  SIZE 16M-BIT [2Mbyte]\n");
			break;

    	case FB_DID_160B:

			Flash_PID   = pid;	// 플래시를 구분한다.
			BlockCount  = FB_160x_BLOCK_COUNT;
			FlashSize   = 2048*1024;
			pBlockAddr  = (int *)FB29LV160B_block_addr;
		
	    		printf( "  Detect %s29LV160B (BOTTOM)Flash : %04X\n", mid_str, pid );
    			printf( "  SIZE 16M-BIT [2Mbyte]\n");
			break;    	

    	default:
    		printf( "  Unknown Flash : pid=%04X\n", pid );
		return -1; 
    	} 

	return 0;
}
//------------------------------------------------------------------------------
//  설 명 : 블럭을 지운다.
//  반 환 : 성공 0
//  주 의 : 반드시 블럭의 주소를 준다
//-----------------------------------------------------------------------------
int 	FB29LVx_EraseBlock( unsigned long vBase, unsigned long vBlkAddr )
{
   	Word16	status;
      	int	loop;

      	for (loop=0; loop<RETRY_COUNT; loop++)
      	{
       		FB29LVx_WriteMemoryShort( vBase+(0x555<<1), 0xAA ); 
       		FB29LVx_WriteMemoryShort( vBase+(0x2AA<<1), 0x55 ); 
       		FB29LVx_WriteMemoryShort( vBase+(0x555<<1), 0x80 ); 
       		FB29LVx_WriteMemoryShort( vBase+(0x555<<1), 0xAA ); 
       		FB29LVx_WriteMemoryShort( vBase+(0x2AA<<1), 0x55 ); 
       	
       		FB29LVx_WriteMemoryShort( vBase+vBlkAddr, 0x30 );   

		msleep(100);
       	
   		status = FB29LVx_GetStatus( vBase );
   		if ( status == FB29LVx_STATUS_READY )
   		{
   			FB29LVx_CMD_RESET(vBase); 
   			return 0;  
   		}
   		else
   		{
   			FB29LVx_CMD_RESET(vBase); 	
   		}
       	}
       	
       	printf( "Erase Error [retry count]...\n" ); 		
       	return -1;
}
//------------------------------------------------------------------------
//  설 명 : Flash에 데이타를 쓴다.
//  반 환 : 성공 0
//------------------------------------------------------------------------
int 	FB29LVx_WriteWord( Word32 vBase, Word32 vOffset, Word16 data )
{
      	Word16 	status;
      	int	loop;
      	
      	for (loop=0; loop<RETRY_COUNT; loop++)
      	{
      		FB29LVx_CMD_WRITE( vBase, vOffset, data );  // 데이타를 써 넣는다.
      
   		status = FB29LVx_GetStatus( vBase );
   		if ( status == FB29LVx_STATUS_READY )
   		{
   			FB29LVx_Wait();
   			return 0;  
   		}
   		else
   		{
   			FB29LVx_CMD_RESET(vBase); 	
   		}
	}
	printf( "Write Error [retry count]...\n" ); 
	return -1;	
}
//------------------------------------------------------------------------------
//  설 명 : offset 주소에서 블럭번호를 찾는다.
//  반 환 : 블럭번호
//-----------------------------------------------------------------------------
int	FB29LVx_GetBlockIndex( unsigned long vOffset )
{
	int	blk;
	
	// 플래시 크기밖의 주소를 거른다.
	if ( FlashSize <= vOffset ) return -1;
	
	// 블럭번호를 찾는다.
	for (blk=0; blk<BlockCount; blk++)
	{
		if ( 0 > pBlockAddr[blk] )
		{
			return -1; // 존재하지 않는 주소이다.
		}
		
		if ( 0 < pBlockAddr[blk+1] )
		{
			if ( ( pBlockAddr[blk] <= vOffset ) &&  ( vOffset < pBlockAddr[blk+1] ) )
			{
				break; // 블럭번호를 찾았다.
			}
		}
		else
		{
			break;	// 마지막 블럭
		}
	}
	
	return blk;
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

void Flash_UpdateProgress(const char *title, unsigned long cur, unsigned long max)
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
// 설 명 : 플래시를 찾는다.
// 주 의 : 없음 
//------------------------------------------------------------------------------
int	BootFlash_Init( void )
{
	printf( "Boot Flash Check ......................\n" );
	return FB29LVx_Detect( (Word32)0x00000000 );
}
//------------------------------------------------------------------------------
// 설명 : Flash에 데이타를 써 넣는다. 
// 매계 : dest : 플래쉬 쓰기 시작 주소 
//        src  : 램 읽기 시작 주소  
//        size : 써넣을 크기 
// 반환 : 
// 주의 : 없음 
//------------------------------------------------------------------------------
int 	BootFlash_ProgramEx( Word32  vBase, Word32  vOffset, char *src, int size, int with_erase )
{
	int	s_blk, e_blk;
	int	idx;
	Word16	wData;

	// 지울 블럭의 시작과 끝을 찾는다.
	s_blk = FB29LVx_GetBlockIndex( vOffset );
	e_blk = FB29LVx_GetBlockIndex( vOffset + size );

	if ( ( s_blk < 0 ) || ( e_blk < 0 ) ) 
	{
		printf( "Invalid Address... 0x%08x\n", vBase+vOffset );
		return -1;
	}

	if( with_erase )
	{	
		// 써넣을 공간의 블럭을 지운다. 
		for ( idx=s_blk; idx<=e_blk; idx++ )
		{
			Flash_UpdateProgress( "    Erasing", idx-s_blk+1, e_blk-s_blk+1 );
		
			if ( 0 > FB29LVx_EraseBlock( vBase, pBlockAddr[idx] ) ) return -1;
			msleep(1000);
		}
		Flash_UpdateProgress( "    Erasing", 100, 100 );
		printf( "    Erase : OK\n" );
	}
	
	for ( idx=0; idx<size; idx+=2 )
	{
		if ( 0 == idx % 512 )
			Flash_UpdateProgress("    Writing", idx, size );
		
		wData = (src[idx]&0xff) | ( (src[idx+1]&0xff) << 8 );
		if ( 0 > FB29LVx_WriteWord( vBase, vOffset+idx, wData ) ) return -1;
		
		if ( *(Word16 *)(vBase+vOffset+idx) != wData )
		{
			if ( 0 > FB29LVx_WriteWord( vBase, vOffset+idx, wData ) ) return -1;
		}
	
	}
	Flash_UpdateProgress( "    Writing", 100, 100 );
	printf( "    Write : OK\n" );

	FB29LVx_CMD_RESET( vBase );
	
	return size;
}
//------------------------------------------------------------------------------
// 설명 : Flash에 데이타를 써 넣는다.
// 매계 : dest : 플래쉬 쓰기 시작 주소 
//        src  : 램 읽기 시작 주소
//        size : 써넣을 크기
// 반환 : 
// 주의 : 없음
//------------------------------------------------------------------------------
int 	CopyTo_BootFlashEx( unsigned int dest, unsigned int src, int size, int with_erase )
{
	int	idx;
	unsigned short 	*pdst = (unsigned short *)dest;
	unsigned short 	*psrc = (unsigned short *)src;

	if( with_erase )	BootFlash_Program( 0, dest, (char *)src, size );
	else				BootFlash_ProgramEx( 0, dest, (char *)src, size, 0 );
	
	for(idx=0; idx<size/2; idx++)
	{
		if ( *pdst != *psrc )
  		{
  			printf( "Verify Error %p: %04x/%04x\n", pdst, *pdst, *psrc );
  			return -1;
  		}
  		pdst++; psrc++;
  	}
  		
	return 0;
}
//------------------------------------------------------------------------------
// 설명 : 
// 주의 : 없음
//------------------------------------------------------------------------------
void	SelfCopy_BootLoader( void )
{
	CopyTo_BootFlash( DEFAULT_FLASH_BOOT, DEFAULT_RAM_BOOT_START, DEFAULT_BOOT_SIZE );
}







