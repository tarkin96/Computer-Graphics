## Compiler
CC=g++
## Linker
LD=$(CC)
## Flags
CPPFLAGS = -Wall -g -DLINUX -std=c++11
LFLAGS = -lglut -L/usr/lib -L/usr/X11R6/lib -lXi -lXmu -lGL -lGLU -lm

TARGETS = $(PROGFILES:.cpp=)

PROGFILES = \
        assn1.cpp \
	Curve.cpp \
        $(NULL)

targets default: $(TARGETS)

$(PROGFILES:.cpp=):
	$(CC) -o assn1 assn1.cpp Curve.cpp ${LFLAGS} ${CPPFLAGS}

depend :
	makedepend ${PROGFILES}
clean: 
	rm assn1
# DO NOT DELETE
