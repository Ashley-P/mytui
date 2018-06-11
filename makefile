# Macros
INCLUDEDIR =include
OBJECTSDIR =src/obj
CFLAGS  =-I$(INCLUDEDIR)
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
    

.PHONY: clean

clean:
	del ".\src\obj\*"
	del *.a

test:
	gcc -o test src/test.c -L. -lmytui -Iinclude

cleantest:
	del ".\test.exe"
