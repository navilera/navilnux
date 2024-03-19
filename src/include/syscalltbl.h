#ifndef _NAVIL_SYS_TBL
#define _NAVIL_SYS_TBL

#define SYSCALLNUM 255

#define SYS_MYSYSCALL   0
#define SYS_MYSYSCALL4  1
#define SYS_ITCSEND     2
#define SYS_ITCGET      3
#define SYS_MUTEXTWAIT  4
#define SYS_MUTEXREL    5
#define SYS_SEMP        6
#define SYS_SEMV        7
#define SYS_MALLOC      8
#define SYS_FREE        9
#define SYS_OPEN        10
#define SYS_CLOSE       11
#define SYS_READ        12
#define SYS_WRITE       13

#define SYS_CALLSCHED   SYSCALLNUM

#endif
