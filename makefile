# Macros
INCLUDEDIR =include
OBJECTSDIR =src/obj
CFLAGS  =-I$(INCLUDEDIR) -g -Wall
OTHERFLAGS = --always-make
PROJECT = libmytui.a
LIBS =


_DEPS = tui.h utils.h draw.h widgets.h input.h
DEPS = $(patsubst %, $(INCLUDEDIR)/%, $(_DEPS))

_OBJECTS = tui.o utils.o draw.o widgets.o input.o
OBJECTS = $(patsubst %, $(OBJECTSDIR)/%, $(_OBJECTS))

$(OBJECTSDIR)/%.o : src/%.c $(DEPS)
	gcc -c -o $@ $< ${LIBS} $(CFLAGS) 

$(PROJECT): $(OBJECTS)
	ar -cvq $@ $^
    

.PHONY: clean test

test:
	$(MAKE) -C test all $(OTHERFLAGS)

visual:
	$(MAKE) -C test visual $(OTHERFLAGS)

functional:
	$(MAKE) -C test functional $(OTHERFLAGS)

clean:
	del ".\src\obj\*"
	del *.a

cleanobj:
	del ".\src\obj\*"

cleanexe:
	del ".\*.exe"
