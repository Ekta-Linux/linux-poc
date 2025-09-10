obj-m  := vdevs_platform.o vdevs_drv.o

KDIR= /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) M=$(PWD)  modules
	@gcc vdevs_test.c -o vdev_test
	@gcc ioctl_vdevs_test.c -o ioctl_vdevs_test
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	@rm -rf $(wildcard *.o *.ko *.mo* Mo* *.order a.out vdev_test ioctl_vdevs_test)
	

