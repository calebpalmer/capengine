# (setq compile-command "(cd ~/Projects/games/pong/src && make)")
all: libcapengine.so

PLATFORM=UNIX
CC=g++
CPP=g++
INCLUDE=./src
CFLAGS= -g -Wall -D$(PLATFORM) -DDEBUG -std=c++11 -pedantic `sdl-config --cflags`
LIBS= `sdl-config --cflags --libs` -lSDL_image -lboost_system -lboost_filesystem
#CFLAGS= -O3 -D$(PLATFORM) -DDEBUG

#CAPEngine
libcapengine.so: Point2d.o Vector2d.o Time.o VideoManager.o EventDispatcher.o CapEngine.h vector.o matrix.o collision.o fontmanager.o capcommon.o numbergenerator.o pcm.o soundplayer.o
	$(CPP) $(CFLAGS) -o libcapengine.so Point2d.o Vector2d.o Time.o VideoManager.o EventDispatcher.o vector.o matrix.o collision.o \
	fontmanager.o capcommon.o numbergenerator.o pcm.o soundplayer.o -lSDL -lSDL_image -lSDL_ttf -lsndfile -lSDL_sound -shared -fPIC

capcommon.o: capcommon.h capcommon.cpp
	$(CPP) $(CFLAGS) -o capcommon.o -c capcommon.cpp  -fPIC

VideoManager.o: VideoManager.cpp VideoManager.h captypes.h CapEngineException.h
	$(CPP) $(CFLAGS) -o VideoManager.o -c VideoManager.cpp  -fPIC

fontmanager.o:  fontmanager.cpp fontmanager.h VideoManager.h captypes.h
	$(CPP) $(CFLAGS) -o fontmanager.o -c fontmanager.cpp -fPIC

Point2d.o: Point2d.cpp Point2d.h
	$(CPP) $(CFLAGS) -o Point2d.o -c Point2d.cpp -fPIC

Vector2d.o: Vector2d.cpp Vector2d.h Point2d.h
	$(CPP) $(CFLAGS) -o Vector2d.o -c Vector2d.cpp -fPIC

vector.o: vector.cpp vector.h
	$(CPP) $(CFLAGS) -o vector.o -c vector.cpp -fPIC

#frame.o: frame.h frame.cpp vector.h
#	$(CPP) $(CFLAGS) -o frame.o -c frame.cpp -fPIC

Time.o: Time.h Time.cpp
	$(CPP) $(CFLAGS) -o Time.o -c Time.cpp -fPIC

EventDispatcher.o: EventDispatcher.cpp EventDispatcher.h
	$(CPP) $(CFLAGS) -o EventDispatcher.o -c EventDispatcher.cpp -fPIC

matrix.o: matrix.h matrix.cpp vector.h CapEngine.h
	$(CPP) $(CFLAGS) -o matrix.o -c matrix.cpp -fPIC

collision.o: collision.h collision.cpp CapEngine.h
	$(CPP) $(CFLAGS) -o collision.o -c collision.cpp -fPIC

numbergenerator.o: numbergenerator.h numbergenerator.cpp
	$(CPP) $(CFLAGS) -o numbergenerator.o -c numbergenerator.cpp -fPIC

soundplayer.o: soundplayer.h soundplayer.cpp pcm.h CapEngineException.h
	$(CPP) $(CFLAGS) -o soundplayer.o -c soundplayer.cpp -fPIC

pcm.o: pcm.h pcm.cpp CapEngineException.h
	$(CPP) $(CFLAGS) -o pcm.o -c pcm.cpp -fPIC

.PHONEY: clean tests tags

clean:
	-rm -f *.o
	-rm -f *.so
	-rm test/*.o
	-rm test/test

tests:
	cd test && $(MAKE)

tags:
	cd .. && ctags -e -R . && ctags -e -R /usr/include/SDL -a

install:
	install -m 775 libcapengine.so /usr/lib/
