GA=google-authenticator/libpam
GA_OBJS=hmac.o sha1.o base32.o pam_google_authenticator.o

.SUFFIXES:

.SUFFIXES: .o .c .h

.c.o :
	gcc -c $< 

hmac.o: $(GA)/hmac.c
	gcc -c -std=gnu99 $<

sha1.o: $(GA)/sha1.c
	gcc -c -std=gnu99 $<

base32.o: $(GA)/base32.c
	gcc -c -std=gnu99 $<

pam_google_authenticator.o: $(GA)/pam_google_authenticator.c
	gcc -c -std=gnu99 -DTESTING $<

ga-cmd.o: ga-cmd.c bin/prockey
	gcc -c -std=gnu99 -DSEED=`bin/prockey $(KEY)` $<

bin/prockey: $(GA_OBJS) ga-lib.o prockey.o
	@mkdir -p $(@D)
	gcc -o $@ $^

bin/ga-cmd: $(GA_OBJS) ga-lib.o ga-cmd.o
	@mkdir -p $(@D)
	gcc -o $@ $^
	strip $@

bin/ga-test: $(GA_OBJS) ga-lib.o ga-test.o
	@mkdir -p $(@D)
	gcc -o $@ $^
	bin/ga-test

prockey: bin/prockey
	@/bin/true

ga-cmd: bin/ga-cmd
	@/bin/true

ga-test: bin/ga-test
	@/bin/true

all: prockey ga-test ga-cmd

clean:
	rm -rf bin
	rm *.o
