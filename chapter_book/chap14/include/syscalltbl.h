#ifndef _NAVIL_SYS_TBL
#define _NAVIL_SYS_TBL

#define SYSCALLNUM 255

#define SYS_MYSYSCALL   0
#define SYS_MYSYSCALL4  1
#define SYS_ITCSEND     2
#define SYS_ITCGET      3
#define SYS_SEMP        4
#define SYS_SEMV        5
#define SYS_MUTEXTWAIT  6
#define SYS_MUTEXREL    7

#define SYS_CALLSCHED   SYSCALLNUM

#endif
