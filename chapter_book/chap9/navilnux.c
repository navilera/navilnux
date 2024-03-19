#include <navilnux.h>

extern Navil_mem_mng memmng;
extern Navil_task_mng taskmng;

void swiHandler(unsigned int syscallnum)
{
    printf("system call %d\n", syscallnum);
}

void irqHandler(void)
{
    if( (ICIP&(1<<27)) != 0 ){
        OSSR = OSSR_M1;
        OSMR1 = OSCR + 3686400;
        printf("Timer Interrupt!!!\n");
    } 
}

void os_timer_init(void)
{
    ICCR = 0x01;

    ICMR |= (1 << 27);      
    ICLR &= ~(1 << 27);    

    OSCR = 0;
    OSMR1 = OSCR + 3686400; 

    OSSR = OSSR_M1;
}

void os_timer_start(void)
{
    OIER |= (1<<1);
    OSSR = OSSR_M1;
}

void irq_enable(void)
{
    __asm__("msr    cpsr_c,#0x40|0x13");
}

void irq_disable(void)
{
    __asm__("msr    cpsr_c,#0xc0|0x13");
}

void navilnux_init(void)
{
    mem_init();
    task_init();

    os_timer_init();
    os_timer_start();
}

int main(void)
{
    navilnux_init();
    navilnux_user();

    irq_enable();

    int i;
    for(i = 0 ; i <= taskmng.max_task_id ; i++){
        printf("TCB : TASK%d - init PC(%p) \t init SP(%p)\n", i+1,
                    taskmng.free_task_pool[i].context_pc,
                    taskmng.free_task_pool[i].context_sp);
    }

    printf("REAL func TASK1 : %p\n", user_task_1);
    printf("REAL func TASK2 : %p\n", user_task_2);
    printf("REAL func TASK3 : %p\n", user_task_3);

    while(1){
        msleep(1000);
    }

    return 0;
}	
