#include <navilnux.h>

extern Navil_task_mng taskmng;
extern Navil_msg_mng msgmng;

void user_task_1(void)
{
    int a, b, c;

    int *ptr_a, *ptr_b, *ptr_c, *ptr_d;

    a = 1;
    b = 2;
    c = a + b;

    ptr_a = (int *)navilnux_malloc(2048);
    printf("Dynamic mem alloc 2K - %p\n", ptr_a);

    ptr_b = (int *)navilnux_malloc(3072);
    printf("Dynamic mem alloc 3K - %p\n", ptr_b);

    navilnux_free(ptr_b);

    ptr_c = (int *)navilnux_malloc(4096);
    printf("Dynamic mem alloc 4K - %p\n", ptr_c);

    ptr_d = (int *)navilnux_malloc(1024);
    printf("Dynamic mem alloc 1K - %p\n", ptr_d);

    while(1){
        printf("TASK1 - a:%p\tb:%p\tc:%p\n", &a, &b, &c);
        navilnux_sleep(1);
    }
}

void user_task_2(void)
{
    int a, b, c;

    a = 1;
    b = 2;
    c = a + b;

    while(1){
        printf("TASK2 - a:%p\tb:%p\tc:%p\n", &a, &b, &c);

        printf("ITC Count is %d\n", a);
        if (a == 3){
            navilnux_itc_send(2, 342);
            a = 1;
            printf("ITC send!!!\n");
        }
        a++;

        navilnux_sleep(1);
    }
}

void user_task_3(void)
{
    int a, b, c;

    a = 1;
    b = 2;
    c = a + b;

    while(1){
        c = navilnux_itc_get(2);
        printf("TASK3 - a:%p\tb:%p\tc:%p\n", &a, &b, &c);
        printf("ITC get!!!! ---> %d\n", c);

        navilnux_sleep(1);
    }
}

void user_task_4(void)
{
    int a, b, c;

    a = 1;
    b = 2;
    c = a + b;

    while(1){
        navilnux_mutex_wait(3);
        printf("TASK4 enter critical section\n");
        printf("TASK4 - a:%p\tb:%p\tc:%p\n", &a, &b, &c);
        navilnux_sleep(4);
        navilnux_mutex_release(3);
        printf("TASK4 out critical section\n");
        navilnux_sleep(3);
    }
}

void user_task_5(void)
{
    int a, b, c;

    a = 1;
    b = 2;
    c = a + b;

    while(1){
        navilnux_mutex_wait(3);
        printf("TASK5 enter critical section\n");
        printf("TASK5 - a:%p\tb:%p\tc:%p\n", &a, &b, &c);
        navilnux_sleep(2);
        navilnux_mutex_release(3);
        printf("TASK5 out critical section\n");
        navilnux_sleep(4);
    }
}

void user_task_6(void)
{
    int a, b, c;

    a = 1;
    b = 2;
    c = a + b;

    while(1){
        navilnux_sem_p(5);
        printf("TASK6 enter critical section SEMAPHORE\n");
        printf("TASK6 - a:%p\tb:%p\tc:%p\n", &a, &b, &c);
        navilnux_sleep(2);
        navilnux_sem_v(5);
        printf("TASK6 out critical section SEMAPHORE\n");
        navilnux_sleep(4);
    }
}

void user_task_7(void)
{
    int a, b, c;

    a = 1;
    b = 2;
    c = a + b;

    while(1){
        navilnux_sem_p(5);
        printf("TASK7 enter critical section SEMAPHORE\n");
        printf("TASK7 - a:%p\tb:%p\tc:%p\n", &a, &b, &c);
        navilnux_sleep(2);
        navilnux_sem_v(5);
        printf("TASK7 out critical section SEMAPHORE\n");
        navilnux_sleep(4);
    }
}

void user_task_8(void)
{
    int a, b, c;

    a = 1;
    b = 2;
    c = a + b;

    while(1){
        navilnux_sem_p(5);
        printf("TASK8 enter critical section SEMAPHORE\n");
        printf("TASK8 - a:%p\tb:%p\tc:%p\n", &a, &b, &c);
        navilnux_sleep(2);
        navilnux_sem_v(5);
        printf("TASK8 out critical section SEMAPHORE\n");
        navilnux_sleep(4);
    }
}

void user_task_9(void)
{
    int a, b, c;
    int led[2] = {0};

    a = 0;
    b = 0;
    c = a + b;

    navilnux_open(MYDRV,O_RDWR);

    while(1){
        navilnux_read(MYDRV, &a, 4);    
        printf("TASK9 - a:%p\tb:%p\tc:%p\n", &a, &b, &c);
        printf("Device Driver Returned %d\n", a);
        
        c = b - 1;
        if(b == 0) c = 3;
        led[0] = c;
        led[1] = LED_OFF;
        navilnux_write(MYDRV, led, 8);

        led[0] = b;
        led[1] = LED_ON;
        navilnux_write(MYDRV, led, 8);
        b++;
        if(b == 4) b = 0;

        navilnux_sleep(5);
    }
    navilnux_close(MYDRV);
}

void navilnux_user(void)
{
    taskmng.create(user_task_1);
    taskmng.create(user_task_2);
    taskmng.create(user_task_3);
    taskmng.create(user_task_4);
    taskmng.create(user_task_5);
    taskmng.create(user_task_6);
    taskmng.create(user_task_7);
    taskmng.create(user_task_8);
    taskmng.create(user_task_9);

    msgmng.sem_init(5,2);
}
