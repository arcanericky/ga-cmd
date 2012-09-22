#!/bin/sh

if [ ! $1 ]; then
	echo Need Key \($0 \<key\>\)
	exit 1
fi

if [ ! -d google-authenticator ]; then
	git clone https://code.google.com/p/google-authenticator/
else
	#(cd google-authenticator; git pull)
	echo git pull
fi

rm ga-cmd.o

make all KEY=$1
