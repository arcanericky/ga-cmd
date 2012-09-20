#!/bin/sh

if [ ! $1 ]; then
	echo Need Seed \($0 \<seed\>\)
	exit 1
fi

if [ ! -d google-authenticator ]; then
	git clone https://code.google.com/p/google-authenticator/
else
	(cd google-authenticator; git pull)
fi

if [ ! -d bin ]; then
	mkdir bin
fi

gcc -o bin/procseed procseed.c
SEED=`bin/procseed $1`

GA=google-authenticator/libpam
gcc -o bin/ga-cmd -DTESTING -DSEED=${SEED} -std=gnu99 ${GA}/hmac.c ${GA}/sha1.c ${GA}/base32.c ${GA}/pam_google_authenticator.c ga-cmd.c
strip bin/ga-cmd
