ARGS = -Wall -fPIC -O -g
OS = linux
SOURCEDIR = source/$(OS)/
SOURCE = $(SOURCEDIR)rsgl.cpp
LIBDIR = $(SOURCEDIR)/deps
LIBS = $(LIBDIR)/libX11.so.6.4.0 $(LIBDIR)/libpng16.so.16.37.0 $(LIBDIR)/libcairo.so.2.11704.0
GXX = g++


build:
	@$(GXX) -c $(ARGS) $(LIBS) $(SOURCE) -c -o rsgl.o
	@$(GXX) -shared  rsgl.o $(LIBS) -o libRSGL.so
	rm rsgl.o;	

install:
	@make build
	@sudo cp libRSGL.so /usr/lib/libRSGL.so
	@rm libRSGL.so

all:
	build
	install

help:
	@echo "make help : runs this help tab"
	@echo "make : runs both build and install"
	@echo "make build : builds libRSGL.so in the local dir"
	@echo "make install : installs the build into /usr/lib, then removes in, runs buld if it's not already build"