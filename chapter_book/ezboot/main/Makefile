#
#  ezBoot 메인 메이크 화일 
#

SRCS = entry.S lib1funcs.S main.c serial.c time.c gpio.c flash_29lvx.c \
       cs8900.c arp_cmd.c net.c tftp_cmd.c go_cmd.c config.c help.c\
       zmodem.c ram_cmd.c zmodem_cmd.c flash_cmd.c nand.c \
       printf.c vsprintf.c vscanf.c string.c 

OBJS = entry.o lib1funcs.o main.o serial.o time.o gpio.o flash_29lvx.o \
       cs8900.o arp_cmd.o net.o tftp_cmd.o go_cmd.o config.o help.o\
       zmodem.o ram_cmd.o zmodem_cmd.o flash_cmd.o nand.o \
       printf.o vsprintf.o vscanf.o string.o 

TARGET      = main_org
PRE_TARGET  = main-elf32

%.o:%.c
	@echo "Compiling $< ..."
	$(CC) -c $(CFLAGS) -o $@ $<

#
# Compilation target for C++ files
#
%.o:%.S
	@echo "Assembler compiling $< ..."
	$(CC) -c $(CFLAGS)  -o $@ $<


all : $(PRE_TARGET)
	$(OC) $(OCFLAGS) $(PRE_TARGET) $(TARGET)

$(PRE_TARGET) : $(OBJS)
	$(LD) $(MAIN_LDFLAGS) -o $@ $(OBJS) 

dep :
	$(CC) -M $(INCLUDES) $(SRCS) > .depend

clean:
	rm -f *.o
	rm -f *.s
	rm -f $(PRE_TARGET)
	rm -f $(TARGET)
	
distclean: clean
	rm -rf .depend	

new : 
	$(MAKE) clean 
	$(MAKE) 

.depend: dep

ifeq (.depend,$(wildcard .depend))
include .depend
endif
