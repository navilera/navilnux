#ifndef _NAVIL_MSG
#define _NAVIL_MSG

#define MAXMSG 255

typedef struct _navil_free_msg {
    int data;
    int flag;
} Navil_free_msg;

typedef struct _navil_msg_mng {
    Navil_free_msg free_msg_pool[MAXMSG];
    
    void (*init)(void);
    int (*itc_send)(int, int);
    int (*itc_get)(int, int*);
} Navil_msg_mng;

void msg_init(void);
int msg_itc_send(int, int);
int msg_itc_get(int, int*);


#endif
