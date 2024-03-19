#include <syscalltbl.h>

.global mysyscall
mysyscall:
    swi SYS_MYSYSCALL 
    mov pc, lr

.global mysyscall4
mysyscall4:
    swi SYS_MYSYSCALL4
    mov pc, lr
