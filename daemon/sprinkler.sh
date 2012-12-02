#! /bin/sh 
#
# USB Controller Gadget Daemon (sprinkler.sh)
# Copyright (C) 2012 Mickey Malone     <mickey.malone at gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
