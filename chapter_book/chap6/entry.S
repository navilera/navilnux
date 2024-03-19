.globl _ram_entry
_ram_entry:
    b   kernel_init 
	b	_ram_entry
    b   navilnux_swiHandler
    b   navilnux_irqHandler


#define irq_stack   0xa0380000

.global kernel_init
kernel_init:
    msr     cpsr_c,#0xc0|0x12    //IRQ mode
    ldr     r0,=irq_stack
    sub     sp,r0,#4

    msr     cpsr_c,#0xc0|0x13

    bl      main
    b       _ram_entry

.global navilnux_swiHandler
navilnux_swiHandler:
    stmfd   sp!,{r0-r12,r14}
    mrs     r1,spsr
    stmfd   sp!,{r1}
    ldr     r10,[lr,#-4]
    bic     r10,r10,#0xff000000
    mov     r0,r10
    bl      swiHandler
    ldmfd   sp!,{r1}
    msr     spsr_cxsf,r1
    ldmfd   sp!,{r0-r12,pc}^

.global navilnux_irqHandler
navilnux_irqHandler:    
    sub     lr,lr,#4
    stmfd   sp!,{lr}
    stmfd   sp!,{r0-r14}^
    mrs     r1,spsr
    stmfd   sp!,{r1}
    bl      irqHandler
    ldmfd   sp!,{r1}
    msr     spsr_cxsf,r1
    ldmfd   sp!,{r0-r14}^
    ldmfd   sp!,{pc}^
