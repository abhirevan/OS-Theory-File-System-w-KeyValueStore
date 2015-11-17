/*
 * Test function for testing Key-value Store library for basic functionality with edge cases
 *
 * Author:  Abhijit Shanbhag<abhijit.shanbhag@rutgers.edu>
 * 		    Priyanka Dhingra<pd374@scarletmail.rutgers.edu>
 */
#include "kvstore.h"
#include<stdio.h>
#include<string.h>

int main() {
	char val1[100];
	/*
	 int k1 = 55;
	 printf("size for key1:%d: is %d\n", k1, size(&k1));

	 if (put(&k1, "value1", 7)) {
	 printf("put done:\n");
	 } else {
	 printf("ERROR in put:\n");
	 }
	 printf("size for key1:%d: is %d\n", k1, size(&k1));

	 int k2 = 20;
	 printf("size for key1:%d: is %d\n", k2, size(&k2));

	 if (get(&k1, val1, 50)) {
	 printf("get success done:\n");
	 printf("value for key1:%d: is %s\n", k1, val1);

	 } else {
	 printf("key not found:\n");
	 }
	 */

	int k1 = 100;

	if (put(&k1, "value1", 7)) {
		printf("put done:\n");
	} else {
		printf("ERROR in put:\n");
	}
	if (get(&k1, val1, 50)) {
		printf("get success done:\n");
		printf("value for key:%d: is %s\n", k1, val1);
	} else {
		printf("key not found:\n");
	}
	printf("size for key:%d: is %d\n", k1, size(&k1));
	if (delete(&k1)) {
		printf("delete done:\n");
	} else {
		printf("delete error:\n");
	}

}

