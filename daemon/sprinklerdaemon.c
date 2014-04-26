/**
 *
 * USB Controller Gadget Daemon (sprinklerdaemon.c)
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

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <syslog.h>
#include "sprinklerdaemon.h"

static sprinkler_config_t *myconfig;
int DRYRUN = 0;

//---------------------------------------------------------
//
// Function:    sighandler
// Description  Handles the signals we care about
// Arguments:   int signal
//
// Returns:     nothing
//
//---------------------------------------------------------
void sighandler(int signal)
{
	if (signal == SIGTERM) {
		loginfo("Received SIGTERM - cleaning up");
		check();
		exit(0);
	}else if (signal == SIGHUP) {
		loginfo("Received SIGHUP - re-reading configuration file [%s]", myconfig->app_config->conf);
		process_config();
	}
}


//---------------------------------------------------------
//
// Function: 	send_sprinkler_command
// Description: Wrapper for sendSprinklerZoneCommand
// Arguments:	int Command
//
// Returns:		int
int send_sprinkler_command(int command)
{
	if(DRYRUN) {
		return EXIT_SUCCESS;
	} else {
		return sendSprinklerZoneCommand(command);
	}
}

//---------------------------------------------------------
//
// Function: 	get_sprinkler_status
// Description: Wrapper for get_status_response;
// Arguments:	nothing
//
// Returns:		char
char get_sprinkler_status()
{
	if(DRYRUN) {
		return '0';
	} else {
		return get_status_response();
	}
}

//---------------------------------------------------------
//
// Function:    process_config
// Description  attempts to process the configuration file - can be called multiple times, hopefully
// Arguments:   nothing
//
// Returns:     nothing
//
//---------------------------------------------------------
void process_config()
{
	if (EXIT_FAILURE == (load_config(myconfig))) {
		logerror("An error occured while loading the config, cannot continue\n");
		exit(EXIT_FAILURE);
	}

	logdebug("periodic check:%d", myconfig->app_config->periodic_check);
	logdebug("configuration file:%s", myconfig->app_config->conf);

	logdebug("monday enabled:%d", myconfig->monday->enabled);
	logdebug("monday time:%d:%d 1:%d 2:%d 3:%d 4:%d 5:%d 6:%d", myconfig->monday->hour, myconfig->monday->minute, myconfig->monday->zone1_duration, myconfig->monday->zone2_duration, myconfig->monday->zone3_duration, myconfig->monday->zone4_duration, myconfig->monday->zone5_duration, myconfig->monday->zone6_duration);

	logdebug("tuesday enabled:%d", myconfig->tuesday->enabled);
	logdebug("tuesday time:%d:%d 1:%d 2:%d 3:%d 4:%d 5:%d 6:%d", myconfig->tuesday->hour, myconfig->tuesday->minute, myconfig->tuesday->zone1_duration, myconfig->tuesday->zone2_duration, myconfig->tuesday->zone3_duration, myconfig->tuesday->zone4_duration, myconfig->tuesday->zone5_duration, myconfig->tuesday->zone6_duration);

	logdebug("wednesday enabled:%d", myconfig->wednesday->enabled);
	logdebug("wednesday time:%d:%d 1:%d 2:%d 3:%d 4:%d 5:%d 6:%d", myconfig->wednesday->hour, myconfig->wednesday->minute, myconfig->wednesday->zone1_duration, myconfig->wednesday->zone2_duration, myconfig->wednesday->zone3_duration, myconfig->wednesday->zone4_duration, myconfig->wednesday->zone5_duration, myconfig->wednesday->zone6_duration);

	logdebug("thursday enabled:%d", myconfig->thursday->enabled);
	logdebug("thursday time:%d:%d 1:%d 2:%d 3:%d 4:%d 5:%d 6:%d", myconfig->thursday->hour, myconfig->thursday->minute, myconfig->thursday->zone1_duration, myconfig->thursday->zone2_duration, myconfig->thursday->zone3_duration, myconfig->thursday->zone4_duration, myconfig->thursday->zone5_duration, myconfig->thursday->zone6_duration);

	logdebug("friday enabled:%d", myconfig->friday->enabled);
	logdebug("friday time:%d:%d 1:%d 2:%d 3:%d 4:%d 5:%d 6:%d", myconfig->friday->hour, myconfig->friday->minute, myconfig->friday->zone1_duration, myconfig->friday->zone2_duration, myconfig->friday->zone3_duration, myconfig->friday->zone4_duration, myconfig->friday->zone5_duration, myconfig->friday->zone6_duration);

	logdebug("saturday enabled:%d", myconfig->saturday->enabled);
	logdebug("saturday time:%d:%d 1:%d 2:%d 3:%d 4:%d 5:%d 6:%d", myconfig->saturday->hour, myconfig->saturday->minute, myconfig->saturday->zone1_duration, myconfig->saturday->zone2_duration, myconfig->saturday->zone3_duration, myconfig->saturday->zone4_duration, myconfig->saturday->zone5_duration, myconfig->saturday->zone6_duration);

	logdebug("sunday enabled:%d", myconfig->sunday->enabled);
	logdebug("sunday time:%d:%d 1:%d 2:%d 3:%d 4:%d 5:%d 6:%d", myconfig->sunday->hour, myconfig->sunday->minute, myconfig->sunday->zone1_duration, myconfig->sunday->zone2_duration, myconfig->sunday->zone3_duration, myconfig->sunday->zone4_duration, myconfig->sunday->zone5_duration, myconfig->sunday->zone6_duration);

	//sanity
	if (sanity_check_config(myconfig) != EXIT_SUCCESS) {
		logerror("An error occured while sanity checking the config\n");
		exit(EXIT_FAILURE);
	}
}


//---------------------------------------------------------
// Function:    check
// Description  send the off command every so often so my water bill does not go sky high
// Arguments:   none
//
// Returns:     nothing
//---------------------------------------------------------
void check(void)
{
	char checkresponse = '0';

	logdebug("======Check======");
	int retval = send_sprinkler_command(CMD_STATUS);
	if (retval != 0) {
		logerror("CMD_STATUS command failed");
		gadgetrefresh();
	}else  {
		checkresponse = get_sprinkler_status();
		loginfo("Status command result [%c]", checkresponse);
		if (checkresponse != '0') {
			loginfo("Sending off command");
			retval = send_sprinkler_command(CMD_TOGGLE_ZONEOFF);
			if (retval != 0) {
				logerror("CMD_TOGGLE_ZONEOFF command failed");
				/* may need to reboot the system here */
			}else  {
				loginfo("CMD_TOGGLE_ZONEOFF command completed without error");
			}
		}else  {
			loginfo("All zones off");
		}
	}
}

