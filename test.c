#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "kvstore.h"

int main() {
	int key = 3;
	int input = 100;
	int status,sz;
	void* result;

	status = put(&key, &input, sizeof(input));

	printf("PUT operation on key: %d returns status: %d\n", key, status);
	sz = size(&key);
	printf("SIZE operation on key: %d return size: %d\n", key, sz);
	result = (char *) malloc(sz);
	status = get(&key, result, sz);
	printf("GET operation on key: %d returns status: %d\n", key, status);
	printf("Result: %d \n", *((int*)result));
}
