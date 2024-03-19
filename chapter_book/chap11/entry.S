.globl _ram_entry
_ram_entry:
    b   kernel_init 
	b	_ram_entry
    b   navilnux_swiHandler
    b   navilnux_irqHandler


#define svc_stack   0xa0300000
#define irq_stack   0xa0380000
#define sys_stack   0xa0400000

.global kernel_init
kernel_init:
    msr     cpsr_c,#0xc0|0x13    //SVC mode
    ldr     r0,=svc_stack
    sub     sp,r0,#4

    msr     cpsr_c,#0xc0|0x12    //IRQ mode
    ldr     r0,=irq_stack
    sub     sp,r0,#4

    msr     cpsr_c,#0xc0|0x1f    //SYSTEM mode
    ldr     r0,=sys_stack
    sub     sp,r0,#4

    msr     cpsr_c,#0xc0|0x13

    bl      main
    b       _ram_entry

.global navilnux_swiHandler
navilnux_swiHandler:
    msr     cpsr_c,#0xc0|0x13


    ldr     sp,=navilnux_current
    ldr     sp,[sp]

    add     sp,sp,#4
    stmia   sp!, {r0-r12}^
    stmia   sp!, {sp,lr}^
    stmia   sp!, {lr}

    sub     sp,sp,#68
    mrs     r1,spsr
    stmia   sp!, {r1}

    ldr     sp,=svc_stack

    ldr     r10,[lr,#-4]
    bic     r10,r10,#0xff000000
    mov     r0,r10

    bl      swiHandler

    ldr     sp,=navilnux_next
    ldr     sp,[sp]

    ldmia   sp!, {r1}
    msr     spsr_cxsf, r1
    ldmia   sp!, {r0-r12}^
    ldmia   sp!, {r13,r14}^
    ldmia   sp!, {pc}^


.global navilnux_irqHandler
navilnux_irqHandler:    
    msr     cpsr_c,#0xc0|0x12

    ldr     sp,=navilnux_current
    ldr     sp,[sp]

    sub     lr,lr,#4
    add     sp,sp,#4
    stmia   sp!, {r0-r12}^
    stmia   sp!, {sp,lr}^
    stmia   sp!, {lr}

    sub     sp,sp,#68
    mrs     r1,spsr
    stmia   sp!, {r1}

    ldr     sp,=irq_stack

    bl      irqHandler

    ldr     sp,=navilnux_next
    ldr     sp,[sp]

    ldmia   sp!, {r1}
    msr     spsr_cxsf, r1
    ldmia   sp!, {r0-r12}^
    ldmia   sp!, {r13,r14}^


    ldmia   sp!, {pc}^
