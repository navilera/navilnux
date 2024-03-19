.globl _ram_entry
_ram_entry:
    bl  main
	b	_ram_entry
    b   navilnux_swiHandler
    b   navilnux_irqHandler


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
    bl      irqHandler
