#include <navilnux.h>

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

int main(void)
{
    os_timer_init();
    os_timer_start();

    irq_enable();

    while(1){
        __asm__("swi 77");
        msleep(1000);
    }

    return 0;
}	
