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
	gcc -o test test/test.c -L. -lmytui -Iinclude
	gcc -o test2 test/test2.c -L. -lmytui -Iinclude
	gcc -o test3 test/test3.c -L. -lmytui -Iinclude
	gcc -o test4 test/test4.c -L. -lmytui -Iinclude

cleantest:
	del ".\test.exe"
	del ".\test2.exe"
	del ".\test3.exe"
	del ".\test4.exe"
