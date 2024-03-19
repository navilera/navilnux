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
    __asm__("swi 77");
    return 0;
}	
