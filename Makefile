
# (setq compile-command "(cd ~/Projects/games/capengine && make)")
all: libcapengine.so editor

include Makefile.defs

OBJECTS=Point2d.o Time.o VideoManager.o EventDispatcher.o vector.o matrix.o collision.o fontmanager.o \
			capcommon.o numbergenerator.o pcm.o soundplayer.o filesystem.o tileset.o map2d.o logger.o timestep.o \
			scanconvert.o xml_parser.o asset_manager.o mouse.o locator.o textbutton.o gameobject.o game_management.o \
			runner.o platformermap.o colour.o button_group.o controller.o physics.o IEventSubscriber.o

EDITOR_OBJECTS=editor.o

#CAPEngine
libcapengine.so: $(OBJECTS)
	$(CPP) $(CFLAGS) -o libcapengine.so $(OBJECTS) -lsndfile -shared -fPIC

editor: $(EDITOR_OBJECTS)
	$(CPP) $(CFLAGS) -o editor $(EDITOR_OBJECTS) libcapengine.so -lsndfile

$(OBJECTS): %.o: %.cpp
	$(CPP) -c $(CFLAGS) $< -o $@ -fPIC

$(EDITOR_OBJECTS): %.o: %.cpp
	$(CPP) -c $(CFLAGS) $< -o $@ -fPIC


.PHONEY: clean tests tags

clean:
	-rm libcapengine.so
	-rm editor
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
