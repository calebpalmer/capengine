all: runtest

PLATFORM=UNIX
CPP=g++
INCLUDE=-I../
CFLAGS= -g -Wall -D$(PLATFORM) -DDEBUG 
LIBS=-lcppunit

runtest: test
	chmod 755 test
	./test

objects = test.o

test: $(objects)
	$(CPP) $(CFLAGS) -o test $(objects) ../libcapengine.so $(LIBS)

all: $(objects)

$(objects): %.o: %.cpp
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

.PHONEY: clean

clean:
	rm test
	rm *.o
