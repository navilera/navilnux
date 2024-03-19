//------------------------------------------------------------------------------
// 화일명   : flash_cmd.c
// 프로젝트 : ezboot_x5
// 설  명   : NOR, NABD 플래시의 상위 처리 루틴이다.
//            하위루틴을 호출하여 상단의 어플과 매칭한다.
// 
// 작성자   : 오재경 (주)제이닷디앤티 freefrug@falinux.com   -- 푸지
// 작성일   : 2003년 6월 7일
//
// 저작권   : (주)제이닷디앤티 
//
//------------------------------------------------------------------------------

//******************************************************************************
//
// 헤더 정의
//
//******************************************************************************
#include <pxa255.h>
#include <stdio.h>
#include <string.h>
#include <config.h>
#include <mem_map.h>
#include <flash.h>


/* 광역 변수 정의 -------------------------------------------------------------*/

extern TConfig Cfg;		// config.c 에 정의

//------------------------------------------------------------------------------
// 설명 : 이미지를 복사한다.
// 주의 : 
//------------------------------------------------------------------------------
void    CopyImage( void )
{
        printf("Copy Kernel Image .....\n" );
	CopyTo_SDRAM_Kernel ( DEFAULT_RAM_KERNEL_START  );

	if ( ( 'Y' == Cfg.UseRamdisk ) || ( 'y' == Cfg.UseRamdisk ) )
	{
        	printf("Copy Ramdisk Image .....\n" );
		CopyTo_SDRAM_Ramdisk( DEFAULT_RAM_RAMDISK_START );
	}
}

//------------------------------------------------------------------------------
// 설명 : 파티션을 지운다.
// 주의 : 
//------------------------------------------------------------------------------
int    ErasePartiotion( int argc, char **argv)
{
	int	ee = TRUE;
	
	if ( ( argc >= 2 ) && ( toupper(argv[1][0]) == 'E')  ) ee = FALSE;
	
	switch ( argv[0][2] )
	{
	case 'K' : Nand_ErasePartition( PARTITION_NUM_KERNEL ,	ee    ); return 0;
	case 'R' : Nand_ErasePartition( PARTITION_NUM_RAMDISK,  ee    ); return 0;
	case 'A' : Nand_ErasePartition( PARTITION_NUM_APPIMAGE, FALSE ); return 0;
	}
	
	return -1;
}



