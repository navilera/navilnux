//------------------------------------------------------------------------------
// 화일명 : printf.c
// 설  명 : ezBoot에서 사용되는 printf 함수 루틴이다. 
// 
// 작성자 : 유영창 (주)제이닷디엔티 frog@falinux.com
// 작성일 : 2001년 11월 4일
// 저작권 : (주)제이닷디엔티 
// 주  의 : 
//------------------------------------------------------------------------------
#include <pxa255.h>
#include <time.h>
#include <gpio.h>
#include <serial.h>
#include <stdio.h>
#include <string.h>

//------------------------------------------------------------------------------
// 
// 광역 변수 
// 
//------------------------------------------------------------------------------
#define HIS_MAX               8

char HisBuff[HIS_MAX][32] = { {0,}, };
int  HisCount               = 0;
int  HisIndex               = 0;

//------------------------------------------------------------------------------
// 설명 : 한문자를 출력한다. 
// 매계 : 없음 
// 반환 : 없음 
// 주의 : 캐리지 리턴의 변화를 처리한다. 
//------------------------------------------------------------------------------
int putc(char c)
{
	SerialOutChar_CheckCR( c );
	return 1;
}
//------------------------------------------------------------------------------
// 설명 : 한문자를 출력한다. 
// 매계 : 없음 
// 반환 : 없음 
// 주의 : 없음 
//------------------------------------------------------------------------------
int putx(char c)
{
	SerialOutChar( c );
	return 1;
}
//------------------------------------------------------------------------------
// 설명 : 포맷형 문자열을 출력한다. 
// 매계 : 없음 
// 반환 : 없음 
// 주의 : 없음 
//------------------------------------------------------------------------------
int printf(const char *fmt, ...)
{
	char buffer[1024];
	va_list ap;
	int len;

	va_start(ap, fmt);
	len = vsprintf(buffer, fmt, ap);
	va_end(ap);

        SerialOutStr_CheckCR(buffer, len);

	return len;
}

//------------------------------------------------------------------------------
// 설명 : 한문자의 입력을 얻는다. 
// 매계 : 없음 
// 반환 : 없음 
// 주의 : 없음 
//------------------------------------------------------------------------------
int getc(void)
{
	int lp = 0;

	// 한문자 대기 
	while( !SerialIsReadyChar() ) 
    {
                lp++; 
    }
	return SerialIsGetChar() & 0xFF;
}
//------------------------------------------------------------------------------
// 설명 : 문자열 입력을 받는다. ( CR이 입력될때까지 )
// 매계 : 없음 
// 반환 : 없음 
// 주의 : 없음 
//------------------------------------------------------------------------------
int gets(char *s)
{
     int cnt = 0;
     char  c;

     while((c = getc()) != CR)
     {
          if(c != BS) 
          {  
               cnt++;
               *s++ = c;
               printf("%c",c );
          }
          else 
          {
               if(cnt > 0) 
               { cnt--; *s-- = ' '; 
                 printf("\b \b");  
               } 
               
          }
     }
     *s = 0;
   
     return(cnt);
}

//------------------------------------------------------------------------------
// 설명 : 히스토리를 추가 한다. 
// 매계 : s : 설정할 버퍼 주소 
// 반환 : 현재 버퍼에 저장된 히스토리 수
// 주의 : 
//------------------------------------------------------------------------------
int his_append(char *s)
{
    int loop;

    // 이미 같은 문자열이 있으면 무시한다. 
    for( loop = 0; loop < HIS_MAX; loop++ )
    {
      if( strcmp( s, HisBuff[loop] ) == 0 ) 
      {
          HisIndex = 0;
          if( HisCount ) HisIndex = HisCount-1;
          return HisCount;
      } 
    } 

    // 새로운 내용을 히스토리 버퍼에 추가 한다. 
    if( HisCount < HIS_MAX )
    {
        strcpy( HisBuff[HisCount], s );
        HisCount++;
    }
    else
    {   
        for( loop = 1; loop < HIS_MAX ; loop++ )
        {
            strcpy( HisBuff[loop-1], HisBuff[loop] );
        }
        strcpy( HisBuff[HIS_MAX-1], s );
        HisIndex = HisCount-1;
    }

    HisIndex = 0; 
    if( HisCount ) HisIndex = HisCount-1;

    return HisCount;
}
//------------------------------------------------------------------------------
// 설명 : 히스토리를 현재 버퍼에 설정한다. 
// 매계 : s : 설정할 버퍼 
//        index : 설정할 내용  
// 반환 : 없음 
// 주의 : 
//------------------------------------------------------------------------------
int his_set(char *s, int index )
{
    int loop;
    int len;

    len = strlen( s );

    // 현재 설정 문자열을 지운다. 
    for( loop = 0; loop < len; loop++ )
    {
      printf("\b \b");
    }
     
    // 버퍼 내용을 설정한다. 
    strcpy( s, HisBuff[index] ); 
    printf( "%s", s );

    return index;
}

