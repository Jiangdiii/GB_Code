.PHONY : clean all
KERN_VERSION := $(shell uname -r)
KERNDIR := /lib/modules/$(KERN_VERSION)/build
PWD := $(shell pwd)
obj-m := charDev.o
all:
	make -C $(KERNDIR) M=$(PWD)  modules
clean:
	make -C $(KERNDIR) M=$(PWD) clean