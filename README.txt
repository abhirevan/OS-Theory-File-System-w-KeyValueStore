This directory contains the following files:

  README:       this file

  Makefile:   	make all - to build library and create all object files
		make clean - to clean everything
		make kvstore_test - to build the test case
		make checkpoint1_test - to build the test case
		make library - to create library libkvstorelib.a
  
  kvstore.[c|h]:source code for the key value implementation

  kvstore_test:	Test function for testing Key-value Store library for basic func		tionality with edge cases

  checkpoint_test1:Test function for testing Key-value Store library with random 			generated keys and values with operations GET,PUT,SIZE and DELETE.
