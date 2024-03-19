//------------------------------------------------------------------------------
// 파 일 명 : time.h
// 프로젝트 : ezboot
// 설    명 : ezboot에서 사용하는 time에 관련된 헤더 
// 작 성 자 : 유영창
// 작 성 일 : 2001년 11월 3일
// 수 정 일 : 
//------------------------------------------------------------------------------

#ifndef _TIME_HEADER_
#define _TIME_HEADER_

#define TICKS_PER_SECOND 3686400
#define TICKS_PER_TICK   36864


void		SetWatchdog( int msec );
void  		TimerInit(void);             	// 타이머 초기화 
unsigned int  	TimerGetTime(void);       		// 1/TICKS_PER_SECOND 의 시간 값을 반환한다. 
int   		TimerDetectOverflow(void);
void  		TimerClearOverflow(void);
void  		msleep(unsigned int msec);   	// 밀리세컨드 단위시간동안 대기 한다. 
void        sleep(unsigned int sec);

extern void 	ReloadTimer( unsigned char bTimer, unsigned int msec);
extern int  	TimeOverflow( unsigned char bTimer );
extern void 	FreeTimer( unsigned char bTimer );

#endif //_TIME_HEADER_

