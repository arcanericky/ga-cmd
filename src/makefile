GA=google-authenticator-libpam
GA_SRC=$(GA)/src
GA_OBJS=hmac.o sha1.o base32.o pam_google_authenticator.o util.o
OBJS=keyhide.o verf.o codegen.o cfgfile.o

.SUFFIXES:

.SUFFIXES: .o .c .h

.c.o :
	gcc -c $< 

$(GA)/config.h:
	(cd $(GA) && ./bootstrap.sh && ./configure)

hmac.o: $(GA_SRC)/hmac.c $(GA)/config.h
	gcc -c -std=gnu99 -I $(GA) $<

sha1.o: $(GA_SRC)/sha1.c $(GA)/config.h
	gcc -c -std=gnu99 -I $(GA) $<

base32.o: $(GA_SRC)/base32.c $(GA)/config.h
	gcc -c -std=gnu99 -I $(GA) $<

util.o: $(GA_SRC)/util.c $(GA)/config.h
	gcc -c -std=gnu99 -I $(GA) $<

pam_google_authenticator.o: $(GA_SRC)/pam_google_authenticator.c $(GA)/config.h
	gcc -c -std=gnu99 -DTESTING -I $(GA) $<

ga-cmd.o: ga-cmd.c bin/prockey
	gcc -c -std=gnu99 -DSEED=`bin/prockey $(KEY)` $<

bin/prockey: $(GA_OBJS) $(OBJS) prockey.o
	@mkdir -p $(@D)
	gcc -o $@ $^

bin/ga-cmd: $(GA_OBJS) $(OBJS) ga-cmd.o
	@mkdir -p $(@D)
	gcc -o $@ $^
	strip $@

bin/ga-test: $(GA_OBJS) $(OBJS) ga-test.o
	@mkdir -p $(@D)
	gcc -o $@ $^
	bin/ga-test

prockey: bin/prockey
	@true

ga-cmd: bin/ga-cmd
	@true

ga-test: bin/ga-test
	@true

all: prockey ga-test ga-cmd

clean:
	rm -rf bin
	rm -f *.o
	rm -f config.h
	(cd $(GA) && make clean)
