//
//  sprinkler-config.h
//  sprinklerdaemon
//
//  Created by Mickey Malone on 4/5/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef sprinklerdaemon_sprinkler_config_h
#define sprinklerdaemon_sprinkler_config_h

#include <syslog.h>

#define MAXLINE 128
#define MAXDURATION 30
#define MINDURATION 2
#define ASCII_NEWLINE 0x0a
#define ASCII_SPACE 0x20
#define ASCII_COMMENT 0x023

#define SECTION_BEGIN_APP_CONFIG        "[BEGIN_APP_CONFIG]"
#define ENTRY_DAEMON                    "DAEMON"
#define ENTRY_LOG_MODE                  "LOG_MODE"
#define ENTRY_LOG_LEVEL                 "LOG_LEVEL"
#define ENTRY_SYSLOG_FACILITY           "LOG_FACILITY"
#define ENTRY_PERIODIC_CHECK            "PERIODIC_CHECK"
#define SECTION_END_APP_CONFIG          "[END_APP_CONFIG]"

#define ENTRY_ENABLED                   "ENABLED"
#define ENTRY_HOUR                      "HOUR"
#define ENTRY_MINUTE                    "MINUTE"
#define ENTRY_ZONE1_DURATION            "ZONE1_DURATION"
#define ENTRY_ZONE2_DURATION            "ZONE2_DURATION"
#define ENTRY_ZONE3_DURATION            "ZONE3_DURATION"
#define ENTRY_ZONE4_DURATION            "ZONE4_DURATION"
#define ENTRY_ZONE5_DURATION            "ZONE5_DURATION"
#define ENTRY_ZONE6_DURATION            "ZONE6_DURATION"

#define SECTION_BEGIN_MONDAY            "[BEGIN_MONDAY]"
#define SECTION_END_MONDAY              "[END_MONDAY]"

#define SECTION_BEGIN_TUESDAY           "[BEGIN_TUESDAY]"
#define SECTION_END_TUESDAY             "[END_TUESDAY]"

#define SECTION_BEGIN_WEDNESDAY         "[BEGIN_WEDNESDAY]"
#define SECTION_END_WEDNESDAY           "[END_WEDNESDAY]"

#define SECTION_BEGIN_THURSDAY          "[BEGIN_THURSDAY]"
#define SECTION_END_THURSDAY            "[END_THURSDAY]"

#define SECTION_BEGIN_FRIDAY            "[BEGIN_FRIDAY]"
#define SECTION_END_FRIDAY              "[END_FRIDAY]"

#define SECTION_BEGIN_SATURDAY          "[BEGIN_SATURDAY]"
#define SECTION_END_SATURDAY            "[END_SATURDAY]"

#define SECTION_BEGIN_SUNDAY            "[BEGIN_SUNDAY]"
#define SECTION_END_SUNDAY              "[END_SUNDAY]"

//struct that will assist us in parsing the config
typedef struct
{
	char *key;
	char *value;
} kv_t;

//daemon specific configuration structure
typedef struct
{
	int periodic_check;     //how often to check the sprinkler system for status in seconds
	char *conf;             //configuration file
	FILE *conffd;           //configuration file handle

} sprinkler_daemon_config_t;

//daily entries
typedef struct
{
	int dayoftheweek;       //day of the week 0 - 6
	int enabled;            //enable bit
	int hour;               //hour of the day
	int minute;             //minute of the hour
	int zone1_duration;     //duration of the sprinkler zone1
	int zone2_duration;     //duration of the sprinkler zone2
	int zone3_duration;     //duration of the sprinkler zone3
	int zone4_duration;     //duration of the sprinkler zone4
	int zone5_duration;     //duration of the sprinkler zone5
	int zone6_duration;     //duration of the sprinkler zone6
} entry_t;

//sprinkler configuration structure - this is what we will pass around
typedef struct
{
	sprinkler_daemon_config_t *app_config;
	entry_t *monday;
	entry_t *tuesday;
	entry_t *wednesday;
	entry_t *thursday;
	entry_t *friday;
	entry_t *saturday;
	entry_t *sunday;

} sprinkler_config_t;

int init_config(sprinkler_config_t *config);
int load_config(sprinkler_config_t *config);
int split_line(char *line, kv_t *kv);
int sanity_check_config(sprinkler_config_t *config);

#endif
