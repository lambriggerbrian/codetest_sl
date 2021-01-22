#==========================================
#    Makefile: makefile for sl 5.1
#	Copyright 1993, 1998, 2014
#                 Toyoda Masashi
#		  (mtoyoda@acm.org)
#	Last Modified: 2014/03/31
#==========================================

MAJOR_VERSION=0
MINOR_VERSION=5

CC=g++
CFLAGS=-O -Wall

PACKAGE_DIR=debpkgs/sl_$(MAJOR_VERSION).$(MINOR_VERSION)_x86
DEB_DIR=$(PACKAGE_DIR)/DEBIAN
OPT_DIR=$(PACKAGE_DIR)/opt/sl
BIN_DIR=$(PACKAGE_DIR)/usr/bin

all: sl

sl: sl.c sl.h
	$(CC) $(CFLAGS) -o sl sl.c -lncurses

debug: sl.c sl.h
	$(CC) $(CFLAGS) -g -o sl_debug sl.c -lncurses

package: sl
	mkdir --parents $(OPT_DIR)
	mkdir $(DEB_DIR)	
	mkdir --parents $(BIN_DIR) 
	cp ./sl $(OPT_DIR)/
	chmod 755 $(OPT_DIR)/sl
	ln -s $(OPT_DIR)/sl $(BIN_DIR)/sl	
	./create_control.sh $(MAJOR_VERSION) $(MINOR_VERSION) > $(DEB_DIR)/control

clean:
	rm -f sl
	rm -f sl_debug
	rm -rf debpkgs

distclean: clean
