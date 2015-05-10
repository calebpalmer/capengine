
# (setq compile-command "(cd ~/Projects/games/capengine && make)")
all: libcapengine.so

include Makefile.defs

#CAPEngine
libcapengine.so: Point2d.o Time.o VideoManager.o EventDispatcher.o CapEngine.h vector.o matrix.o collision.o fontmanager.o capcommon.o numbergenerator.o pcm.o soundplayer.o filesystem.o tileset.o map2d.o logger.o timestep.o scanconvert.o xml_parser.o asset_manager.o
	$(CPP) $(CFLAGS) -o libcapengine.so Point2d.o Time.o VideoManager.o EventDispatcher.o vector.o matrix.o collision.o \
	fontmanager.o capcommon.o numbergenerator.o pcm.o soundplayer.o filesystem.o tileset.o map2d.o logger.o timestep.o \
	scanconvert.o xml_parser.o asset_manager.o mouse.o \
	-lSDL -lSDL_image -lSDL_ttf -lsndfile -lSDL_sound -shared -fPIC

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

collision.o: collision.h collision.cpp
	$(CPP) $(CFLAGS) -o collision.o -c collision.cpp -fPIC

numbergenerator.o: numbergenerator.h numbergenerator.cpp
	$(CPP) $(CFLAGS) -o numbergenerator.o -c numbergenerator.cpp -fPIC

soundplayer.o: soundplayer.h soundplayer.cpp pcm.h CapEngineException.h
	$(CPP) $(CFLAGS) -o soundplayer.o -c soundplayer.cpp -fPIC

pcm.o: pcm.h pcm.cpp CapEngineException.h
	$(CPP) $(CFLAGS) -o pcm.o -c pcm.cpp -fPIC

filesystem.o: filesystem.cpp filesystem.h
	$(CPP) $(CFLAGS) -o filesystem.o -c filesystem.cpp -fPIC

tileset.o: tileset.cpp tileset.h
	$(CPP) $(CFLAGS) -o tileset.o -c tileset.cpp -fPIC

map2d.o: map2d.cpp map2d.h collision.h
	$(CPP) $(CFLAGS) -o map2d.o -c map2d.cpp -fPIC

logger.o: logger.cpp logger.h
	$(CPP) $(CFLAGS) -o logger.o -c logger.cpp -fPIC

timestep.o: timestep.cpp timestep.h
	$(CPP) $(CFLAGS) -o timestep.o -c timestep.cpp -fPIC

scanconvert.o: scanconvert.cpp scanconvert.h
	$(CPP) $(CFLAGS) -o scanconvert.o -c scanconvert.cpp -fPIC

xml_parser.o: xml_parser.cpp xml_parser.h
	$(CPP) $(CFLAGS) -o xml_parser.o -c xml_parser.cpp -fPIC

asset_manager.o: asset_manager.cpp asset_manager.h
	$(CPP) $(CFLAGS) -o asset_manager.o -c asset_manager.cpp -fPIC

mouse.o: mouse.cpp mouse.h
	$(CPP) $(CFLAGS) -o mouse.o -c mouse.cpp -fPIC

.PHONEY: clean tests tags

clean:
	-rm -f *.o
	-rm -f *.so
	-rm test/*.o
	-rm test/test

tests:
	cd test && $(MAKE)

tags:
	gtags

install:
	install -m 775 libcapengine.so /usr/lib/
