obj-m := test.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
SYMBOL_INC =$(obj)/../include
EXTRA_CFLAGS += -I $(SYMBOL_INC)
KBUILD_EXTRA_SYMBOLS=$(obj)/../print/Module.symvers

modules:
	cp ../Module.symvers $(PWD)
	$(MAKE) -C $(KDIR) M=$(PWD) modules

modules_install:
	$(MAKE) -C $(KDIR) M=$(PWD) modules_install

clean:
	rm -rf *o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions modules.order *.symvers
.PHONY: modules modules_install clean

