# Makefile for modified sl version
# Created for OpenDrives codetest
# Brian Lambrigger <lambrigger.brian@gmail.com>
# Original copyright below

#==========================================
#    Makefile: makefile for sl 5.1
#	Copyright 1993, 1998, 2014
#                 Toyoda Masashi
#		  (mtoyoda@acm.org)
#	Last Modified: 2014/03/31
#==========================================

MAJOR_VERSION=1
MINOR_VERSION=2

CC=g++
CFLAGS=-O -w

PACKAGE_DIR=debpkgs/sl_$(MAJOR_VERSION).$(MINOR_VERSION)_x86
DEB_DIR=$(PACKAGE_DIR)/DEBIAN
OPT_DIR=$(PACKAGE_DIR)/opt/sl
BIN_DIR=$(PACKAGE_DIR)/usr/bin

all: sl

sl: sl.cpp sl.h ascii.cpp ascii.h vehicles.h
	$(CC) $(CFLAGS) -o sl sl.cpp ascii.cpp -lncurses

debug: sl.cpp sl.h ascii.cpp ascii.h vehicles.h
	$(CC) -Wall $(CFLAGS) -g -o sl_debug sl.cpp ascii.cpp -lncurses

package: sl
	mkdir --parents $(OPT_DIR)
	mkdir $(DEB_DIR)	
	mkdir --parents $(BIN_DIR) 
	cp ./sl $(OPT_DIR)/
	chmod 755 $(OPT_DIR)/sl 
	ln -s /opt/sl/sl $(BIN_DIR)/sl	
	# Command below passes version to script that wraps heredoc template	
	./create_control.sh $(MAJOR_VERSION) $(MINOR_VERSION) > $(DEB_DIR)/control
	dpkg-deb --build $(PACKAGE_DIR)
    
clean:
	rm -f sl
	rm -f sl_debug
	rm -rf debpkgs

distclean: clean
