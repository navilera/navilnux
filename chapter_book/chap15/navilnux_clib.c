#include <navilnux.h>

extern Navil_free_task *navilnux_current;
extern unsigned int navilnux_time_tick;

int navilnux_itc_send(int itcnum, int data)
{
    return itc_send(itcnum, data);
}

int navilnux_itc_get(int itcnum)
{
    int ret_value = 0;
    int data = 0;
    while(1){
        ret_value = itc_get(itcnum, &data);
        if(ret_value == 0){
            return data;
        }else if(ret_value == -1){
            return ret_value;
        }else{
            call_scheduler();
        }
    }
}

int navilnux_mutex_wait(int mutexnum)
{
    int ret_value = 0;
    while(1){
        ret_value = mutex_wait(mutexnum);
        if(ret_value == 0){
            return 0;
        }else if(ret_value == -1){
            return -1;
        }else{
            call_scheduler();
        }
    }
}

int navilnux_mutex_release(int mutexnum)
{
    return mutex_release(mutexnum);
}

int navilnux_sem_p(int semnum)
{
    int ret_value = 0;
    while(1){
        ret_value = sem_p(semnum);
        if(ret_value == 0){
            return 0;
        }else if(ret_value == -1){
            return -1;
        }else{
            call_scheduler();
        }
    }
}

int navilnux_sem_v(int semnum)
{
    return sem_v(semnum);
}

int navilnux_sleep(int sec)
{
    while(1){
        if(navilnux_current->sleep_end_tick == 0){
            navilnux_current->sleep_end_tick = navilnux_time_tick + sec;
        }else if(navilnux_current->sleep_end_tick <= navilnux_time_tick){
            navilnux_current->sleep_end_tick = 0;
            return 0;
        }else{
            call_scheduler();
        }
    }
    return 0;
}