//------------------------------------------------------------------------------
// 설명 : 문자열 입력을 받는다. ( CR이 입력될때까지 )
// 매계 : 없음 
// 반환 : 없음 
// 주의 : 히스토리 기능을 추가 하였다. 
//------------------------------------------------------------------------------
int gets_his(char *s)
{
     int cnt = 0;
     char  *fp;
     char  c;

     fp = s;
     
     while( 1 )
     {
          c = getc();

          if( c == CR )
          {
              *s = 0;
              if( strlen( fp ) ) his_append( fp );
              break;
          } 

          switch( c )
          {
          case 0x1a  : // ^Z
                       if( HisIndex >= 0 )
                       { 
                           his_set( fp, HisIndex );
                           if( HisIndex ) HisIndex--;
                           if( HisIndex >= HisCount ) HisIndex = HisCount -1;
                           cnt = strlen( fp );
                           s = fp + cnt; 
                       } 
                       break;
          case 0x18  : // ^X
                       if( HisIndex < HisCount )
                       { 
                           his_set( fp, HisIndex );
                           HisIndex++;
                           cnt = strlen( fp );
                           s = fp + cnt; 
                       }  
                       break;
      
          case BS    : if( cnt > 0 )
                       { 
                         cnt--; *s-- = ' '; 
                         printf("\b \b");  
                       } 
                       break;
          default    : cnt++;
                       *s++ = c;
                       printf("%c",c );
                       break;
          }
     }
   
     return(cnt);

}

//------------------------------------------------------------------------------
// 설명 : 한문자의 입력을 얻는다. 
// 매계 : 비교키, msec
// 반환 : 실패 -1  성공 0
// 주의 : 시간의 제한이 있다. 
//------------------------------------------------------------------------------
#define halfsec ((TICKS_PER_SECOND/1000)*500)
int     getc_timed( char cmpKey, int mTimeOver )
{
	unsigned long ticks, end;
	int     toggle = 0;
	char    key;
	
	if ( mTimeOver == 0 ) return -1;
	
	// 틱을 계산한다.
	ticks   = (TICKS_PER_SECOND/1000) * mTimeOver;
	end     = OSCR + ticks;
        
        OSSR  = OSSR_M0 | OSSR_M1;      // 타이머0,1 오버플로우비트 클리어
        OSMR0 = end;                    // 타이머0 오버플로우가 발생할 틱
        OSMR1 = OSCR + halfsec;
        // 타이머0 인터럽트 활성화
        OIER = OSSR_M0 | OSSR_M1;

        // 타이머0 오버플로우비트가 있을때 까지 대기
        while( 0 == (OSSR&OSSR_M0) )
        {
                if ( SerialIsReadyChar() )
                {
                        key = SerialIsGetChar()&0xFF;
                        if ( cmpKey     == key ) return 0;
                        if ( (char)0x0d == key ) return -1;
                }
                
                // 0.5 초마다 깜박인다.
                if ( OSSR&OSSR_M1 )
                {
                        OSSR  = OSSR_M1;      // 타이머1 오버플로우비트 클리어
                        OSMR1 = OSCR + halfsec;
                        
                        toggle ++;
                        printf( "." );
                }        
        }
        
        // 모든 타이머 인터럽트 비활성화
        OIER = 0;

        return -1;
}






