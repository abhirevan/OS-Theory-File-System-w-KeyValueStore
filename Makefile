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
all: kvstore_library checkpoint_userinput1_test checkpoint1_test fs test

checkpoint1_test: checkpoint1_test.c kvstore.c kvstore.h
	$(CC) $(CFLAGS) checkpoint1_test.c -o checkpoint1_test.o -L../OS-Theory-File-System-w-KeyValueStore -lkvstorelib -lglib-2.0

checkpoint_userinput1_test: checkpoint_userinput1_test.c kvstore.c kvstore.h
	$(CC) $(CFLAGS) checkpoint_userinput1_test.c -o checkpoint_userinput1_test.o -L../OS-Theory-File-System-w-KeyValueStore -lkvstorelib -lglib-2.0

kvstore_library: kvstore.c kvstore.h 
	$(CC) -c -g kvstore.c kvstore.h -I./  `pkg-config --cflags --libs glib-2.0`
	ar rcs libkvstorelib.a kvstore.o
	
test: test.c
	$(CC) $(CFLAGS) test.c -o test.o -L../OS-Theory-File-System-w-KeyValueStore -lkvstorelib -lglib-2.0

fs: fs.c fs.h fuse.c
	$(CC) $(CFLAGS) -D_FILE_OFFSET_BITS=64 -lfuse fuse.c fs.c -o fs.o -L../OS-Theory-File-System-w-KeyValueStore -lkvstorelib -lglib-2.0
	
clean:
	@echo "Cleaning everything"
	@rm -f *.a *.o .datafile1b  .datafile1kb  .datafile1mb  .datafile512kb  .kvstoremapping  .kvstoremeta fs
