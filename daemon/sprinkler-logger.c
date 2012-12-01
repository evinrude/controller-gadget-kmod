//
//  sprinkler-logger.c
//  sprinklerdaemon
//
//  Created by Mickey Malone on 4/6/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include <time.h>
#include <syslog.h>
#include <stdarg.h>
#include <stdlib.h>

#include "sprinkler-logger.h"
#include "sprinkler-config.h"

static s_logger_mode MODE;
static s_logger_level LEVEL;

//---------------------------------------------------------
// Function:    initstdoutlog(s_logger_level level);
// Description  initiate logger in stdout mode
// Arguments:   s_logger_level
//
// Returns:     void
//---------------------------------------------------------
void initstdoutlog(s_logger_level level)
{
	MODE = S_LOGGER_MODE_STDOUT;
	LEVEL = level;
	logdebug("Initialized logger in stdout mode");
}

//---------------------------------------------------------
// Function:    initsyslog(s_logger_level level);
// Description  initiate logger in syslog mode
// Arguments:   s_logger_level
//              syslog_facility
//
// Returns:     void
//---------------------------------------------------------
void initsyslog(s_logger_level level, int facility)
{
	LEVEL = level;
	MODE = S_LOGGER_MODE_SYSLOG;
	openlog("sprinklerdaemon", LOG_PID | LOG_CONS, facility);
	logdebug("Initialized logger in syslog mode");
}



//---------------------------------------------------------
// Function:    daytostring
// Description  converts a day to string
// Arguments:   struct tm
//
// Returns:     string representation of the day
//---------------------------------------------------------
char *daytostring(struct tm *tmst)
{
	switch (tmst->tm_wday) {
	case 0:
		return "sunday";
		break;
	case 1:
		return "monday";
		break;
	case 2:
		return "tuesday";
		break;
	case 3:
		return "wednesday";
		break;
	case 4:
		return "thursday";
		break;
	case 5:
		return "friday";
		break;
	case 6:
		return "saturday";
		break;
	default:
		return "WTF";
		break;
	}
}

//---------------------------------------------------------
// Function:    logerror
// Description  send a message to specified location
// Arguments:   level
//              message format just like printf
//
// Returns:     nothing
//---------------------------------------------------------
void logerror(const char *format, ... )
{
	//get the time in case we need it
	time_t now;
	struct tm *structured_time;

	now = time(NULL);
	structured_time = localtime(&now);

	//create a message place holder
	char *message = malloc(128);

	//stdarg.h stuff
	va_list arguments;
	va_start(arguments, format);

	//for some reason, casting void here does not fuck up my args being passed
	(void)vsprintf(message, format, arguments);

	//put the va_list back to a good state
	va_end(arguments);

	if (MODE == S_LOGGER_MODE_SYSLOG) {
		syslog(LOG_ERR, "ERROR: %s", message);
	}else  {
		fprintf(stdout, "%s %02d:%02d [sprinklerdaemon][%d] ERROR: %s\n", daytostring(structured_time), structured_time->tm_hour, structured_time->tm_min, getpid(), message);
	}

	free(message);
}

//---------------------------------------------------------
// Function:    loginfo
// Description  send a message to specified location
// Arguments:   level
//              message format just like printf
//
// Returns:     nothing
//---------------------------------------------------------
void loginfo(const char *format, ... )
{
	//get the time in case we need it
	time_t now;
	struct tm *structured_time;

	now = time(NULL);
	structured_time = localtime(&now);

	//create a message place holder
	char *message = malloc(128);

	//stdarg.h stuff
	va_list arguments;
	va_start(arguments, format);

	//for some reason, casting void here does not fuck up my args being passed
	(void)vsprintf(message, format, arguments);

	//put the va_list back to a good state
	va_end(arguments);

	if (MODE ==  S_LOGGER_MODE_SYSLOG) {
		syslog(LOG_INFO, "INFO: %s", message);
	}else  {
		fprintf(stdout, "%s %02d:%02d [sprinklerdaemon][%d] INFO: %s\n", daytostring(structured_time), structured_time->tm_hour, structured_time->tm_min, getpid(), message);
	}

	free(message);
}

//---------------------------------------------------------
// Function:    logdebug
// Description  send a message to specified location
// Arguments:   level
//              message format just like printf
//
// Returns:     nothing
//---------------------------------------------------------
void logdebug(const char *format, ... )
{
	if (LEVEL != S_LOGGER_DEBUG) {
		return;
	}

	//get the time in case we need it
	time_t now;
	struct tm *structured_time;
	now = time(NULL);
	structured_time = localtime(&now);

	//create a message place holder
	char *message = malloc(128);

	//stdarg.h stuff
	va_list arguments;
	va_start(arguments, format);

	//for some reason, casting void here does not fuck up my args being passed
	(void)vsprintf(message, format, arguments);

	//put the va_list back to a good state
	va_end(arguments);

	if (MODE ==  S_LOGGER_MODE_SYSLOG) {
		syslog(LOG_DEBUG, "DEBUG: %s", message);
	}else  {
		fprintf(stdout, "%s %02d:%02d [sprinklerdaemon][%d] DEBUG: %s\n", daytostring(structured_time), structured_time->tm_hour, structured_time->tm_min, getpid(), message);
	}

	free(message);
}
