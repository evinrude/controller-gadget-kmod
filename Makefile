obj-m += controller-gadget-kmod.o

all:
	@make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	@make -C cli all

clean:
	@make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	@make -C cli clean
