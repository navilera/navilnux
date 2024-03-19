#ifndef _NAVIL_SYS
#define _NAVIL_SYS


#include <syscalltbl.h>


void syscall_init(void);

int sys_mysyscall(int, int, int);
int sys_mysyscall4(int, int, int, int);

int sys_itcsend(int, int);
int sys_itcget(int, int*);

extern void sys_scheduler(void);
#endif
