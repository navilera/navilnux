#include <syscalltbl.h>

.global mysyscall
mysyscall:
    swi SYS_MYSYSCALL 
    mov pc, lr

.global mysyscall4
mysyscall4:
    swi SYS_MYSYSCALL4
    mov pc, lr

.global itc_send
itc_send:
    swi SYS_ITCSEND
    mov pc, lr

.global itc_get
itc_get:
    swi SYS_ITCGET
    mov pc, lr

.global mutex_wait
mutex_wait:
    swi SYS_MUTEXTWAIT
    mov pc, lr

.global mutex_release
mutex_release:
    swi SYS_MUTEXREL
    mov pc, lr

.global sem_p
sem_p:
    swi SYS_SEMP
    mov pc, lr

.global sem_v
sem_v:
    swi SYS_SEMV
    mov pc, lr

.global navilnux_malloc
navilnux_malloc:
    swi SYS_MALLOC
    mov pc, lr

.global navilnux_free
navilnux_free:
    swi SYS_FREE
    mov pc, lr

.global call_scheduler
call_scheduler:
    swi SYS_CALLSCHED
    mov pc, lr
