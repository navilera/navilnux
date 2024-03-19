//------------------------------------------------------------------------------
// 화일명   : flash_29lvx.h
// 프로젝트 : ezboot_x5
// 설  명   : MX29LVxxxxflash 처리 루틴이다. 
// 
// 작성자   : 오재경 (주)제이닷디앤티 freefrug@falinux.com   -- 푸지
// 작성일   : 2003년 5월 17일
// 수  정   : 2003년 6월 07일
//
// 저작권   : FA리눅스
//
//------------------------------------------------------------------------------

#ifndef _MX29LVx_FLASH_HEADER_
#define _MX29LVx_FLASH_HEADER_

#define	MXIC_MID		0xC2
#define	AMD_MID			0x01
#define	ES_MID			0x4A

#define	FB_DID_400T		0x22B9
#define	FB_DID_400B		0x22BA
#define	FB_400x_BLOCK_COUNT	11
        
#define	FB_DID_800T		0x22DA
#define	FB_DID_800B		0x225B
#define	FB_800x_BLOCK_COUNT	19

#define	FB_DID_160T		0x22C4
#define	FB_DID_160B		0x2249
#define	FB_160x_BLOCK_COUNT	35

#define FB29LVx_STATUS_READY    0       // ready for action 
#define FB29LVx_STATUS_BUSY     1       // operation in progress 
#define FB29LVx_STATUS_ERSUSP   2       // erase suspended 
#define FB29LVx_STATUS_TIMEOUT  3       // operation timed out 
#define FB29LVx_STATUS_ERROR    4       // unclassified but unhappy status 

#define BootFlash_Program( a, b, c, d )  	BootFlash_ProgramEx( a, b, c, d, 1 )
#define CopyTo_BootFlash( a, b, c )			CopyTo_BootFlashEx( a, b, c, 1 )

extern	int BootFlash_Init( void );
extern void SelfCopy_BootLoader( void );

            
#endif //_MX29LVx_FLASH_HEADER_

