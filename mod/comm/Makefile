ifeq ($(KERNELRELEASE),)
	KDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
	PRINT_INC = $(PWD)/../include/
	EXTRA_CFLAGS += -I $(PRINT_INC)

modules:
	$(MAKE) $(EXTRA_CFLAGS) -C $(KDIR) M=$(PWD) modules

modules_install:
	$(MAKE) -C $(KDIR) M=$(PWD) modules_install

clean:
	rm -rf *o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions *.symvers *.order

.PHONY: modules modules_install clean
else
	# called from kernel build system: just declare what our modules are
	obj-m := add_sub.o
endif


