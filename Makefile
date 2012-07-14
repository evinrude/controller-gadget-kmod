obj-m += controller-gadget-kmod.o
GCC := gcc

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	$(GCC) -Wall -o ./cli/gadget-cli ./cli/gadget-cli.c

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f ./cli/gadget-cli
