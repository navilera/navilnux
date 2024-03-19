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


.global call_scheduler
call_scheduler:
    swi SYS_CALLSCHED
    mov pc, lr