//---------------------------------------------------------
// Function:    cycle
// Description  cycle zones based on entry
// Arguments:   entry_t
//
// Returns:     nothing
//---------------------------------------------------------
void cycle(entry_t *entry)
{
	logdebug("Cycling zones");

	//start, counter, and end place holders
	time_t start;
	time_t counter;
	time_t end;

	//friendly ascii end time
	char *ascii_end;

	//track the zone
	int zone = 0;

	//cycle the zones based on the duration
	for (zone = 1; zone <= 6; zone++) {
		start = time(NULL);

		switch (zone) {
		case 1:
			if (entry->zone1_duration == 0) {
				loginfo("Skipping zone 1");
				continue;
			}
			end = start + (entry->zone1_duration * 60);
			loginfo("Cycling zone 1");
			if ((send_sprinkler_command(CMD_TOGGLE_ZONE1) == EXIT_SUCCESS)) {
				loginfo("Commmand was sent successfully");
			}else  {
				gadgetrefresh();
				logerror("Received bad status from controller");
			}
			break;
		case 2:
			if (entry->zone2_duration == 0) {
				loginfo("Skipping zone 2");
				continue;
			}
			end = start + (entry->zone2_duration * 60);
			loginfo("Cycling zone 2");
			if ((send_sprinkler_command(CMD_TOGGLE_ZONE2) == EXIT_SUCCESS)) {
				loginfo("Commmand was sent successfully");
			}else  {
				gadgetrefresh();
				logerror("Received bad status from controller");
			}
			break;
		case 3:
			if (entry->zone3_duration == 0) {
				loginfo("Skipping zone 3");
				continue;
			}	
			end = start + (entry->zone3_duration * 60);
			loginfo("Cycling zone 3");
			if ((send_sprinkler_command(CMD_TOGGLE_ZONE3) == EXIT_SUCCESS)) {
				loginfo("Commmand was sent successfully");
			}else  {
				gadgetrefresh();
				logerror("Received bad status from controller");
			}
			break;
		case 4:
			if (entry->zone4_duration == 0) {
				loginfo("Skipping zone 4");
				continue;
			}	
			end = start + (entry->zone4_duration * 60);
			loginfo("Cycling zone 4");
			if ((send_sprinkler_command(CMD_TOGGLE_ZONE4) == EXIT_SUCCESS)) {
				loginfo("Commmand was sent successfully");
			}else  {
				logerror("Received bad status from controller");
			}
			break;
		case 5:
			if (entry->zone5_duration == 0) {
				loginfo("Skipping zone 5");
				continue;
			}	
			end = start + (entry->zone5_duration * 60);
			loginfo("Cycling zone 5");
			if ((send_sprinkler_command(CMD_TOGGLE_ZONE5) == EXIT_SUCCESS)) {
				loginfo("Commmand was sent successfully");
			}else  {
				gadgetrefresh();
				logerror("Received bad status from controller");
			}
			break;
		case 6:
			if (entry->zone6_duration == 0) {
				loginfo("Skipping zone 6");
				continue;
			}
			end = start + (entry->zone6_duration * 60);
			loginfo("Cycling zone 6");
			if ((send_sprinkler_command(CMD_TOGGLE_ZONE6) == EXIT_SUCCESS)) {
				loginfo("Commmand was sent successfully");
			}else  {
				gadgetrefresh();
				logerror("Received bad status from controller");
			}
			break;
		}

		ascii_end = asctime(localtime(&end));
		ascii_end[strlen(ascii_end)-1] = '\0';
		logdebug("Zone cycling will complete on %s", ascii_end);
		while ((counter = time(NULL)) < end) {
			//sleep until finished
			sleep(10);
		}
	}

	//turn them off
	loginfo("Sending off command");
	if ((send_sprinkler_command(CMD_TOGGLE_ZONE6) == EXIT_SUCCESS)) {
			loginfo("Command OFF was sent successfully, cycle finished");
	}else {
		gadgetrefresh();
                logerror("Received bad status from controller");
	}
}


