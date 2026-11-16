ifneq ($(KERNELRELEASE),)
obj-m := hello1.o hello2.o
# дозволяє писати #include "hello1.h" без шляху
ccflags-y += -I$(src)/inc
else
KDIR ?= $(HOME)/repos/linux-stable
ARCH ?= arm
CROSS_COMPILE ?= arm-linux-gnueabihf-

all:
	$(MAKE) -C $(KDIR) M=$(CURDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules

clean:
	$(MAKE) -C $(KDIR) M=$(CURDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) clean
endif
