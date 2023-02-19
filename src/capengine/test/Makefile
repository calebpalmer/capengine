all: runtest

include ../Makefile.defs

LIBS += -lcppunit

runtest: test
	chmod 755 test
	./test

build: 	test
	chmod 755 test

objects = test.o

test: $(objects) 
	$(CPP) $(CFLAGS) -o test $(objects) ../libcapengine.so $(LIBS)

test.o: vectortest.h xml_parser_test.h capcommontest.h matrixtest.h scanconvert_test.h 
	$(CPP) $(CFLAGS) -c test.cpp 

all: $(objects)

#$(objects): %.o: %.cpp
#	$(CPP) -c $(CFLAGS) $(INCLUDE) $< -o $@

.PHONEY: clean build

clean:
	rm test
	rm *.o