//---------------------------------------------------------
// Function:    daemonize
// Description  makes us into a daemon
// Arguments:
// Returns:     int
//---------------------------------------------------------
int daemonize(void)
{
	int fd;
	pid_t pid;

	//we fork first
	pid = fork();

	switch (pid) {
	case 0:
		fd = open("/dev/null", O_RDWR);

		if (fd < 0) {
			logerror("Cannot open /dev/null");
			return -1;
		}

		if ((dup2(fd, 0) < 0) || (dup2(fd, 1) < 0) || (dup2(fd, 2) < 0)) {
			logerror("Unable to reassign descriptors");
			return -1;
		}

		close(fd);
		chdir("/");
		setsid();

		pid = fork();
		switch (pid) {
		case 0:
			logdebug("Grandchild spawned");
			break;
		case -1:
			return -1;
			break;
		default:
			logdebug("Child is dead");
			exit(0);
			break;
		}
		break;
	case -1:
		return -1;
		break;
	default:
		logdebug("Parent is dead");
		exit(0);
		break;
	}

	//setup signal handlers
	//signal(SIGTERM,sighandler);
	//signal(SIGHUP,sighandler);

	return 0;
}

//---------------------------------------------------------
// Function:    usage
// Description  Displays the program usage
// Arguments:
// Returns:
//---------------------------------------------------------
void usage(void)
{
	//-a = dryrun
	//-o = stdout
	//-d = debug
	//-f = facility [1-7]
	//-c = configuration file
	fprintf(stderr, "sprinklerdaemon [-a] [-o] [-d] [-f [1-9]] [-c file]\n");
	fprintf(stderr, "-a\tdry run sprinkler zones (will not fire)\n");
	fprintf(stderr, "-o\tprint to stdout (will not fork)\n");
	fprintf(stderr, "-d\tdebug\n");
	fprintf(stderr, "-f\tsyslog facility 1-9\n");
	fprintf(stderr, "-c\tconfiguration file\n");
	exit(EXIT_FAILURE);
}

