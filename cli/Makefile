CFLAGS = -O2 -Wall -pedantic

default: all

all: clean
	@echo "  CC $$(pwd)/gadget-cli.c"
	@$(CC) $(CFLAGS) -o gadget-cli gadget-cli.c

clean:
	@echo "  CLEAN $$(pwd)"
	@rm -f gadget-cli
