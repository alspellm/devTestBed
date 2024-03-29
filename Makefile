CXXFLAGS=$(shell root-config --cflags)
GLIBS=$(shell root-config --glibs)

SRCS:=$(wildcard src/*.cxx)

all: hello

hello: $(SRCS)
	g++ $(CXXFLAGS) $^ -o $@ -I./include/ $(GLIBS)
