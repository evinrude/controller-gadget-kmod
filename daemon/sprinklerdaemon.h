/**
 *
 * USB Controller Gadget Daemon (sprinklerdaemon.h)
 * Copyright (C) 2012 Mickey Malone     <mickey.malone at gmail.com>
 *
 */

#ifndef sprinklerdaemon_sprinklerdaemon_h
#define sprinklerdaemon_sprinklerdaemon_h

#include "sprinkler-controller.h"
#include "sprinkler-config.h"
#include "sprinkler-logger.h"

void cycle(entry_t *entry);
void check(void);
void usage(void);
int daemonize(void);
void main_loop(sprinkler_config_t *config);
void sighandler(int signal);
#endif
