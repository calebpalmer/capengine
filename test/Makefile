all: runtest

include ../Makefile.defs

LIBS += -lcppunit

runtest: test
	chmod 755 test
	./test

objects = test.o

test: $(objects) vectortest.h xml_parser_test.h
	$(CPP) $(CFLAGS) -o test $(objects) ../libcapengine.so $(LIBS)

all: $(objects)

$(objects): %.o: %.cpp
	$(CPP) -c $(CFLAGS) $(INCLUDE) $< -o $@

.PHONEY: clean

clean:
	rm test
	rm *.o
