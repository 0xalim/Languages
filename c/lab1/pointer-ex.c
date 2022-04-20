#include <stdio.h>

int main(int argc, char** argv) {
	int	a = 5;
	char	b = 'b';
	double  c = 2.718282;
	printf("a is: %d \n", a);
	printf("b is: %c \n", b);
	printf("c is: %f \n", c);
	int*	ptr_a = &a;
	char*	ptr_b = &b;
	double*	ptr_c = &c;
	printf("ptr_a points to (0x%x) with dereferenced value %d\n", ptr_a, *ptr_a);
	printf("ptr_b points to (0x%x) with dereferenced value %c\n", ptr_b, *ptr_b);
	printf("ptr_c points to (0x%x) with dereferenced value %f\n", ptr_c, *ptr_c);
	return 0;
}
