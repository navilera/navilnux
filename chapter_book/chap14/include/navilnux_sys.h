#ifndef _NAVIL_SYS
#define _NAVIL_SYS

#include <syscalltbl.h>


void syscall_init(void);

int sys_mysyscall(int, int, int);
int sys_mysyscall4(int, int, int, int);

int sys_itcsend(int, int);
int sys_itcget(int, int*);

int sys_semp(int);
int sys_semv(int);

int sys_mutexwait(int);
int sys_mutexrelease(int);

extern void sys_scheduler(void);
#endif
