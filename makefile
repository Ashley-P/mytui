# Macros
INCLUDEDIR =include
OBJECTSDIR =src/obj
CFLAGS  =-I$(INCLUDEDIR) -g -Wall
PROJECT = libmytui.a
LIBS =


_DEPS = tui.h utils.h draw.h widgets.h
DEPS = $(patsubst %, $(INCLUDEDIR)/%, $(_DEPS))

_OBJECTS = tui.o utils.o draw.o widgets.o
OBJECTS = $(patsubst %, $(OBJECTSDIR)/%, $(_OBJECTS))

$(OBJECTSDIR)/%.o : src/%.c $(DEPS)
	gcc -c -o $@ $< ${LIBS} $(CFLAGS)

$(PROJECT): $(OBJECTS)
	ar -cvq $@ $^
    

.PHONY: clean test

clean:
	del ".\src\obj\*"
	del *.a

test:
	$(MAKE) -C test

cleantest:
	del ".\test.exe"
	del ".\test2.exe"
	del ".\test3.exe"
	del ".\test4.exe"
