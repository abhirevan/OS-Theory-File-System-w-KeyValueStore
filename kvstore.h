/*
 * Key value store library for storing value block for some key
 *
 * supported operations size,put,get and delete
 *
 * Author:  Abhijit Shanbhag<abhijit.shanbhag@rutgers.edu>
 * 		    Priyanka Dhingra<pd374@scarletmail.rutgers.edu>
 */
/*********************************************************************
 * Library functions
 ********************************************************************/
int size(void* keyPtr);
int put(void* keyPtr, void* valPtr, int size);
int get(void* keyPtr, void* valPtr, int size);
int delete(void* keyPtr);
