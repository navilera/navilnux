#include <navilnux.h>

Navil_msg_mng msgmng;
extern Navil_free_task *navilnux_current;

int msg_itc_send(int itcnum, int data)
{
    if(itcnum > ITCEND  || itcnum < ITCSTART){
        return -1;
    }
    msgmng.free_msg_pool[itcnum].data = data;
    msgmng.free_msg_pool[itcnum].flag = 1;

    return itcnum;
}

int msg_itc_get(int itcnum, int *data)
{
    if(itcnum > ITCEND || itcnum < ITCSTART){
        return -1;
    }

    if(msgmng.free_msg_pool[itcnum].flag == 0){
        return -2;
    }

    *data = msgmng.free_msg_pool[itcnum].data;
    msgmng.free_msg_pool[itcnum].flag = 0;
    msgmng.free_msg_pool[itcnum].data = 0;

    return 0;
}

int msg_sem_init(int semnum, int s)
{
    semnum += SEMSTART;

    if(semnum > SEMEND || semnum < SEMSTART){
        return -1;
    }

    msgmng.free_msg_pool[semnum].flag = s;

    return 0;
}

int msg_sem_p(int semnum)
{
    semnum += SEMSTART;

    if(semnum > SEMEND || semnum < SEMSTART){
        return -1;
    }

    if(msgmng.free_msg_pool[semnum].flag <= 0){
        return -2;  
    }

    msgmng.free_msg_pool[semnum].flag--;

    return 0;
}

int msg_sem_v(int semnum)
{
    semnum += SEMSTART;

    if(semnum > SEMEND || semnum < SEMSTART){
        return -1;
    }

    msgmng.free_msg_pool[semnum].flag++;

    return 0;
}

int msg_mutex_wait(int mutexnum)
{
    mutexnum += MUTEXSTART;

    if(mutexnum > MUTEXEND || mutexnum < MUTEXSTART){
        return -1;
    }
    
    if(msgmng.free_msg_pool[mutexnum].flag == 0){
        msgmng.free_msg_pool[mutexnum].flag = 1;
        msgmng.free_msg_pool[mutexnum].data = (unsigned int)navilnux_current;
    }else{
        return -2;
    }

    return 0;    
}

int msg_mutex_release(int mutexnum)
{
    mutexnum += MUTEXSTART;

    if(mutexnum > MUTEXEND || mutexnum < MUTEXSTART){
        return -1;
    }
    if(msgmng.free_msg_pool[mutexnum].data != (unsigned int)navilnux_current){
        return -2;
    }

    msgmng.free_msg_pool[mutexnum].flag = 0;
    msgmng.free_msg_pool[mutexnum].data = 0;
    
    return 0;
}

void msg_init(void)
{
    int i;
    for (i = 0 ; i < MAXMSG ; i++){
        msgmng.free_msg_pool[i].data = 0;
        msgmng.free_msg_pool[i].flag = 0;
    }

    msgmng.init = msg_init;
    msgmng.itc_send = msg_itc_send;
    msgmng.itc_get = msg_itc_get;
    msgmng.sem_init = msg_sem_init;
    msgmng.sem_p = msg_sem_p;
    msgmng.sem_v = msg_sem_v;
    msgmng.mutex_wait = msg_mutex_wait;
    msgmng.mutex_release = msg_mutex_release;
}
