#include <navilnux.h>

extern Navil_task_mng taskmng;

void user_task_1(void)
{
    int a, b, c;

    a = 1;
    b = 2;
    c = a + b;

    while(1){
        printf("TASK1 - a:%p\tb:%p\tc:%p\n", &a, &b, &c);
        msleep(1000);
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
        msleep(1000);
    }
}

void user_task_3(void)
{
    int a, b, c;

    a = 1;
    b = 2;
    c = a + b;

    while(1){
        printf("TASK3 - a:%p\tb:%p\tc:%p\n", &a, &b, &c);
        msleep(1000);
    }
}

void navilnux_user(void)
{
    taskmng.create(user_task_1);
    taskmng.create(user_task_2);
    taskmng.create(user_task_3);
}
