obj-m += controller-gadget-kmod.o
UDEV_RULE_PATH = /etc/udev/rules.d

default: all

.PHONY: lib cli daemon firmware
all: kernmodule lib cli daemon firmware

kernmodule:
	@make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

lib:
	@make -C lib all

cli:
	@make -C cli all

daemon:
	@make -C daemon all

firmware:
	@make -C usbfirmware/USBSprinklerController.X all


clean:
	@make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	@make -C lib clean
	@make -C cli clean
	@make -C daemon clean
	@make -C usbfirmware/USBSprinklerController.X all

install:
	@echo "  INSTALLING controller-gadget-kmod.ko"
	cp -vf controller-gadget-kmod.ko /lib/modules/$(shell uname -r)/kernel/drivers/misc/
	cp -vf 99-controller.rules $(UDEV_RULE_PATH)
	udevadm control --reload-rules
	udevadm trigger
	depmod
	modprobe controller-gadget-kmod

uninstall:
	@echo "  UNINSTALLING controller-gadget-kmod.ko"
	rm -fv /lib/modules/$(shell uname -r)/kernel/drivers/misc/controller-gadget-kmod.ko
	rm -fv $(UDEV_RULE_PATH)/99-controller.rules
	udevadm control --reload-rules
	udevadm trigger
	rmmod controller-gadget-kmod.ko
	depmod
