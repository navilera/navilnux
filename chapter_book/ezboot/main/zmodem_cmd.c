//------------------------------------------------------------------------------
// 화일명 : ram_cmd.c
// 설  명 : ezBoot의 메모리에 관련 명령 처리 루틴이다. 
// 
// 작성자 : 유영창 (주)제이닷디엔티 frog@falinux.com
//          오재경 (주)제이닷디앤티 freefrug@falinux.com --푸지
// 작성일 : 2001년 10월 8일
// 수  정 : 2003년 6월 05일
// 저작권 : (주)제이닷디엔티 
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
#include <zmodem.h>

#include <mem_map.h>
#include <config.h>
#include <flash.h>

//******************************************************************************
//
// 광역 변수 정의
//
//******************************************************************************
// config.c 에 정의
extern TConfig Cfg;

//******************************************************************************
//
// 함수 정의
//
//******************************************************************************


//------------------------------------------------------------------------------
// 설명 : ZMODEM으로 데이타 받기 
// 매계 : argc    : 토큰 갯수 
//        argv    : 분리된 토큰 문자열 주소가 담겨질 배열 
// 반환 : 에러 -1  정상 0
// 주의 : 없음 
//------------------------------------------------------------------------------
int     ZModem_Memory(int argc, char **argv)
{
        unsigned long  To;
        int            size;

        if( argc != 2 ) 
        {
                printf( "Argument Count Error!\n");
                return -1; 
        }

        // ZModem으로 데이타를 받는다.
        size = rz( "test.txt",  DEFAULT_RAM_WORK_START );	
        if( size < 0 ) 
        {
                printf( "ZModem Resive Error!\n" );
        }
        else
        {
                printf( "ZModem Resive Size %d\n", size );
        }


        // 써넣을 주소를 얻는다. 
        // 주소에 대한 검증은 하지 않았다.
        To = (unsigned long)strtoul( argv[1], NULL, 0);

        if ( 'F' == argv[0][1] )
        {
                printf( "아직 하지 않았다\n" );
                // To &= 0x0fffffff;
                // 플래시 영역에 쓴다. 
		// Flash_WriteFull( To, DEFAULT_RAM_WORK_START, size );
        }
        else
        {
                memcpy( (void *)To, (void *)DEFAULT_RAM_WORK_START, size );
        }

        return 0;
}
//------------------------------------------------------------------------------
// 설명 : ZMODEM으로 데이타를 받은후 부트에 써 넣기 
// 매계 : argc    : 토큰 갯수 
//        argv    : 분리된 토큰 문자열 주소가 담겨질 배열 
// 반환 : 에러 -1  정상 0
// 주의 : 없음 
//------------------------------------------------------------------------------
int     ZModem_FlashBoot(int argc, char **argv)
{
        int     size;

        // ZModem으로 데이타를 받는다.
        size = rz( "test.txt",  DEFAULT_RAM_WORK_START );
        if( size < 0 ) 
        {
                printf( "ZModem Resive Error!\n" );
                return -1; 
        }

        printf( "ZModem Resive Size %d\n", size );

	    size = (size+1) & (DEFAULT_BOOT_SIZE-2);
       	// 부트 영역에 츨래쉬를 쓴다. 
       	CopyTo_BootFlash( DEFAULT_FLASH_BOOT, DEFAULT_RAM_WORK_START, size );

        return 0;
}
//------------------------------------------------------------------------------
// 설명 : ZMODEM으로 데이타를 Kernel 영역에 넣는다. 
// 매계 : argc    : 토큰 갯수 
//        argv    : 분리된 토큰 문자열 주소가 담겨질 배열 
// 반환 : 에러 -1  정상 0
// 주의 : 없음 
//------------------------------------------------------------------------------
int ZModem_Kernel(int argc, char **argv)
{
        int     size;

        // ZModem으로 데이타를 받는다.
        size = rz( "test.txt",  DEFAULT_RAM_WORK_START );  //DEFAULT_RAM_KERNEL_START
        if( size < 0 ) 
        {
                printf( "ZModem Resive Error!\n" );
                return -1; 
        }
        printf( "ZModem Resive Size %d\n", size );

        if ( 'F' == argv[0][1] ) // 플래시 영역에 쓴다. 
        {
		CopyTo_NandFlash_Kernel( DEFAULT_RAM_WORK_START, size );
        }
        else
        {
                memcpy( (void *)(DEFAULT_RAM_KERNEL_START), (void *)DEFAULT_RAM_WORK_START, size );
        }

        return 0;
}   
//------------------------------------------------------------------------------
// 설명 : ZMODEM으로 데이타를 Ram Disk 영역에 넣는다. 
// 매계 : argc    : 토큰 갯수 
//        argv    : 분리된 토큰 문자열 주소가 담겨질 배열 
// 반환 : 에러 -1  정상 0
// 주의 : 없음 
//------------------------------------------------------------------------------
int ZModem_RamDisk(int argc, char **argv)
{
        int     size;

        // ZModem으로 데이타를 받는다.
        size = rz( "test.txt",  DEFAULT_RAM_WORK_START );  //DEFAULT_RAM_RAMDISK_START
        if( size < 0 ) 
        {
                printf( "ZModem Resive Error!\n" );
                return -1; 
        }
        printf( "ZModem Resive Size %d\n", size );

        // 환경변수에 크기를 적는다.
        if ( 'F' == argv[0][1] )
        {
                // 플래시 영역에 쓴다. 
                CopyTo_NandFlash_Ramdisk( DEFAULT_RAM_WORK_START, size );
        }
        else
        {
                memcpy( (void *)(DEFAULT_RAM_RAMDISK_START), (void *)DEFAULT_RAM_WORK_START, size );
        }

        return 0;
}   

