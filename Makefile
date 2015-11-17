CC=gcc
CFLAGS=-g

default: help

help: 
	@echo "make all - to build library and create all object files"
	@echo "make clean - to clean everything"
	@echo "make kvstore_test - to build the test case"
	@echo "make checkpoint1_test - to build the test case"
	@echo "make checkpoint_userinput1_test - to build the test case"
	@echo "make library - to create library libkvstorelib.a"
all: library kvstore_test checkpoint_userinput1_test checkpoint1_test

kvstore_test: kvstore_test.c kvstore.c kvstore.h 
	$(CC) $(CFLAGS) kvstore_test.c -o kvstore_test.o -L../OS-Theory-File-System-w-KeyValueStore -lkvstorelib -lglib-2.0
checkpoint1_test: checkpoint1_test.c kvstore.c kvstore.h
	$(CC) $(CFLAGS) checkpoint1_test.c -o checkpoint1_test.o -L../OS-Theory-File-System-w-KeyValueStore -lkvstorelib -lglib-2.0

checkpoint_userinput1_test: checkpoint_userinput1_test.c kvstore.c kvstore.h
	$(CC) $(CFLAGS) checkpoint_userinput1_test.c -o checkpoint_userinput1_test.o -L../OS-Theory-File-System-w-KeyValueStore -lkvstorelib -lglib-2.0

library: kvstore.c kvstore.h 
	$(CC) -c -g kvstore.c kvstore.h -I./  `pkg-config --cflags --libs glib-2.0`
	ar rcs libkvstorelib.a kvstore.o
clean:
	@echo "Cleaning everything"
	@rm -f *.a *.o .datafile1b  .datafile1kb  .datafile1mb  .datafile512kb  .kvstoremapping  .kvstoremeta

test: test.c
	$(CC) $(CFLAGS) test.c -o test.o `pkg-config --cflags --libs glib-2.0`