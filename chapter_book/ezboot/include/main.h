//------------------------------------------------------------------------------
// 파 일 명 : main.h
// 프로젝트 : ezboot
// 설    명 : ezboot에서 사용하는 main에 관련된 헤더 
// 작 성 자 : 유영창
// 작 성 일 : 2001년 11월 3일
// 수 정 일 : 
// 주    의 : 
//------------------------------------------------------------------------------

#ifndef _MAIN_HEADER_
#define _MAIN_HEADER_

typedef struct {
        char *CmdStr;                     	// 명령 문자열
	int (*func)(int argc, char **argv);	// 수행 함수
//        char *DocLine;    			// 한라인 도움말  
//        char *DocText;   			// 자세한 도움말 
} TCommand;

#endif //_MAIN_HEADER_

