#ifndef _NAVIL_LIB
#define _NAVIL_LIB

extern int mysyscall(int, int, int);
extern int mysyscall4(int, int, int, int);

extern int itc_send(int, int);
extern int itc_get(int, int*);
extern int mutex_wait(int);
extern int mutex_release(int);
extern int sem_p(int);
extern int sem_v(int);
extern int open(int, int);
extern int close(int);
extern int read(int, void*, int);
extern int write(int, void*, int);

extern void call_scheduler(void);

int navilnux_itc_send(int, int);
int navilnux_itc_get(int);
int navilnux_mutex_wait(int);
int navilnux_mutex_release(int);
int navilnux_sem_p(int);
int navilnux_sem_v(int);

int navilnux_sleep(int);

void* navilnux_malloc(int);
int navilnux_free(void *);

int navilnux_open(int, int);
int navilnux_close(int);
int navilnux_read(int, void*, int);
int navilnux_write(int, void*, int);


#endif
