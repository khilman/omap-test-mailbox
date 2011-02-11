KERNELDIR=${HOME}/work/kernel/omap/dev

obj-m = mbox_test.o
mbox_test-objs = main.o

all:
	make ${MAKE_OPTS} -C $(KERNELDIR) SUBDIRS=$(PWD) modules

clean:
	$(RM) -r *.o *.ko *.mod.c .*.cmd .tmp_versions *.symvers modules.order
