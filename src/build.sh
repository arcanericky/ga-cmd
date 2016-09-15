#!/bin/sh

if [ ! $1 ]; then
	echo Need Key \($0 \<key\>\)
	exit 1
fi

if [ ! -d google-authenticator ]; then
	git clone https://github.com/google/google-authenticator.git
else
	#(cd google-authenticator; git pull)
	echo git pull
fi

if [ -e gmd-cmd.o ]
then
	rm gmd-cmd.o
fi

make all KEY=$1
