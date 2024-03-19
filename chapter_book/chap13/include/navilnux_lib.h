#ifndef _NAVIL_LIB
#define _NAVIL_LIB

extern int mysyscall(int, int, int);
extern int mysyscall4(int, int, int, int);

extern int itc_send(int, int);
extern int itc_get(int, int*);

extern void call_scheduler(void);


int navilnux_itc_send(int, int);
int navilnux_itc_get(int);

#endif
