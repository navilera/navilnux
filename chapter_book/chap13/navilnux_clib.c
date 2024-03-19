#include <navilnux.h>

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
