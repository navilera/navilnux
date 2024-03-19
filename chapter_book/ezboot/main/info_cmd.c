//------------------------------------------------------------------------------
// 화일명 : info_cmd.c
// 설  명 : 보드의 상태를 표출한다. 
// 
// 작성자 : 유영창 (주)제이닷디엔티 frog@falinux.com
// 작성일 : 2003년 05월 20일
//          
// 저작권 : (주)제이닷디엔티 
// 주  의 : 
//------------------------------------------------------------------------------

//******************************************************************************
//
// 헤더 정의
//
//******************************************************************************
#include <pxa255.h>
#include <config.h>
#include <serial.h>
#include <time.h>
#include <stdio.h>
#include <gpio.h>


#define	EZ_X5_RAM_BOOT_END	0xA0F20000      // 부트 영역의 끝

//******************************************************************************
//
// 광역 변수 정의
//
//******************************************************************************

//******************************************************************************
//
// 함수 정의
//
//******************************************************************************

//------------------------------------------------------------------------------
// 설명 : 시스템 상태를 표출한다. 
// 매계 : argc    : 토큰 갯수 
//        argv    : 분리된 토큰 문자열 주소가 담겨질 배열 
// 반환 : 에러 -1  정상 0
// 주의 : 없음 
//------------------------------------------------------------------------------
int     Info_cmd(int argc, char **argv)
{
       // 메모리 환경 상태를 표출한다. 
       printf( "-[MEMORY INFO]-------------------------------------------------------------\n" );
       printf( " FLASH ROM START        : 0x%08X\n",  DEFAULT_FLASH_BOOT           );
       printf( " BOOT START             : 0x%08X\n",  DEFAULT_FLASH_BOOT           );
       printf( " BOOT PARAM             : 0x%08X  - 0x%08X \n",  
                                                      DEFAULT_FLASH_BOOT
                                                    + DEFAULT_BOOT_SIZE,
                                                      DEFAULT_FLASH_BOOT
                                                    + DEFAULT_BOOT_SIZE
                                                    + DEFAULT_BOOT_PARAM_SIZE
                                                                )  
       printf( "\n" );
       printf( " CS8900 BASE            : 0x%08X\n",  DEFAULT_nCS_CS8900_BASE      );
       printf( " NAND   BASE            : 0x%08X\n",  DEFAULT_nCS_NAND_BASE        );
       printf( " MK712  BASE            : 0x%08X\n",  DEFAULT_nCS_MK712_BASE       );
       printf( "\n" );
       printf( " SDRAM START            : 0x%08X\n" , BANK0_START                  ); 
       printf( " KERNEL ZERO PAGE       : 0x%08X\n",  DEFAULT_RAM_KERNEL_ZERO_PAGE );
       printf( " KERNEL START           : 0x%08X\n",  DEFAULT_RAM_KERNEL_START     );
       printf( " RAMDISK START          : 0x%08X\n",  DEFAULT_RAM_RAMDISK_START    );
       printf( " BOOTLOADER START       : 0x%08X\n",  DEFAULT_RAM_BOOT_START       );
       printf( " STACK                  : 0x%08X\n",  EZ_X5_RAM_BOOT_END           );
       printf( " WORKING AREA START     : 0x%08X\n",  DEFAULT_RAM_WORK_START       );
       printf( "\n" );
       printf( "-[MEMORY INFO]-------------------------------------------------------------\n" );
       printf( " BOOT FLASH SIZE        : 512 K byte\n" );
       printf( " SDRAM SIZE             :  64 M Byte\n" );
       printf( " NAND FLASH SIZE        :  64 M Byte\n" );
       printf( " KERNEL  SIZE           : 0x%08X\n",                     );
       printf( " RAMDISK SIZE           : 0x%08X\n",                     );
       printf( "---------------------------------------------------------------------------\n" );
       
        
       return 0;
}

