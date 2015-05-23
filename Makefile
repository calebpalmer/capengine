
# (setq compile-command "(cd ~/Projects/games/capengine && make)")
all: libcapengine.so

include Makefile.defs

OBJECTS=Point2d.o Time.o VideoManager.o EventDispatcher.o vector.o matrix.o collision.o fontmanager.o \
			capcommon.o numbergenerator.o pcm.o soundplayer.o filesystem.o tileset.o map2d.o logger.o timestep.o \
			scanconvert.o xml_parser.o asset_manager.o mouse.o locator.o

#CAPEngine
libcapengine.so: $(OBJECTS)
	$(CPP) $(CFLAGS) -o libcapengine.so $(OBJECTS) -lSDL -lSDL_image -lSDL_ttf -lsndfile -lSDL_sound -shared -fPIC

$(OBJECTS): %.o: %.cpp
	$(CC) -c $(CFLAGS) $(INCLUDEDIRS) $< -o $@ -fPIC

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
