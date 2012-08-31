SOURCES = libcontroller-gadget.c
OBJECTS = libcontroller-gadget.o
HEADERS = libcontroller-gadget.h
LIB_DIR = /usr/lib
LIB_NAME = libcontroller-gadget.so
LIB_VERSION = 1
LD = ld
INCLUDES = -I.

default: all

all:
	@echo "  CC $(shell pwd)/$(SOURCES)"
	@$(CC) $(INCLUDES) -fPIC -c $(SOURCES)
	@echo "  LD $(shell pwd)/$(LIB_NAME)"
	@$(LD) -shared -soname=$(LIB_NAME).$(LIB_VERSION) $(OBJECTS) -o $(LIB_NAME).$(LIB_VERSION).0 -lc
	@ln -s $(LIB_NAME).$(LIB_VERSION).0 $(LIB_NAME)
	@ln -s $(LIB_NAME).$(LIB_VERSION).0 $(LIB_NAME).$(LIB_VERSION)

install:
	@cp -v $(LIB_NAME).$(LIB_VERSION).0 $(LIB_DIR)/
	@cp -v $(LIB_NAME) $(LIB_DIR)
	@cp -v $(LIB_NAME).$(LIB_VERSION) $(LIB_DIR)
	@cp -v $(HEADERS) /usr/include/

uninstall:
	@rm -fv $(LIB_DIR)/$(LIB_NAME)*
	@rm -rv /usr/include/$(HEADERS)

clean:
	@rm -fv $(LIB_NAME).$(LIB_VERSION).0 $(LIB_NAME) $(LIB_NAME).$(LIB_VERSION) $(OBJECTS)