//---------------------------------------------------------
// Function:    main_loop
// Description  This is the looping point of the program
// Arguments:   sprinkler_config_t
// Returns:     never
//---------------------------------------------------------
void main_loop(sprinkler_config_t *config)
{
	logdebug("=======Main loop=======");
	time_t next_check = 0;
	time_t now;
	struct tm *time_struct;

	while (1) {
		//populate now with the number of seconds since January 1, 1970
		now = time(NULL);
		if (next_check <= now) {
			check();
			next_check = now + config->app_config->periodic_check;
		}

		//get it into workable structure
		time_struct = localtime(&now);

		//switch on the day
		switch (time_struct->tm_wday) {
		//monday
		case 1:
			if (time_struct->tm_hour == config->monday->hour && time_struct->tm_min == config->monday->minute && config->monday->enabled != 0) {
				loginfo("Starting monday");
				cycle(config->monday);
			}
			break;
		//tuesday
		case 2:
			if (time_struct->tm_hour == config->tuesday->hour && time_struct->tm_min == config->tuesday->minute && config->tuesday->enabled != 0) {
				loginfo("Starting tuesday");
				cycle(config->tuesday);
			}
			break;
		//wednesday
		case 3:
			if (time_struct->tm_hour == config->wednesday->hour && time_struct->tm_min == config->wednesday->minute && config->wednesday->enabled != 0) {
				loginfo("Starting wednesday");
				cycle(config->wednesday);
			}
			break;
		//thursday
		case 4:
			if (time_struct->tm_hour == config->thursday->hour && time_struct->tm_min == config->thursday->minute && config->thursday->enabled != 0) {
				loginfo("Starting thursday");
				cycle(config->thursday);
			}
			break;
		//friday
		case 5:
			if (time_struct->tm_hour == config->friday->hour && time_struct->tm_min == config->friday->minute && config->friday->enabled != 0) {
				loginfo("Starting friday");
				cycle(config->friday);
			}
			break;
		//saturday
		case 6:
			if (time_struct->tm_hour == config->saturday->hour && time_struct->tm_min == config->saturday->minute && config->saturday->enabled != 0) {
				loginfo("Starting saturday");
				cycle(config->saturday);
			}
			break;
		//sunday
		case 0:
			if (time_struct->tm_hour == config->sunday->hour && time_struct->tm_min == config->sunday->minute && config->sunday->enabled != 0) {
				loginfo("Starting sunday");
				cycle(config->sunday);
			}
			break;

		default:
			logerror("Error, invalid weekday entry");
			exit(-1);
		}

		sleep(10);

	}

}

//---------------------------------------------------------
// Function:    main
// Description  program entry point
// Arguments:   int
//              **argv
//
// Returns:     int
//---------------------------------------------------------
int main(int argc, char **argv)
{
	s_logger_level level = S_LOGGER_INFO;
	s_logger_mode mode = S_LOGGER_MODE_SYSLOG;
	int facility = LOG_LOCAL1;
	char *config_file;

	//arm errno
	errno = 0;

	//parse command line for logging mode and level along with config file override
	//-o = stdout
	//-d = debug
	//-f = facility [1-7]
	//-c = configuration file
	int c;
	while ((c = getopt(argc, argv, "aodf:c:")) != -1) {
		switch (c) {
		case 'a':
			DRYRUN = 1;
		case 'o':
			mode = S_LOGGER_MODE_STDOUT;
			break;
		case 'd':
			level = S_LOGGER_DEBUG;
			break;
		case 'f':
			switch (atoi(optarg)) {
			case 1:
				facility = LOG_LOCAL1;
				break;
			case 2:
				facility = LOG_LOCAL2;
				break;
			case 3:
				facility = LOG_LOCAL3;
				break;
			case 4:
				facility = LOG_LOCAL4;
				break;
			case 5:
				facility = LOG_LOCAL5;
				break;
			case 6:
				facility = LOG_LOCAL6;
				break;
			case 7:
				facility = LOG_LOCAL7;
				break;
			default:
				usage();
				break;
			}
			break;
		case 'c':
			config_file = optarg;
			break;
		default:
			usage();
		}
	}

	if (mode == S_LOGGER_MODE_SYSLOG) {
		initsyslog(level, facility);
	}else  {
		initstdoutlog(level);
	}

	//place our main structure into the heap
	myconfig = malloc(sizeof(sprinkler_config_t));
	if (myconfig <= 0) {
		logerror("Unable to allocate memory (%d) %s", errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

	if ((init_config(myconfig)) != EXIT_SUCCESS) {
		logerror("An error occured while initializing the config, cannot continue\n");
		exit(EXIT_FAILURE);
	}

	if (config_file != NULL || strcmp(config_file, "") != 0) {
		myconfig->app_config->conf = config_file;
	}

	process_config();

	if (mode == S_LOGGER_MODE_SYSLOG) {
		if (daemonize() != 0) {
			logerror("Unable to daemonize");
			exit(EXIT_FAILURE);
		}
	}

	//set signals
	signal(SIGTERM, sighandler);
	signal(SIGHUP, sighandler);

	main_loop(myconfig);

	//we should never get here
	return 0;
}
