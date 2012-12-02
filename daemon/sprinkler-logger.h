/**
 *
 * USB Controller Gadget Daemon (sprinkler-logger.h)
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

#ifndef sprinklerdaemon_sprinkler_logger_h
#define sprinklerdaemon_sprinkler_logger_h

#include "sprinkler-config.h"
#include <time.h>

typedef enum
{
	S_LOGGER_INFO     =   0,
	S_LOGGER_DEBUG    =   1,
	S_LOGGER_TRACE    =   2
} s_logger_level;

typedef enum
{
	S_LOGGER_MODE_STDOUT    =   0,
	S_LOGGER_MODE_SYSLOG    =   1,
} s_logger_mode;

char *daytostring(struct tm *tmst);
void logerror(const char *format, ... );
void logdebug(const char *format, ... );
void loginfo(const char *format, ... );
void initstdoutlog(s_logger_level level);
void initsyslog(s_logger_level level, int facility);
#endif
