CC=clang++


CFLAGS= -std=c++11 -O3 -Wall -g
LINKFLAGS=-O3

#debug = true
ifdef debug
	CFLAGS +=-g
	LINKFLAGS += -flto
endif

INCDIR= -I./middleware -Imiddleware/glad/include -Imiddleware/glm-0.9.8.2

LIBDIR=

LIBS=

OS_NAME:=$(shell uname -s)

ifeq ($(OS_NAME),Darwin)
	LIBS += `pkg-config --static --libs glfw3 gl`
endif
ifeq ($(OS_NAME),Linux)
	LIBS += `pkg-config --static --libs glfw3 gl`
endif

SRCDIR=./

SRCLIST=$(wildcard $(SRCDIR)/*cpp)

HEADERDIR=./

OBJDIR=./obj

OBJLIST=$(addprefix $(OBJDIR)/,$(notdir $(SRCLIST:.cpp=.o))) $(OBJDIR)/glad.o

EXECUTABLE= Boilerplate.out

all: buildDirectories $(EXECUTABLE)

$(EXECUTABLE): $(OBJLIST)
	$(CC) $(LINKFLAGS) $(OBJLIST) -o $@ $(LIBS) $(LIBDIR)

$(OBJDIR)/glad.o: glad.c
	$(CC) -c $(CFLAGS) -I$(HEADERDIR) $(INCDIR) $(LIBDIR) $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) -c $(CFLAGS) -I$(HEADERDIR) $(INCDIR) $(LIBDIR) $< -o $@


.PHONY: buildDirectories
buildDirectories:
	mkdir -p $(OBJDIR)

.PHONY: clean
clean:
	rm -f *.out $(OBJDIR)/*.o; rmdir obj;
