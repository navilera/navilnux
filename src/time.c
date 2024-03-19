//------------------------------------------------------------------------------
// 화일명 : time.c
// 설  명 : ezBoot의 Timer 제어 루틴이다. 
// 
// 작성자 : 유영창 (주)제이닷디엔티 frog@falinux.com
// 수  정 : 2003-06-07	오재경 (주)제이닷디엔티 freefrug@falinux.com  -- 푸지
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
#include <time.h>

//******************************************************************************
//
// 광역 변수 정의
//
//******************************************************************************
static int numOverflows;
//******************************************************************************
//
// 함수 정의
//
//******************************************************************************

/*
void sleep(unsigned int sec)
{
    while(1){
        if(taskMng.taskRunning->sleep_start == 0){
            taskMng.taskRunning->sleep_start = memos_time_tick;
            taskMng.taskRunning->sleep_end = memos_time_tick + sec;
        }else{
            if(taskMng.taskRunning->sleep_end <= memos_time_tick){
                taskMng.taskRunning->sleep_start = 0;
                taskMng.taskRunning->sleep_end = 0;
                return;
            }
            contextSwitch();
        }
    }
}
*/

//------------------------------------------------------------------------------
// 설명 : 워치독 설정
// 매계 : msec 단위 시간
// 반환 : 없음 
// 주의 : 없음 
//------------------------------------------------------------------------------
void	SetWatchdog( int msec )
{
	OWER  = OWER_WME;
	OSSR  = OSSR_M3;
	OSMR3 = OSCR + (36864*(msec/10));
}

//------------------------------------------------------------------------------
// 설명 : 타이머를 초기화 한다. 
// 매계 : 없음 
// 반환 : 없음 
// 주의 : 없음 
//------------------------------------------------------------------------------
void TimerInit(void)
{
	// 타이머 카운터값을 초기화 한다.
	OSCR = 0;

    // 타이머에 의해서 발생하는 모든 인터럽트를 금지한다. 
	OIER = 0;		

	// 타이머 카운터값이 0이 될때까지 기다린다. 
	while(OSCR == 0); // WatchDoc() ;

	// 매치 레지스터0를 0으로 설정하여 매치값을 0으로 한다. 
	OSMR0 = 0;

	// 매치 레지스터를 매치 상태값을 초기화 한다. 
	OSSR = OSSR_M0;

	// 타이머 오버 플로어 감시 변수를 초기화 한다. 
	numOverflows = 0;
}

//------------------------------------------------------------------------------
// 설명 : 타이머의 값을 얻어 온다. 
// 매계 : 없음 
// 반환 : 없음 
// 주의 : 없음 
//------------------------------------------------------------------------------
unsigned int  TimerGetTime(void)
{
	return ((unsigned int) OSCR);
}
//------------------------------------------------------------------------------
// 설명 : 오버 플로워를 검출한다. 
// 매계 : 없음 
// 반환 : 없음 
// 주의 : 없음 
//------------------------------------------------------------------------------
int TimerDetectOverflow(void)
{
	return(OSSR & OSSR_M0);
}
//------------------------------------------------------------------------------
// 설명 : 오버 플로워 상태를 지운다. 
// 매계 : 없음 
// 반환 : 없음 
// 주의 : 없음 
//------------------------------------------------------------------------------
void TimerClearOverflow(void)
{
	if( TimerDetectOverflow() ) numOverflows++;

	OSSR = OSSR_M0;
}

//------------------------------------------------------------------------------
// 설명 : 밀리 세컨드 동안 지연을 갇는다.
// 매계 : 없음 
// 반환 : 없음 
// 주의 : 1160 초 이상의 지연은 지원하지 않는다.
//------------------------------------------------------------------------------
void 	msleep(unsigned int msec)
{
        ReloadTimer( 0, msec );
        while( 0 == TimeOverflow(0) );
        FreeTimer( 0 );
}


void    ReloadTimer( unsigned char bTimer, unsigned int msec )
{
        unsigned long reg;
        
        bTimer &= 0x03;
        reg = (1<<bTimer);
        
        // 타이머 오버플로우비트 클리어
        OSSR  = reg;
        
        // 타이머 오버플로우가 발생할 틱
        switch( bTimer )
        {
        	case 0 : OSMR0 = OSCR + (TICKS_PER_SECOND/1000)*msec; break;
        	case 1 : OSMR1 = OSCR + (TICKS_PER_SECOND/1000)*msec; break;
        	case 2 : OSMR2 = OSCR + (TICKS_PER_SECOND/1000)*msec; break;
        	case 3 : OSMR3 = OSCR + (TICKS_PER_SECOND/1000)*msec; break;
        }
        
        
        // 타이머 인터럽트 활성화
        OIER = OIER | reg;
}

int     TimeOverflow( unsigned char bTimer )
{
        unsigned long reg;
        
        reg = 1 << (bTimer&0x03);
        return (OSSR&reg);
}

void    FreeTimer( unsigned char bTimer )
{
        unsigned long mask;

        mask = 1 << (bTimer&0x03);
        OIER = OIER&(~mask);       // 모든 타이머 인터럽트 비활성화
}

