CFLAGS = -O2 -Wall -pedantic -L ../lib
CDEBUG = -g $(CFLAGS)
LIBS = -lcontroller-gadget

default: all

all:
	@echo "  CC $$(pwd)/gadget-cli.c"
	@export LD_LIBRARY_PATH=i$(shell pwd)/../lib && $(CC) $(CFLAGS) -o gadget-cli gadget-cli.c $(LIBS)

debug:
	@echo "  DEBUG $$(pwd)/gadget-cli.c"
	@export LD_LIBRARY_PATH=i$(shell pwd)/../lib && $(CC) $(CDEBUG) -o gadget-cli gadget-cli.c $(LIBS)

clean:
	@echo "  CLEAN $$(pwd)"
	@rm -vf gadget-cli
