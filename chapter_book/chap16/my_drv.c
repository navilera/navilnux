#include <navilnux.h>

extern Navil_drv_mng drvmng;
extern int (*navilnux_irq_vector[64])(void);

int switch_pushed;

int gpio0_irq_handler(void)
{
    if(switch_pushed) return -1;

    printf("Switch Push!! in Device Driver Layer\n");
    switch_pushed = 1;
    return 0;
}

int mydrv_open(int drvnum, int mode)
{
    navilnux_irq_vector[IRQ8] = gpio0_irq_handler;
    switch_pushed = 0;

    return 0;
}

int mydrv_close(int drvnum)
{
    return 0;
}

int mydrv_read(int drvnum, void *buf, int size)
{
    int *b = (int *)buf;
    if(switch_pushed == 1){
        *b = switch_pushed;
        switch_pushed = 0;
        return 4;
    }else{
        return -2;
    }
}

int mydrv_write(int drvnum, void *buf, int size)
{
    int *b = (int *)buf;
    int n = (int)b[0];
    int s = (int)b[1];
    GPIO_SetLED(n,s);

    return 0;
}

fops mydrv_fops = 
{
    open : mydrv_open,
    read : mydrv_read,
    write : mydrv_write,
    close : mydrv_close,
};

int mydrv_init()
{
    return drvmng.register_drv(MYDRV, "navilnux first drv", &mydrv_fops);
}
