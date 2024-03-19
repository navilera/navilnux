#include <navilnux.h>

extern Navil_msg_mng msgmng;

unsigned int navilnux_syscallvec[SYSCALLNUM];

int sys_mysyscall(int a, int b, int c)
{
    printf("My Systemcall - %d , %d , %d\n", a, b, c);
    return 333;
}

int sys_mysyscall4(int a, int b, int c, int d)
{
    printf("My Systemcall4 - %d , %d , %d , %d\n", a, b, c, d);
    return 3413;
}

int sys_itcsend(int itcnum, int data)
{
    return msgmng.itc_send(itcnum, data);
}

int sys_itcget(int itcnum, int *data)
{
    return msgmng.itc_get(itcnum, data);
}


void syscall_init(void)
{
    navilnux_syscallvec[SYS_MYSYSCALL] = (unsigned int)sys_mysyscall; 
    navilnux_syscallvec[SYS_MYSYSCALL4] = (unsigned int)sys_mysyscall4; 
    navilnux_syscallvec[SYS_ITCSEND] = (unsigned int)sys_itcsend; 
    navilnux_syscallvec[SYS_ITCGET] = (unsigned int)sys_itcget; 
}
