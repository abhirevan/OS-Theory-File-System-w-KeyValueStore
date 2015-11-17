/*
 * Test function for testing Key-value Store library with random generated keys and
 * values with operations GET,PUT,SIZE and DELETE.
 *
 * Author:  Abhijit Shanbhag<abhijit.shanbhag@rutgers.edu>
 * 		    Priyanka Dhingra<pd374@scarletmail.rutgers.edu>
 */
#include<stdio.h>
#include "kvstore.h"
#include<stdlib.h>
#include<string.h>

#define MAX_EXPERIMENTS 1000
#define RAND_LIMIT 1024*1024

void print_success(int i) {
	printf("Experiment: %d SUCCESS as all operations succeeded\n", i + 1);
}

void print_failure(int i, int mode) {
//Mode 1- PUT failed
//Mode 2- Size of stored block is different to size of input block
//Mode 3- GET failed
//Mode 4- input != result
//Mode 5 - Delete failed

	switch (mode) {
	case 1:
		printf("Experiment: %d FAILED as PUT operation failed\n", i + 1);
		break;
	case 2:
		printf(
				"Experiment: %d FAILED as Size(result block)!= size(input block)\n",
				i + 1);
		break;
	case 3:
		printf("Experiment: %d FAILED as GET operation failed\n", i + 1);
		break;
	case 4:
		printf("Experiment: %d FAILED as (result block)!= (input block)\n",
				i + 1);
		break;
	case 5:
		printf("Experiment: %d FAILED as DELETE failed\n", i + 1);
		break;
	default:
		printf("Experiment: %d something FAILED\n", i + 1);
		break;
	}
	exit(1);
}

int main() {
	printf("Performing %d experiments with Key Value store\n", MAX_EXPERIMENTS);
	int i, key, val, sz, comp, status;
	char *input, *result;
	for (i = 0; i < MAX_EXPERIMENTS; i++) {
		printf("***********Performing experiment %d**********\n", i + 1);
		//generate key and value randomly
		key = rand() % RAND_LIMIT;
		do {

			val = rand() % RAND_LIMIT;
			if (val != 0)
				break;
		} while (1);
		printf(
				"Key generated: %d with value- A string with %d x's of size %d bytes\n",
				key, val, val);
		//Allocate input string
		input = (char *) malloc(val);
		memset(input, '\0', val);
		memset(input, 'x', val - 1);
		//printf("Value generated: %s\n", str);

		printf("SIZE operation before inserting key: %d return size: %d\n", key,
				size(&key));
		status = put(&key, input, val);
		printf("PUT operation on key: %d returns status: %d\n", key, status);
		if (!status) {
			print_failure(i, 1);
		}
		sz = size(&key);
		printf("SIZE operation on key: %d return size: %d\n", key, sz);
		if (sz != val) {
			print_failure(i, 2);
		}

		//allocate result string
		result = (char *) malloc(sz);
		status = get(&key, result, sz);
		printf("GET operation on key: %d returns status: %d\n", key, status);
		if (!status) {
			print_failure(i, 3);
		}
		comp = strcmp(input, result);
		if (comp != 0) {
			print_failure(i, 4);
		}
		status = delete(&key);
		printf("DELETE operation on key: %d returns status: %d\n", key, status);
		if (!status) {
			print_failure(i, 5);
		}
		//Cleanup
		free(input);
		free(result);
		print_success(i);
	}

}

