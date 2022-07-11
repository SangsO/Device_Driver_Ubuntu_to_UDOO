MOD := hello
obj-m := $(MOD).o

CROSS = ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-
#KDIR := /lib/modules/$(shell uname -r)/build
KDIR := /home/ubuntu/udoo_linux_bsp/kernel/3.14.1
PWD := $(shell pwd)

default:
#	$(MAKE) -c $(KDIR) SUBDIR=$(PWD) modules
	$(MAKE) -C $(KDIR) M=$(PWD) modules $(CROSS)
	cp $(MOD).ko /srv/nfs
clean:
#	rm -rf *.ko
#커널 오브젝트 ko남기고싶으면 상단 라인 주석처리
	rm -rf *.mod.*
	rm -rf .*.cmd
	rm -rf *.o
	rm -rf $(MOD).mod
	rm -rf modules.order
	rm -rf Module.symvers
