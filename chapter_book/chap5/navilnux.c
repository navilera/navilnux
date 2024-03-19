#include <navilnux.h>

void swiHandler(unsigned int syscallnum)
{
    printf("system call %d\n", syscallnum);
}

void irqHandler(void)
{

}

int main(void)
{
    while(1){
        __asm__("swi 77");
        msleep(1000);
    }

    return 0;
}	
