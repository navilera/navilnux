#
#  ezboot 메이크 화일 
#  
#  이 메이크 화일은 등록된 디렉토리에 있는 모든 것을 make 수행한다.
#

#
# 이 화일에 선언된 모든 변수를 서브 메이크 화일에서 사용할수 있게 한다.
#
.EXPORT_ALL_VARIABLES:

#
# 컴파일 할 대상이 있는 디렉토리를 적은다. 
#

DIRS = 	start main
TOPDIR	:= $(shell if [ "$$PWD" != "" ]; then echo $$PWD; else pwd; fi)
TINY_DIR = eztiny

#
# 컴파일 하기 위한 기본 환경 내용이 들어 있다. 
#

CC = arm-linux-gcc
LD = arm-linux-ld
OC = arm-linux-objcopy

INCLUDES = -I. -I$(TOPDIR)/include 

CFLAGS    = -nostdinc $(INCLUDES) 
CFLAGS   += -Wall -Wstrict-prototypes -Wno-trigraphs -O2
CFLAGS   += -fno-strict-aliasing -fno-common -pipe -mapcs-32 
CFLAGS   += -mcpu=xscale -mshort-load-bytes -msoft-float -fno-builtin

START_LDFLAGS  = -p -X -T ./start-ld-script
MAIN_LDFLAGS   = -static -nostdlib -nostartfiles -nodefaultlibs -p -X -T ./main-ld-script
                
OCFLAGS = -O binary -R .note -R .comment -S

BOOT_IMAGE = ezboot.x5
TINY_IMAGE = eztiny.x5


#
# ezboot 이미지를 만든다. 
#
all:
	for i in $(DIRS) ; do make -C $$i || exit $? ; done
	dd if=start/start_org of=image/$(BOOT_IMAGE) bs=1k conv=sync
	dd if=main/main_org of=image/$(BOOT_IMAGE) bs=1k seek=2
	
#	cp image/$(BOOT_IMAGE) /tftpboot/$(BOOT_IMAGE)
#	cp image/$(BOOT_IMAGE) /nfsfg/$(BOOT_IMAGE)
#	chmod 777 /nfsfg/$(BOOT_IMAGE)

tiny:
	make -C $(TINY_DIR)
	dd if=$(TINY_DIR)/$(TINY_IMAGE) of=image/$(TINY_IMAGE) bs=128 conv=sync
	cp image/$(TINY_IMAGE) /tftpboot/$(TINY_IMAGE)
#	cp image/$(TINY_IMAGE) /nfsfg/$(TINY_IMAGE)



#
# 쓸데없는 화일을 지운다.
#
clean:
	for i in $(DIRS) ; do make -C $$i clean; done
	rm -f image/$(BOOT_IMAGE)

tiny_clean:
	make -C $(TINY_DIR) clean
	rm -f image/$(TINY_IMAGE)	

#
# 쏘스 참조를 자동으로 만든다. 
#
dep:
	for i in $(DIRS) ; do make -C $$i; done

tiny_dep:
	make -C $(TINY_DIR) dep


