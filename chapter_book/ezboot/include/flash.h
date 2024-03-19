//------------------------------------------------------------------------------
// 화일명   : flash.h
// 프로젝트 : ezboot_x5
// 설  명   : NOR, NAND 플래시의 상위 처리 루틴이다.
//            하위루틴을 호출하여 상단의 어플과 매칭한다.
// 
// 작성자   : 오재경 (주)제이닷디앤티 freefrug@falinux.com   -- 푸지
// 작성일   : 2003년 6월 7일
//
// 저작권   : (주)제이닷디앤티 
//
//------------------------------------------------------------------------------

#ifndef _FLASH_HEADER_
#define _FLASH_HEADER_

#include <flash_29lvx.h>
#include <nand.h>

extern void CopyImage( void );
extern int  ErasePartiotion( int argc, char **argv);

#endif // _FLASH_HEADER_
