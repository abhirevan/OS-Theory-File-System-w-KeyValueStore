# OS-Theory-KeyValueStore
Simple key value store to be used later for building a file system


This directory contains the following files:

  1. README:       this file
  2. Makefile:   	make all - to build library and create all object files
		make clean - to clean everything
		make kvstore_test - to build the test case
		make checkpoint1_test - to build the test case
		make library - to create library libkvstorelib.a
  3. kvstore.[c|h]:source code for the key value implementation

  4. kvstore_test:	Test function for testing Key-value Store library for basic functionality with edge cases

  5. checkpoint_test1:Test function for testing Key-value Store library with random generated keys and values with operations GET,PUT,SIZE and DELETE.
  
  6. checkpoint_userinput1_test:Test function for testing Key-value Store library with user input keys and values with operations GET,PUT,SIZE and DELETE.

