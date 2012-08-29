CFLAGS = -O2 -Wall -pedantic -L ../lib
LIBS = -lcontroller-gadget

default: all

all:
	@echo "  CC $$(pwd)/gadget-cli.c"
	@export LD_LIBRARY_PATH=/home/mickey/Development/c-programming/controller-gadget-kmod/lib && $(CC) $(CFLAGS) -o gadget-cli gadget-cli.c $(LIBS)

clean:
	@echo "  CLEAN $$(pwd)"
	@rm -vf gadget-cli
