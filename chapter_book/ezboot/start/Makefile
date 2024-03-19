#
#  초기화 관련 ezBoot 메이크 화일 
#


SRCS       = start.S gpio.S memory.S
OBJS       = $(SRCS:.S=.o)

TARGET     = start_org
PRE_TARGET = start-elf32

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
	$(OC) $(OCFLAGS)  $(PRE_TARGET) $(TARGET)


$(PRE_TARGET) : $(OBJS)
	$(LD) $(START_LDFLAGS) -o $@  $(OBJS)

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
