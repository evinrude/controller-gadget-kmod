obj-m += controller-gadget-kmod.o

default: all

.PHONY: lib cli daemon
all: kernmodule lib cli daemon

kernmodule:
	@make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

lib:
	@make -C lib all

cli:
	@make -C cli all

daemon:
	@make -C daemon all


clean:
	@make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	@make -C lib clean
	@make -C cli clean
	@make -C daemon clean

install:
	@echo "  INSMOD controller-gadget-kmod.ko"
	@insmod controller-gadget-kmod.ko
	@make -C lib install

uninstall:
	@echo "  RMMOD controller-gadget-kmod.ko"
	@rmmod controller-gadget-kmod.ko
	@make -C lib uninstall
