#! /bin/sh 
#
# USB Controller Gadget Daemon (sprinkler.sh)
# Copyright (C) 2012 Mickey Malone     <mickey.malone at gmail.com>
#
# > update-rc.d sprinkler defaults 60
#
sprinkler=${SPRINKLER_BIN:-/sbin/sprinklerdaemon}
config=${SPRINKLER_CONF:-/etc/sprinkler.conf}
pid=$(ps -ef | grep sprinklerdaemon | grep -v grep | awk '{print $2}')
test -x "$sprinkler" || exit 0

case "$1" in
  start)
    if [ "X$pid" != "X" ]
    then
	echo "already running [$pid]"
	exit 1
    fi
    
    echo -n "Starting sprinklerdaemon"
    $sprinkler -d -f 1 -c $config
    echo "."
    ;;
  stop)
    echo -n "Stopping sprinklerdaemon"
    kill -s TERM $pid
    echo "."
    ;;
  status)
	if [ "X$pid" != "X" ]
    then
		echo "Sprinkler daemon is running as process id [$pid]"
	else
		echo "Sprinkler daemon is not running"
	fi
    ;;
  refresh)
    echo "Sending HUP to sprinkler daemon"
    kill -s HUP $pid
    ;;
  *)
    echo "Usage: /etc/init.d/sprinklerdaemon {start|stop|restart}"
    exit 1
esac

exit 0
