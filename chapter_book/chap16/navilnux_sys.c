#include <navilnux.h>

extern Navil_msg_mng msgmng;
extern Navil_mem_mng memmng;
extern Navil_drv_mng drvmng;

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

int sys_mutexwait(int mutexnum)
{
    return msgmng.mutex_wait(mutexnum);
}

int sys_mutexrelease(int mutexnum)
{
    return msgmng.mutex_release(mutexnum);
}

int sys_semp(int semnum)
{
    return msgmng.sem_p(semnum);
}

int sys_semv(int semnum)
{
    return msgmng.sem_v(semnum);
}

void *sys_malloc(int size)
{
    return memmng.malloc(size);
}

int sys_free(void *addr)
{
    return memmng.free(addr);
}

int sys_open(int drvnum, int mode)
{
    if(drvnum > DRVLIMIT || drvnum < 0){
        return -1;
    }

    if(drvmng.free_drv_pool[drvnum].usecount < 0){
        return -1;
    }

    drvmng.free_drv_pool[drvnum].usecount++;

    return drvmng.free_drv_pool[drvnum].navil_fops->open(drvnum, mode); 
}

int sys_close(int drvnum)
{
    if(drvnum > DRVLIMIT || drvnum < 0){
        return -1;
    }

    drvmng.free_drv_pool[drvnum].usecount--;

    return drvmng.free_drv_pool[drvnum].navil_fops->close(drvnum);
}

int sys_read(int drvnum, void *buf, int size)
{
    if(drvnum > DRVLIMIT || drvnum < 0){
        return -1;
    }

    return drvmng.free_drv_pool[drvnum].navil_fops->read(drvnum, buf, size);
}

int sys_write(int drvnum, void *buf, int size)
{
     if(drvnum > DRVLIMIT || drvnum < 0){
        return -1;
    }

    return drvmng.free_drv_pool[drvnum].navil_fops->write(drvnum, buf, size);
}

void syscall_init(void)
{
    navilnux_syscallvec[SYS_MYSYSCALL] = (unsigned int)sys_mysyscall; 
    navilnux_syscallvec[SYS_MYSYSCALL4] = (unsigned int)sys_mysyscall4; 
    navilnux_syscallvec[SYS_ITCSEND] = (unsigned int)sys_itcsend; 
    navilnux_syscallvec[SYS_ITCGET] = (unsigned int)sys_itcget; 
    navilnux_syscallvec[SYS_MUTEXTWAIT] = (unsigned int)sys_mutexwait;
    navilnux_syscallvec[SYS_MUTEXREL] = (unsigned int)sys_mutexrelease;
    navilnux_syscallvec[SYS_SEMP] = (unsigned int)sys_semp;
    navilnux_syscallvec[SYS_SEMV] = (unsigned int)sys_semv;
    navilnux_syscallvec[SYS_MALLOC] = (unsigned int)sys_malloc;
    navilnux_syscallvec[SYS_FREE] = (unsigned int)sys_free;
    navilnux_syscallvec[SYS_OPEN] = (unsigned int)sys_open;
    navilnux_syscallvec[SYS_CLOSE] = (unsigned int)sys_close;
    navilnux_syscallvec[SYS_READ] = (unsigned int)sys_read;
    navilnux_syscallvec[SYS_WRITE] = (unsigned int)sys_write;
}
