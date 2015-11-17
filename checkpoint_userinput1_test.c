/*
 * Test function for testing Key-value Store library with user input keys and
 * values with operations GET,PUT,SIZE and DELETE.
 *
 * Author:  Abhijit Shanbhag<abhijit.shanbhag@rutgers.edu>
 * 		    Priyanka Dhingra<pd374@scarletmail.rutgers.edu>
 */
#include<stdio.h>
#include "kvstore.h"
#include<stdlib.h>
#include<string.h>

void print_success() {
	printf("Experiment SUCCESS as all operations succeeded\n");
}

void print_failure(int mode) {
//Mode 1- PUT failed
//Mode 2- Size of stored block is different to size of input block
//Mode 3- GET failed
//Mode 4- input != result
//Mode 5 - Delete failed
	switch (mode) {
	case 1:
		printf("Experiment FAILED as PUT operation failed\n");
		break;
	case 2:
		printf("Experiment FAILED as Size(result block)!= size(input block)\n");
		break;
	case 3:
		printf("Experiment FAILED as GET operation failed\n");
		break;
	case 4:
		printf("Experiment FAILED as (result block)!= (input block)\n");
		break;
	case 5:
		printf("Experiment FAILED as DELETE failed\n");
		break;
	default:
		printf("Experiment something FAILED\n");
		break;
	}
	exit(1);
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("usage: %s key val\n", argv[0]);
		exit(0);
	}
	int i, key, val, sz, comp, status;
	char *input, *result;

	printf("***********Performing experiment**********\n");
	//generate key and value randomly
	key = atoi(argv[1]);
	val = atoi(argv[2]);
	printf(
			"Key generated: %d with value- A string with %d x's of size %d bytes\n",
			key, val, val);
	//Allocate input string
	input = (char *) malloc(val);
	memset(input, '\0', val);
	memset(input, 'x', val - 1);

	printf("SIZE operation before inserting key: %d return size: %d\n", key,
			size(&key));
	status = put(&key, input, val);
	printf("PUT operation on key: %d returns status: %d\n", key, status);
	if (!status) {
		print_failure(1);
	}
	sz = size(&key);
	printf("SIZE operation on key: %d return size: %d\n", key, sz);
	if (sz != val) {
		print_failure(2);
	}

	//allocate result string
	result = (char *) malloc(sz);
	status = get(&key, result, val);
	printf("GET operation on key: %d returns status: %d\n", key, status);
	if (!status) {
		print_failure(3);
	}
	comp = strcmp(input, result);
	if (comp != 0) {
		print_failure(4);
	}
	status = delete(&key);
	printf("DELETE operation on key: %d returns status: %d\n", key, status);
	if (!status) {
		print_failure(5);
	}
	//Cleanup
	free(input);
	free(result);
	print_success();
}

