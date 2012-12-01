//
//  sprinkler-config.c
//  sprinklerdaemon
//
//  Created by Mickey Malone on 4/5/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include "sprinkler-config.h"
#include "sprinkler-logger.h"
#include <errno.h>
#include <string.h>
#include <stdlib.h>

//---------------------------------------------------------
// Function:    split_line
// Description  splits a line into a key value pair
// Arguments:   char*
//              kv_t
// Returns:     result
//---------------------------------------------------------
int split_line(char *line, kv_t *kv)
{
	char *result = NULL;
	char *key_ptr_equals = strchr(line, ASCII_NEWLINE);

	//null out spaces and newline
	if (key_ptr_equals) {
		*key_ptr_equals = 0;
	}

	result = strtok(line, "=");
	if ( result != NULL ) {
		kv->key = result;
		result = strtok( NULL, "=" );
		if (result != NULL) {
			kv->value = result;
		}
	}else  {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;

}

//---------------------------------------------------------
// Function:    load_config
// Description  loads the config and sets the values of the sprinkler_config_t
// Arguments:   sprinkler_config_t
//
// Returns:     int, consult enum
//---------------------------------------------------------
int load_config(sprinkler_config_t *config)
{
	char line[MAXLINE];

	if (NULL == (config->app_config->conffd = fopen(config->app_config->conf, "r"))) {
		logerror("An error occured while trying to open %s. (%d) %s", config->app_config->conf, errno, strerror(errno));
		return EXIT_FAILURE;
	}

	//config is loaded, now parse it line by line and set the sprinkler_config_t values as specified
	kv_t *kv = malloc(sizeof(kv_t));
	int d_config = 0;
	int s_day = -1;
	while (fgets( line, MAXLINE, config->app_config->conffd) != NULL) {
		//do not send comments, spaces, or newlines to be parsed
		if (line[0] == ASCII_NEWLINE || line[0] == ASCII_SPACE || line[0] == ASCII_COMMENT) {
			continue;
		}

		if (EXIT_FAILURE == split_line(line, kv)) {
			logerror("An error occured while while loading (%s), unable to split string.", line);
			return EXIT_FAILURE;
		}

		//daemon stuff
		if (strcmp(SECTION_BEGIN_APP_CONFIG, kv->key) == 0) {
			d_config = 1;
		}

		if (strcmp(ENTRY_PERIODIC_CHECK, kv->key) == 0 && d_config) {
			config->app_config->periodic_check = atoi(kv->value);
		}

		if (strcmp(SECTION_END_APP_CONFIG, kv->key) == 0) {
			d_config = 0;
		}

		//monday stuff
		if (strcmp(SECTION_BEGIN_MONDAY, kv->key) == 0) {
			s_day = 1;
		}

		if (strcmp(SECTION_END_MONDAY, kv->key) == 0) {
			s_day = -1;
		}

		//tuesday stuff
		if (strcmp(SECTION_BEGIN_TUESDAY, kv->key) == 0) {
			s_day = 2;
		}

		if (strcmp(SECTION_END_TUESDAY, kv->key) == 0) {
			s_day = -1;
		}

		//wednesday stuff
		if (strcmp(SECTION_BEGIN_WEDNESDAY, kv->key) == 0) {
			s_day = 3;
		}

		if (strcmp(SECTION_END_WEDNESDAY, kv->key) == 0) {
			s_day = -1;
		}

		//thursday stuff
		if (strcmp(SECTION_BEGIN_THURSDAY, kv->key) == 0) {
			s_day = 4;
		}

		if (strcmp(SECTION_END_THURSDAY, kv->key) == 0) {
			s_day = -1;
		}

		//friday stuff
		if (strcmp(SECTION_BEGIN_FRIDAY, kv->key) == 0) {
			s_day = 5;
		}

		if (strcmp(SECTION_END_FRIDAY, kv->key) == 0) {
			s_day = -1;
		}

		//saturday stuff
		if (strcmp(SECTION_BEGIN_SATURDAY, kv->key) == 0) {
			s_day = 6;
		}

		if (strcmp(SECTION_END_SATURDAY, kv->key) == 0) {
			s_day = -1;
		}

		//sunday stuff
		if (strcmp(SECTION_BEGIN_SUNDAY, kv->key) == 0) {
			s_day = 0;
		}

		if (strcmp(SECTION_END_SUNDAY, kv->key) == 0) {
			s_day = -1;
		}

		//sprinkler stuff
		switch (s_day) {
		//monday
		case 1:
			if (strcmp(ENTRY_ENABLED, kv->key) == 0) {
				config->monday->enabled = atoi(kv->value);
			}
			if (strcmp(ENTRY_HOUR, kv->key) == 0) {
				config->monday->hour = atoi(kv->value);
			}
			if (strcmp(ENTRY_MINUTE, kv->key) == 0) {
				config->monday->minute = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE1_DURATION, kv->key) == 0) {
				config->monday->zone1_duration = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE2_DURATION, kv->key) == 0) {
				config->monday->zone2_duration = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE3_DURATION, kv->key) == 0) {
				config->monday->zone3_duration = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE4_DURATION, kv->key) == 0) {
				config->monday->zone4_duration = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE5_DURATION, kv->key) == 0) {
				config->monday->zone5_duration = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE6_DURATION, kv->key) == 0) {
				config->monday->zone6_duration = atoi(kv->value);
			}
			break;

		//tuesday
		case 2:
			if (strcmp(ENTRY_ENABLED, kv->key) == 0) {
				config->tuesday->enabled = atoi(kv->value);
			}
			if (strcmp(ENTRY_HOUR, kv->key) == 0) {
				config->tuesday->hour = atoi(kv->value);
			}
			if (strcmp(ENTRY_MINUTE, kv->key) == 0) {
				config->tuesday->minute = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE1_DURATION, kv->key) == 0) {
				config->tuesday->zone1_duration = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE2_DURATION, kv->key) == 0) {
				config->tuesday->zone2_duration = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE3_DURATION, kv->key) == 0) {
				config->tuesday->zone3_duration = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE4_DURATION, kv->key) == 0) {
				config->tuesday->zone4_duration = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE5_DURATION, kv->key) == 0) {
				config->tuesday->zone5_duration = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE6_DURATION, kv->key) == 0) {
				config->tuesday->zone6_duration = atoi(kv->value);
			}
			break;

		//wednesday
		case 3:
			if (strcmp(ENTRY_ENABLED, kv->key) == 0) {
				config->wednesday->enabled = atoi(kv->value);
			}
			if (strcmp(ENTRY_HOUR, kv->key) == 0) {
				config->wednesday->hour = atoi(kv->value);
			}
			if (strcmp(ENTRY_MINUTE, kv->key) == 0) {
				config->wednesday->minute = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE1_DURATION, kv->key) == 0) {
				config->wednesday->zone1_duration = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE2_DURATION, kv->key) == 0) {
				config->wednesday->zone2_duration = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE3_DURATION, kv->key) == 0) {
				config->wednesday->zone3_duration = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE4_DURATION, kv->key) == 0) {
				config->wednesday->zone4_duration = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE5_DURATION, kv->key) == 0) {
				config->wednesday->zone5_duration = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE6_DURATION, kv->key) == 0) {
				config->wednesday->zone6_duration = atoi(kv->value);
			}
			break;

		//thursday
		case 4:
			if (strcmp(ENTRY_ENABLED, kv->key) == 0) {
				config->thursday->enabled = atoi(kv->value);
			}
			if (strcmp(ENTRY_HOUR, kv->key) == 0) {
				config->thursday->hour = atoi(kv->value);
			}
			if (strcmp(ENTRY_MINUTE, kv->key) == 0) {
				config->thursday->minute = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE1_DURATION, kv->key) == 0) {
				config->thursday->zone1_duration = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE2_DURATION, kv->key) == 0) {
				config->thursday->zone2_duration = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE3_DURATION, kv->key) == 0) {
				config->thursday->zone3_duration = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE4_DURATION, kv->key) == 0) {
				config->thursday->zone4_duration = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE5_DURATION, kv->key) == 0) {
				config->thursday->zone5_duration = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE6_DURATION, kv->key) == 0) {
				config->thursday->zone6_duration = atoi(kv->value);
			}
			break;

		//friday
		case 5:
			if (strcmp(ENTRY_ENABLED, kv->key) == 0) {
				config->friday->enabled = atoi(kv->value);
			}
			if (strcmp(ENTRY_HOUR, kv->key) == 0) {
				config->friday->hour = atoi(kv->value);
			}
			if (strcmp(ENTRY_MINUTE, kv->key) == 0) {
				config->friday->minute = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE1_DURATION, kv->key) == 0) {
				config->friday->zone1_duration = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE2_DURATION, kv->key) == 0) {
				config->friday->zone2_duration = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE3_DURATION, kv->key) == 0) {
				config->friday->zone3_duration = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE4_DURATION, kv->key) == 0) {
				config->friday->zone4_duration = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE5_DURATION, kv->key) == 0) {
				config->friday->zone5_duration = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE6_DURATION, kv->key) == 0) {
				config->friday->zone6_duration = atoi(kv->value);
			}
			break;

		//saturday
		case 6:
			if (strcmp(ENTRY_ENABLED, kv->key) == 0) {
				config->saturday->enabled = atoi(kv->value);
			}
			if (strcmp(ENTRY_HOUR, kv->key) == 0) {
				config->saturday->hour = atoi(kv->value);
			}
			if (strcmp(ENTRY_MINUTE, kv->key) == 0) {
				config->saturday->minute = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE1_DURATION, kv->key) == 0) {
				config->saturday->zone1_duration = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE2_DURATION, kv->key) == 0) {
				config->saturday->zone2_duration = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE3_DURATION, kv->key) == 0) {
				config->saturday->zone3_duration = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE4_DURATION, kv->key) == 0) {
				config->saturday->zone4_duration = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE5_DURATION, kv->key) == 0) {
				config->saturday->zone5_duration = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE6_DURATION, kv->key) == 0) {
				config->saturday->zone6_duration = atoi(kv->value);
			}
			break;

		//sunday
		case 0:
			if (strcmp(ENTRY_ENABLED, kv->key) == 0) {
				config->sunday->enabled = atoi(kv->value);
			}
			if (strcmp(ENTRY_HOUR, kv->key) == 0) {
				config->sunday->hour = atoi(kv->value);
			}
			if (strcmp(ENTRY_MINUTE, kv->key) == 0) {
				config->sunday->minute = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE1_DURATION, kv->key) == 0) {
				config->sunday->zone1_duration = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE2_DURATION, kv->key) == 0) {
				config->sunday->zone2_duration = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE3_DURATION, kv->key) == 0) {
				config->sunday->zone3_duration = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE4_DURATION, kv->key) == 0) {
				config->sunday->zone4_duration = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE5_DURATION, kv->key) == 0) {
				config->sunday->zone5_duration = atoi(kv->value);
			}
			if (strcmp(ENTRY_ZONE6_DURATION, kv->key) == 0) {
				config->sunday->zone6_duration = atoi(kv->value);
			}
			break;

		default:
			break;

		}


	}

	fclose(config->app_config->conffd);
	free(kv);
	return EXIT_SUCCESS;

}


