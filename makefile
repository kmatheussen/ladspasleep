


CFLAGS=-Wall -Iinclude -I/usr/local/include -I/site/include -fPIC
LDFLAGS=-lm -lc

all: sleep.so

clean:
	rm -f *.o *.so *~

install: sleep.so
	cp sleep.so ${LADSPA_PATH}

sleep.so: sleep.o makefile
	ld -shared -o sleep.so sleep.o $(LDFLAGS)

sleep.o: sleep.c
	gcc -c sleep.c $(CFLAGS)

