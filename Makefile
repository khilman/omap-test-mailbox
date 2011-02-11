KERNELDIR=${HOME}/work/kernel/omap/dev

obj-m = mytest.o
mytest-objs = main.o

all:
	make ${MAKE_OPTS} -C $(KERNELDIR) SUBDIRS=$(PWD) modules

mytest.tgz: clean $(mytest-objs:.o=.c) Makefile
	(cd ..; tar --exclude CVS --exclude mytest.tgz -zcvf mytest/mytest.tgz ./mytest)

clean:
	$(RM) -r *.o *.ko *.mod.c .*.cmd .tmp_versions *.symvers modules.order *~ mytest.tgz