//---------------------------------------------------------
// Function:    init_config
// Description  readies and sets the sprinkler_config_t with default values
// Arguments:   sprinkler_config_t
//
// Returns:     EXIT status
//---------------------------------------------------------
int init_config(sprinkler_config_t *config)
{
	//app config
	config->app_config = malloc(sizeof(sprinkler_daemon_config_t));
	if (  config->app_config <= 0 ) {
		logerror("Unable to malloc sprinkler_daemon_config_t");
		return EXIT_FAILURE;
	}
	config->app_config->periodic_check = 3600;
	config->app_config->conf = "/etc/sprinkler.conf";
	config->app_config->conffd = 0;

	//monday
	config->monday = malloc(sizeof(entry_t));
	if (  config->monday <= 0 ) {
		logerror("Unable to malloc entry_t");
		return EXIT_FAILURE;
	}
	config->monday->dayoftheweek = 1;
	config->monday->enabled = 0;
	config->monday->hour = 0;
	config->monday->minute = 0;
	config->monday->zone1_duration = 0;
	config->monday->zone2_duration = 0;
	config->monday->zone3_duration = 0;
	config->monday->zone4_duration = 0;
	config->monday->zone5_duration = 0;
	config->monday->zone6_duration = 0;

	//tuesday
	config->tuesday = malloc(sizeof(entry_t));
	if (  config->tuesday <= 0 ) {
		logerror("Unable to malloc entry_t");
		return EXIT_FAILURE;
	}
	config->tuesday->dayoftheweek = 2;
	config->tuesday->enabled = 0;
	config->tuesday->hour = 0;
	config->tuesday->minute = 0;
	config->tuesday->zone1_duration = 0;
	config->tuesday->zone2_duration = 0;
	config->tuesday->zone3_duration = 0;
	config->tuesday->zone4_duration = 0;
	config->tuesday->zone5_duration = 0;
	config->tuesday->zone6_duration = 0;

	//wednesday
	config->wednesday = malloc(sizeof(entry_t));
	if (  config->wednesday <= 0 ) {
		logerror("Unable to malloc entry_t");
		return EXIT_FAILURE;
	}
	config->wednesday->dayoftheweek = 3;
	config->wednesday->enabled = 0;
	config->wednesday->hour = 0;
	config->wednesday->minute = 0;
	config->wednesday->zone1_duration = 0;
	config->wednesday->zone2_duration = 0;
	config->wednesday->zone3_duration = 0;
	config->wednesday->zone4_duration = 0;
	config->wednesday->zone5_duration = 0;
	config->wednesday->zone6_duration = 0;

	//thursday
	config->thursday = malloc(sizeof(entry_t));
	if (  config->thursday <= 0 ) {
		logerror("Unable to malloc entry_t");
		return EXIT_FAILURE;
	}
	config->thursday->dayoftheweek = 4;
	config->thursday->enabled = 0;
	config->thursday->hour = 0;
	config->thursday->minute = 0;
	config->thursday->zone1_duration = 0;
	config->thursday->zone2_duration = 0;
	config->thursday->zone3_duration = 0;
	config->thursday->zone4_duration = 0;
	config->thursday->zone5_duration = 0;
	config->thursday->zone6_duration = 0;

	//friday
	config->friday = malloc(sizeof(entry_t));
	if (  config->friday <= 0 ) {
		logerror("Unable to malloc entry_t");
		return EXIT_FAILURE;
	}
	config->friday->dayoftheweek = 5;
	config->friday->enabled = 0;
	config->friday->hour = 0;
	config->friday->minute = 0;
	config->friday->zone1_duration = 0;
	config->friday->zone2_duration = 0;
	config->friday->zone3_duration = 0;
	config->friday->zone4_duration = 0;
	config->friday->zone5_duration = 0;
	config->friday->zone6_duration = 0;

	//saturday
	config->saturday = malloc(sizeof(entry_t));
	if (  config->saturday <= 0 ) {
		logerror("Unable to malloc entry_t");
		return EXIT_FAILURE;
	}
	config->saturday->dayoftheweek = 6;
	config->saturday->enabled = 0;
	config->saturday->hour = 0;
	config->saturday->minute = 0;
	config->saturday->zone1_duration = 0;
	config->saturday->zone2_duration = 0;
	config->saturday->zone3_duration = 0;
	config->saturday->zone4_duration = 0;
	config->saturday->zone5_duration = 0;
	config->saturday->zone6_duration = 0;

	//sunday
	config->sunday = malloc(sizeof(entry_t));
	if (  config->sunday <= 0 ) {
		logerror("Unable to malloc entry_t");
		return EXIT_FAILURE;
	}
	config->sunday->dayoftheweek = 0;
	config->sunday->enabled = 0;
	config->sunday->hour = 0;
	config->sunday->minute = 0;
	config->sunday->zone1_duration = 0;
	config->sunday->zone2_duration = 0;
	config->sunday->zone3_duration = 0;
	config->sunday->zone4_duration = 0;
	config->sunday->zone5_duration = 0;
	config->sunday->zone6_duration = 0;

	return EXIT_SUCCESS;
}

