CC = arm-linux-gcc
LD = arm-linux-ld
OC = arm-linux-objcopy

CFLAGS    = -nostdinc -I. -I./include -I fs/include
CFLAGS   += -Wall -Wstrict-prototypes -Wno-trigraphs -O2
CFLAGS   += -fno-strict-aliasing -fno-common -pipe -mapcs-32 
CFLAGS   += -mcpu=xscale -mshort-load-bytes -msoft-float -fno-builtin

LDFLAGS   = -static -nostdlib -nostartfiles -nodefaultlibs -p -X -T ./main-ld-script
                
OCFLAGS = -O binary -R .note -R .comment -S

all: navilnux.c
	$(CC) -c $(CFLAGS) -o entry.o entry.S
	$(CC) -c $(CFLAGS) -o gpio.o gpio.c
	$(CC) -c $(CFLAGS) -o time.o time.c
	$(CC) -c $(CFLAGS) -o vsprintf.o vsprintf.c
	$(CC) -c $(CFLAGS) -o printf.o printf.c
	$(CC) -c $(CFLAGS) -o string.o string.c
	$(CC) -c $(CFLAGS) -o serial.o serial.c
	$(CC) -c $(CFLAGS) -o lib1funcs.o lib1funcs.S
	$(CC) -c $(CFLAGS) -o navilnux.o navilnux.c
	$(LD) $(LDFLAGS) -o navilnux_elf entry.o gpio.o time.o vsprintf.o printf.o string.o serial.o lib1funcs.o navilnux.o 
	$(OC) $(OCFLAGS) navilnux_elf navilnux_img
	$(CC) -c $(CFLAGS) -o serial.o serial.c -D IN_GUMSTIX
	$(LD) $(LDFLAGS) -o navilnux_gum_elf entry.o gpio.o time.o vsprintf.o printf.o string.o serial.o lib1funcs.o navilnux.o 
	$(OC) $(OCFLAGS) navilnux_gum_elf navilnux_gum_img

clean:
	rm *.o
	rm navilnux_elf
	rm navilnux_img
	rm navilnux_gum_elf
	rm navilnux_gum_img
