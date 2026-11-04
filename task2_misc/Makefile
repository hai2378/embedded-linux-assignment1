obj-m += counter_driver.o

KDIR := $(HOME)/embedded_lab1/kernel/linux-5.15
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- M=$(PWD) clean
