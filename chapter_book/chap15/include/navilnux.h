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

#include <navilnux_user.h>

void navilnux_init(void);
void navilnux_user(void);

#endif
