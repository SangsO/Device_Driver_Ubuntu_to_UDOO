MOD := hello
obj-m := $(MOD).o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
#	$(MAKE) -c $(KDIR) SUBDIR=$(PWD) modules
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
#	rm -rf *.ko
#커널 오브젝트 ko남기고싶으면 상단 라인 주석처리
	rm -rf *.mod.*
	rm -rf .*.cmd
	rm -rf *.o
	rm -rf $(MOD).mod
	rm -rf modules.order
	rm -rf Module.symvers
