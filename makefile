# Macros
INCLUDEDIR =include
OBJECTSDIR =src/obj
CFLAGS  =-I$(INCLUDEDIR)
PROJECT = libmytui.a
LIBS =


_DEPS =
DEPS = $(patsubst %, $(INCLUDEDIR)/%, $(_DEPS))

_OBJECTS = tui.o
OBJECTS = $(patsubst %, $(OBJECTSDIR)/%, $(_OBJECTS))

$(OBJECTSDIR)/%.o : src/%.c $(DEPS)
	gcc -c -o $@ $< ${LIBS} $(CFLAGS)

$(PROJECT): $(OBJECTS)
	ar -cvq $@ $^
    

.PHONY: clean

clean:
	del "D:/Code/c/projects/mytui/src/obj/"
	del *.a

test:
	gcc -o test src/test.c -L. -lmytui -Iinclude
