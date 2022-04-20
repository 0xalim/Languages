#include <stdio.h>


int main(int argc, char argv[]) {

	int    a = 5;
	double b = 10.0;
	char   c = 'c';

	int*    aPtr = &a;
	double* bPtr = &b;
	char*   cPtr = &c;
	
//	printf("value of a %d, at 0x%x\n", a, aPtr);
//	printf("value of &b %x ", &b);
//	printf("value of c %c, at 0x%x\n", c, cPtr);

	printf("aPtr value: %x\n", aPtr);
	printf("bPtr value: %x\n", bPtr);
	printf("cPtr value: %x\n", cPtr);

}
