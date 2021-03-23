#include <stdio.h>
#include "conio.h"
void main() {
	int radius;
	float area,circumf,pi=3.14;
		  area = pi * radius*radius;
		  circumf = pi *radius * 2;

	clrscr();
	printf("Enter radius");
	scanf("%d", &radius);
	printf("area is %f\n", area);
	printf("circumference is %f\n", circumf);
}