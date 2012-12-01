SOURCES=sprinkler-config.c  sprinkler-controller.c  sprinkler-logger.c  sprinklerdaemon.c
CFLAGS=
DFLAGS= -ggdb $(CFLAGS)
OBJECTS= sprinkler-config.o  sprinkler-controller.o  sprinkler-logger.o  sprinklerdaemon.o
LIBS= -lcontroller-gadget

all: controller

debug:
	gcc $(DFLAGS) $(SOURCES) $(LIBS) -o sprinklerdaemon

controller: sprinkler-config.o  sprinkler-controller.o  sprinkler-logger.o  sprinklerdaemon.o
	gcc $(CFLAGS) $(OBJECTS) $(LIBS) -o sprinklerdaemon
clean:
	rm -vf sprinklerdaemon *.o
install:
	cp -bf sprinkler.conf /etc/
	cp -f sprinkler.sh /etc/init.d/
	cp -f sprinklerdaemon /sbin/
