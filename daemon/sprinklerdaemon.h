/**
 *
 * USB Controller Gadget Daemon (sprinklerdaemon.h)
 * Copyright (C) 2012 Mickey Malone     <mickey.malone at gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#ifndef sprinklerdaemon_sprinklerdaemon_h
#define sprinklerdaemon_sprinklerdaemon_h

#include "sprinkler-controller.h"
#include "sprinkler-config.h"
#include "sprinkler-logger.h"

void process_config();
void cycle(entry_t *entry);
void check(void);
void usage(void);
int daemonize(void);
void main_loop(sprinkler_config_t *config);
void sighandler(int signal);
int send_sprinkler_command(int command);
char get_sprinkler_status();
#endif
