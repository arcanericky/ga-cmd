#!/bin/sh

set -e

if [ ! "$1" ]; then
	echo Need Key \("$0" \<key\>\)
	exit 1
fi

GACMD="bin/ga-cmd.o"

if [ -e "$GACMD" ]
then
	rm "$GACMD"
fi

make all KEY="$(echo "$1" | tr '[:upper:]' '[:lower:]')"
