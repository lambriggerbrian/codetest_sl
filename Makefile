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

MAJOR=1
MINOR=4

CC=g++
CFLAGS=-O -w

PACKAGEDIR=codetest-sl-$(MAJOR).$(MINOR)
ORIGDIR=codetest-sl-$(MAJOR).$(MINOR).orig

SOURCE=art ascii.cpp ascii.h sl.cpp sl.h sl.1 sl.1.ja Makefile LICENSE README.md README.ja.md
CONTORL=control 

all: sl

sl: sl.cpp sl.h ascii.cpp ascii.h
	$(CC) $(CFLAGS) -o sl sl.cpp ascii.cpp -lncurses

debug: sl.cpp sl.h ascii.cpp ascii.h
	$(CC) -Wall $(CFLAGS) -g -o sl_debug sl.cpp ascii.cpp -lncurses

install: sl
	mkdir /opt/sl   
	install -m 0755 sl /opt/sl/
	ln -s /opt/sl/sl /usr/bin/sl

package: clean
	mkdir $(PACKAGEDIR) $(ORIGDIR)
	cp -t $(PACKAGEDIR) -r $(SOURCE)   
	cd $(PACKAGEDIR) && dh_make --createorig -s -c gpl2	
	cp -r ./debian $(PACKAGEDIR)/	
	cd $(PACKAGEDIR) && dch -v $(MAJOR).$(MINOR) && debuild -b -us -uc

clean:
	rm -f sl
	rm -f sl_debug
	rm -rf codetest-sl*
	rm -rf codetest_sl*

distclean: clean
