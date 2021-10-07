#include <stdio.h>

int main(int argc, char* argv[])
{

	double number1 = 7.3;
	double number2;
	char*  ptr = NULL;
	char   s1[100], s2[100];
	
	double* dPtr;		// A
	dPtr = &number1;	// B
	printf("value pointed to by dPtr: %f\n", *dPtr);	// C

	number2 = *dPtr;					// D
	printf("value of number2: %f\n", number2);		// E
	printf("address of number1: 0x%x\n", &number1);		// F
	printf("address stored in dPtr: 0x%x\n", *dPtr);	// G

	s2 == s1;
	printf("%s\n", s1);
	printf("%s\n", s2);

}
