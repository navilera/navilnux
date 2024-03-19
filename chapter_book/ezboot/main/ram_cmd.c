//------------------------------------------------------------------------------
// 화일명 : ram_cmd.c
// 설  명 : ezBoot의 메모리에 관련 명령 처리 루틴이다. 
// 
// 작성자 : 유영창 (주)제이닷디엔티 frog@falinux.com
// 작성일 : 2001년 10월 8일
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


//******************************************************************************
//
// 광역 변수 정의
//
//******************************************************************************

static unsigned long LastRamDumpAddress = 0x000;

//******************************************************************************
//
// 함수 정의
//
//******************************************************************************

//------------------------------------------------------------------------------
// 설명 : 명령 리스트를 보여 준다. 
// 매계 : address : 보여줄 시작 주소 
//        size    : 크기  
// 반환 : 없음
// 주의 : 없음
//------------------------------------------------------------------------------
void MemoryHexDump( void *address, int size )
{
    int lp1,lp2,lp3;
    unsigned char *ptrData;


    ptrData = address;

    for( lp1 = 0; lp1 < size*4; lp1 += 16 )
    {
        // 주소 표시 
        printf( "%04X-%04X :",  (((unsigned int)ptrData)>>16)&0xFFFF,
                                 ((unsigned int)ptrData)     &0xFFFF  );
              

        // 총 4번 표시 
        for( lp2 = 0; lp2 < 4; lp2++ )
        {
            for( lp3 = 0; lp3 < 4; lp3++ )
            {
                 printf( "%02X", ptrData[ lp2 * 4 + lp3 ] & 0xFF );
            } 
            printf( " " );
        }
        printf( "  " );
        for( lp3 = 0; lp3 < 16; lp3++ )
        {
            if      ( ptrData[lp3] < ' ' ) printf( "." );
            else if ( ptrData[lp3] > '~' ) printf( "." ); 
            else    printf("%c", ptrData[lp3]  );
        } 

        printf( "\n" );
        ptrData += 16;
    }

    LastRamDumpAddress = (unsigned long)ptrData;

}
//------------------------------------------------------------------------------
// 설명 : 명령 리스트를 보여 준다. 
// 매계 : argc    : 토큰 갯수 
//        argv    : 분리된 토큰 문자열 주소가 담겨질 배열 
// 반환 : 에러 -1  정상 0
// 주의 : 없음 
//------------------------------------------------------------------------------
int RamDump(int argc, char **argv)
{
    int           size;

    switch( argc )
    {
      case  1 : // 명령만 썼을때 
                MemoryHexDump( (void * )LastRamDumpAddress, 16 ); 
                break;
      case  2 : // 시작 주소만 주어 졌을때 
                LastRamDumpAddress = strtoul( argv[1], NULL, 0);
                MemoryHexDump( (void * ) LastRamDumpAddress, 16 ); 
                break;
      case  3 : // 시작 주소와 카운트가 있을때 
                LastRamDumpAddress = strtoul( argv[1], NULL, 0);
                size               = strtoul( argv[2], NULL, 0);
                if( size < 0 ) size *= -1;
                
                MemoryHexDump( (void * ) LastRamDumpAddress, size );   
                break;
      default : printf( "Argument Count Error!\n");
                return -1;
    } 

  return 0;
}

//------------------------------------------------------------------------------
// 설명 : 메모리에 데이타 쓰기 ( 1 Byte )
// 매계 : argc    : 토큰 갯수 
//        argv    : 분리된 토큰 문자열 주소가 담겨질 배열 
// 반환 : 에러 -1  정상 0
// 주의 : 없음 
//------------------------------------------------------------------------------
int RamWriteChar (int argc, char **argv)
{

    unsigned char *ptrTo;
    int            lp;

    if( argc < 3 ) 
    {
        printf( "Argument Count Error!\n");
        return -1; 
    }

    // 써넣을 주소를 얻는다. 

    ptrTo = (unsigned char *)strtoul( argv[1], NULL, 0);

    for( lp = 2; lp < argc; lp++ )
       ptrTo[ lp - 2 ] = strtoul( argv[lp], NULL, 0);
    
   return 0;
}
//------------------------------------------------------------------------------
// 설명 : 메모리에 데이타 쓰기 ( 2 Byte ) 
// 매계 : argc    : 토큰 갯수 
//        argv    : 분리된 토큰 문자열 주소가 담겨질 배열 
// 반환 : 에러 -1  정상 0
// 주의 : 없음 
//------------------------------------------------------------------------------
int RamWriteWord (int argc, char **argv)
{
    unsigned short *ptrTo;
    int            lp;

    if( argc < 3 ) 
    {
        printf( "Argument Count Error!\n");
        return -1; 
    }

    // 써넣을 주소를 얻는다. 

    ptrTo = (unsigned short *)strtoul( argv[1], NULL, 0);

    for( lp = 2; lp < argc; lp++ )
       ptrTo[ lp - 2 ] = strtoul( argv[lp], NULL, 0);
   return 0;
}
//------------------------------------------------------------------------------
// 설명 : 메모리에 데이타 쓰기 ( 3 Byte ) 
// 매계 : argc    : 토큰 갯수 
//        argv    : 분리된 토큰 문자열 주소가 담겨질 배열 
// 반환 : 에러 -1  정상 0
// 주의 : 없음 
//------------------------------------------------------------------------------
int RamWriteLong (int argc, char **argv)
{

    unsigned long *ptrTo;
    int            lp;

    if( argc < 3 ) 
    {
        printf( "Argument Count Error!\n");
        return -1; 
    }

    // 써넣을 주소를 얻는다. 

    ptrTo = (unsigned long *)strtoul( argv[1], NULL, 0);

    for( lp = 2; lp < argc; lp++ )
       ptrTo[ lp - 2 ] = strtoul( argv[lp], NULL, 0);

   return 0;
}

