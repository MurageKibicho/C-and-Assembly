//simple program to convert temperature from centigrade to fahrenheit
#include <stdio.h>
#include "conio.h"

void main() {
	float c,f;
	clrscr();
	printf("Enter temperature in centigrade");
	scanf("%f", &c);

	f = 1.8 * c + 32;
	printf("\nFAhrenheit: %f\n", f);
}