#ifndef _KERNEL_H_
#define _KERNEL_H_

#include <pxa255.h>
#include <time.h>
#include <gpio.h>
#include <stdio.h>
#include <string.h>

#include <navilnux_memory.h>
#include <navilnux_task.h>
#include <navilnux_lib.h>
#include <navilnux_sys.h>
#include <navilnux_msg.h>
#include <navilnux_drv.h>

#include <navilnux_user.h>

void navilnux_init(void);
void navilnux_user(void);

#define IRQNUM  64
#define IRQ0    0
#define IRQ1    1
#define IRQ2    2
#define IRQ3    3
#define IRQ4    4
#define IRQ5    5
#define IRQ6    6
#define IRQ7    7
#define IRQ8    8
// 여기에 IRQ핸들러 등록번호를 추가 해 주세요
#define IRQ63   63

#endif
