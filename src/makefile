GA=google-authenticator-libpam
GA_SRC=$(GA)/src
GA_OBJS=hmac.o sha1.o base32.o pam_google_authenticator.o util.o
OBJS=keyhide.o verf.o codegen.o cfgfile.o outputcode.o
SRCS=keyhide.c verf.c codegen.c cfgfile.c outputcode.c

.SUFFIXES:

.SUFFIXES: .o .c .h

$(GA)/config.h:
	(cd $(GA) && ./bootstrap.sh && ./configure)

%.o: $(GA_SRC)/%.c $(GA)/config.h
	gcc -c -std=gnu99 -DTESTING -I $(GA) $<

%.o: %.c $(GA)/config.h
	gcc -c -std=gnu99 $(CFLAGS) -DTESTING -I $(GA) $<

ga-cmd.o: ga-cmd.c bin/prockey
	gcc -c -std=gnu99 -DHMACKEY=`bin/prockey $(KEY)` $<

bin/prockey: $(GA_OBJS) $(OBJS) prockey.o
	@mkdir -p $(@D)
	gcc -o $@ $^

bin/ga-cmd: $(GA_OBJS) $(OBJS) ga-cmd.o
	@mkdir -p $(@D)
	gcc -o $@ $^
	strip $@

bin/ga-test: $(GA_OBJS) $(OBJS) ga-test.o cfgfile_test.o
	@mkdir -p $(@D)
	gcc -o $@ -fprofile-arcs $^

test_compile:
	$(eval CFLAGS += -coverage -O0 -fprofile-arcs -ftest-coverage)
	rm -rf *.o

test: test_compile bin/ga-test
	bin/ga-test
	@echo "All tests PASSED"

codecov: test
	gcov $(SRCS)

coverage: test
	gcovr --keep --exclude='.*test.*' --html --html-details -o coverage.html

all: bin/ga-cmd

clean:
	rm -rf bin
	rm -f *.o config.h *.gcno *.gcda *.gcov coverage*.html
	(cd $(GA) && make clean)
