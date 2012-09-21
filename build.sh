#!/bin/sh

if [ ! $1 ]; then
	echo Need Seed \($0 \<seed\>\)
	exit 1
fi

if [ ! -d google-authenticator ]; then
	git clone https://code.google.com/p/google-authenticator/
else
	#(cd google-authenticator; git pull)
	echo git pull
fi

if [ ! -d bin ]; then
	mkdir bin
fi

GA=google-authenticator/libpam

echo Compiling ga-test
gcc -o bin/ga-test -DTESTING -std=gnu99 ${GA}/hmac.c ${GA}/sha1.c ${GA}/base32.c ${GA}/pam_google_authenticator.c ga-lib.c ga-test.c
if [ $? -ne 0 ]; then
	echo Compile of ga-test failed
	exit 1
fi

bin/ga-test
if [ $? -ne 0 ]; then
	echo Tests failed
	exit 1
fi

echo Compiling prockey
gcc -o bin/prockey -DTESTING -std=gnu99 ${GA}/hmac.c ${GA}/sha1.c ${GA}/base32.c ${GA}/pam_google_authenticator.c ga-lib.c prockey.c
if [ $? -ne 0 ] ; then
	echo Compile of prockey failed
	exit 1
fi

SEED=`bin/prockey $1`
if [ $? -ne 0 ] ; then
	echo Execution of prockey failed
	exit 1
fi

echo Compiling ga-cmd
gcc -o bin/ga-cmd -DTESTING -DSEED=${SEED} -std=gnu99 ${GA}/hmac.c ${GA}/sha1.c ${GA}/base32.c ${GA}/pam_google_authenticator.c ga-lib.c ga-cmd.c
strip bin/ga-cmd
