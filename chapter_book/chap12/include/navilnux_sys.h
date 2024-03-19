#ifndef _NAVIL_SYS
#define _NAVIL_SYS

#include <syscalltbl.h>

#define SYSCALLNUM 255

void syscall_init(void);


int sys_mysyscall(int, int, int);
int sys_mysyscall4(int, int, int, int);

#endif
