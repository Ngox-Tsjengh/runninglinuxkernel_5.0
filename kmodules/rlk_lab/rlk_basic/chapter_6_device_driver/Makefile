BASEINCLUDE ?= /lib/modules/`uname -r`/build

mydemo-objs := mydemodrv.o

obj-m := mydemo.o

all:
	$(MAKE) -C $(BASEINCLUDE) M=$(PWD) modules;

clean:
	$(MAKE) -C $(BASEINCLUDE) M=$(PWD) clean;
	rm -f *.ko *.cmd *.o test;
