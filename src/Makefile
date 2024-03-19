CC = arm-none-eabi-gcc
LD = arm-none-eabi-gcc
OC = arm-none-eabi-objcopy

CFLAGS = -nostdinc -I. -I./include 
CFLAGS += -c -g -std=c11 -mthumb-interwork -mcpu=xscale -march=armv5te -fno-builtin
LDFLAGS = -nostartfiles -nostdlib -nodefaultlibs -static -lgcc

OCFLAGS = -O binary -R .note -R .comment -S

CFILES = entry.S navilnux.c navilnux_memory.c navilnux_task.c navilnux_user.c navilnux_lib.S navilnux_clib.c navilnux_sys.c navilnux_msg.c navilnux_drv.c my_drv.c
HFILES = include/navilnux.h include/navilnux_memory.h include/navilnux_task.h include/navilnux_user.h include/navilnux_lib.h include/navilnux_sys.h include/syscalltbl.h include/navilnux_msg.h include/navilnux_drv.h

all: $(CFILES) $(HFILES)
	$(CC) -c $(CFLAGS) -o entry.o entry.S
	$(CC) -c $(CFLAGS) -o gpio.o gpio.c
	$(CC) -c $(CFLAGS) -o time.o time.c
	$(CC) -c $(CFLAGS) -o vsprintf.o vsprintf.c
	$(CC) -c $(CFLAGS) -o printf.o printf.c
	$(CC) -c $(CFLAGS) -o string.o string.c
	$(CC) -c $(CFLAGS) -o serial.o serial.c
	$(CC) -c $(CFLAGS) -o navilnux.o navilnux.c
	$(CC) -c $(CFLAGS) -o navilnux_memory.o navilnux_memory.c
	$(CC) -c $(CFLAGS) -o navilnux_task.o navilnux_task.c
	$(CC) -c $(CFLAGS) -o navilnux_user.o navilnux_user.c
	$(CC) -c $(CFLAGS) -o navilnux_lib.o navilnux_lib.S 
	$(CC) -c $(CFLAGS) -o navilnux_clib.o navilnux_clib.c 
	$(CC) -c $(CFLAGS) -o navilnux_sys.o navilnux_sys.c
	$(CC) -c $(CFLAGS) -o navilnux_msg.o navilnux_msg.c
	$(CC) -c $(CFLAGS) -o navilnux_drv.o navilnux_drv.c
	$(CC) -c $(CFLAGS) -o drvs.o my_drv.c
	$(LD) -n -T ./main-ld-script -o navilnux_elf entry.o gpio.o time.o vsprintf.o printf.o string.o serial.o navilnux.o navilnux_memory.o navilnux_task.o navilnux_msg.o navilnux_sys.o navilnux_lib.o navilnux_clib.o navilnux_user.o navilnux_drv.o drvs.o $(LDFLAGS)
	$(OC) $(OCFLAGS) navilnux_elf navilnux_img
	$(CC) -c $(CFLAGS) -o serial.o serial.c -D IN_GUMSTIX
	$(LD) -n -T ./main-ld-script -o navilnux_gum_elf entry.o gpio.o time.o vsprintf.o printf.o string.o serial.o navilnux.o navilnux_memory.o navilnux_task.o navilnux_msg.o navilnux_sys.o navilnux_lib.o navilnux_clib.o navilnux_user.o navilnux_drv.o drvs.o $(LDFLAGS)
	$(OC) $(OCFLAGS) navilnux_gum_elf navilnux_gum_img


clean:
	rm *.o
	rm navilnux_elf
	rm navilnux_img
	rm navilnux_gum_elf
	rm navilnux_gum_img