//---------------------------------------------------------
// Function:    sanity_check_config
// Description  checks the sprinkler_config_t for completeness
// Arguments:   sprinkler_config_t
//
// Returns:     result
//---------------------------------------------------------
int sanity_check_config(sprinkler_config_t *config)
{
	//check monday
	if (config->monday->enabled) {
		if (config->monday->hour < 0 || config->monday->hour > 23) {
			logerror("Hour entry must be between 0 and 23, please check config");
			return EXIT_FAILURE;
		}

		if (config->monday->minute < 0 || config->monday->minute > 59) {
			logerror("Minute entry must be between 0 and 59, please check config");
			return EXIT_FAILURE;
		}

		if (config->monday->zone1_duration < MINDURATION || config->monday->zone1_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		if (config->monday->zone2_duration < MINDURATION || config->monday->zone2_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		if (config->monday->zone3_duration < MINDURATION || config->monday->zone3_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		if (config->monday->zone4_duration < MINDURATION || config->monday->zone4_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		if (config->monday->zone5_duration < MINDURATION || config->monday->zone5_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		if (config->monday->zone6_duration < MINDURATION || config->monday->zone6_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		logdebug("Monday passed validation");
	}

	//check tuesday
	if (config->tuesday->enabled) {
		if (config->tuesday->hour < 0 || config->tuesday->hour > 23) {
			logerror("Hour entry must be between 0 and 23, please check config");
			return EXIT_FAILURE;
		}

		if (config->tuesday->minute < 0 || config->tuesday->minute > 59) {
			logerror("Minute entry must be between 0 and 59, please check config");
			return EXIT_FAILURE;
		}

		if (config->tuesday->zone1_duration < MINDURATION || config->tuesday->zone1_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		if (config->tuesday->zone2_duration < MINDURATION || config->tuesday->zone2_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		if (config->tuesday->zone3_duration < MINDURATION || config->tuesday->zone3_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		if (config->tuesday->zone4_duration < MINDURATION || config->tuesday->zone4_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		if (config->tuesday->zone5_duration < MINDURATION || config->tuesday->zone5_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		if (config->tuesday->zone6_duration < MINDURATION || config->tuesday->zone6_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		logdebug("Tuesday passed validation");
	}

	//check wednesday
	if (config->wednesday->enabled) {
		if (config->wednesday->hour < 0 || config->wednesday->hour > 23) {
			logerror("Hour entry must be between 0 and 23, please check config");
			return EXIT_FAILURE;
		}

		if (config->wednesday->minute < 0 || config->wednesday->minute > 59) {
			logerror("Minute entry must be between 0 and 59, please check config");
			return EXIT_FAILURE;
		}

		if (config->wednesday->zone1_duration < MINDURATION || config->wednesday->zone1_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		if (config->wednesday->zone2_duration < MINDURATION || config->wednesday->zone2_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		if (config->wednesday->zone3_duration < MINDURATION || config->wednesday->zone3_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		if (config->wednesday->zone4_duration < MINDURATION || config->wednesday->zone4_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		if (config->wednesday->zone5_duration < MINDURATION || config->wednesday->zone5_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		if (config->wednesday->zone6_duration < MINDURATION || config->wednesday->zone6_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		logdebug("Wednesday passed validation");
	}

	//check thursday
	if (config->thursday->enabled) {
		if (config->thursday->hour < 0 || config->thursday->hour > 23) {
			logerror("Hour entry must be between 0 and 23, please check config");
			return EXIT_FAILURE;
		}

		if (config->thursday->minute < 0 || config->thursday->minute > 59) {
			logerror("Minute entry must be between 0 and 59, please check config");
			return EXIT_FAILURE;
		}

		if (config->thursday->zone1_duration < MINDURATION || config->thursday->zone1_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		if (config->thursday->zone2_duration < MINDURATION || config->thursday->zone2_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		if (config->thursday->zone3_duration < MINDURATION || config->thursday->zone3_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		if (config->thursday->zone4_duration < MINDURATION || config->thursday->zone4_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		if (config->thursday->zone5_duration < MINDURATION || config->thursday->zone5_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		if (config->thursday->zone6_duration < MINDURATION || config->thursday->zone6_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		logdebug("Thursday passed validation");
	}

	//check friday
	if (config->friday->enabled) {
		if (config->friday->hour < 0 || config->friday->hour > 23) {
			logerror("Hour entry must be between 0 and 23, please check config");
			return EXIT_FAILURE;
		}

		if (config->friday->minute < 0 || config->friday->minute > 59) {
			logerror("Minute entry must be between 0 and 59, please check config");
			return EXIT_FAILURE;
		}

		if (config->friday->zone1_duration < MINDURATION || config->friday->zone1_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		if (config->friday->zone2_duration < MINDURATION || config->friday->zone2_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		if (config->friday->zone3_duration < MINDURATION || config->friday->zone3_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		if (config->friday->zone4_duration < MINDURATION || config->friday->zone4_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		if (config->friday->zone5_duration < MINDURATION || config->friday->zone5_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		if (config->friday->zone6_duration < MINDURATION || config->friday->zone6_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		logdebug("Friday passed validation");
	}

	//check saturday
	if (config->saturday->enabled) {
		if (config->saturday->hour < 0 || config->saturday->hour > 23) {
			logerror("Hour entry must be between 0 and 23, please check config");
			return EXIT_FAILURE;
		}

		if (config->saturday->minute < 0 || config->saturday->minute > 59) {
			logerror("Minute entry must be between 0 and 59, please check config");
			return EXIT_FAILURE;
		}

		if (config->saturday->zone1_duration < MINDURATION || config->saturday->zone1_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		if (config->saturday->zone2_duration < MINDURATION || config->saturday->zone2_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		if (config->saturday->zone3_duration < MINDURATION || config->saturday->zone3_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		if (config->saturday->zone4_duration < MINDURATION || config->saturday->zone4_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		if (config->saturday->zone5_duration < MINDURATION || config->saturday->zone5_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		if (config->saturday->zone6_duration < MINDURATION || config->saturday->zone6_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		logdebug("Saturday passed validation");
	}

	//check sunday
	if (config->sunday->enabled) {
		if (config->sunday->hour < 0 || config->sunday->hour > 23) {
			logerror("Hour entry must be between 0 and 23, please check config");
			return EXIT_FAILURE;
		}

		if (config->sunday->minute < 0 || config->sunday->minute > 59) {
			logerror("Minute entry must be between 0 and 59, please check config");
			return EXIT_FAILURE;
		}

		if (config->sunday->zone1_duration < MINDURATION || config->sunday->zone1_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		if (config->sunday->zone2_duration < MINDURATION || config->sunday->zone2_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		if (config->sunday->zone3_duration < MINDURATION || config->sunday->zone3_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		if (config->sunday->zone4_duration < MINDURATION || config->sunday->zone4_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		if (config->sunday->zone5_duration < MINDURATION || config->sunday->zone5_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		if (config->sunday->zone6_duration < MINDURATION || config->sunday->zone6_duration > MAXDURATION) {
			logerror("Duration is out of range (%d-%d), please check config", MINDURATION, MAXDURATION);
			return EXIT_FAILURE;
		}

		logdebug("Sunday passed validation");
	}

	return EXIT_SUCCESS;
}




