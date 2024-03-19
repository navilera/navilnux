#include <navilnux.h>

int main(void)
{
    int i = 0;

    printf("hello world\n");
    msleep(1000);

    while(1){
        GPIO_SetLED( i, LED_OFF );
        msleep(500);
        GPIO_SetLED( i, LED_ON );
        msleep(500);
        i++;
        if(i >= 4) i = 0;
    }
    return 0;
}	
