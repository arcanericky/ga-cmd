#!/bin/sh

set -e

if [ ! "$1" ]; then
	echo "Building without internal key"
fi

GACMD="bin/ga-cmd.o"

if [ -e "$GACMD" ]
then
	rm "$GACMD"
fi

make clean
make all KEY="$(echo "$1" | tr '[:upper:]' '[:lower:]')"
