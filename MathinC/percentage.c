#include <stdio.h>
#include "conio.h"

void main() {
	int a,b,c,d,sum, total = 500;
	float percentage;
	clrscr();
	printf("Enter marks");
	scanf("%d%d%d%d", &a, &b, &c, &d);
	sum = a + b +c +d;
	printf("\n sum = %d\n", sum);
	percentage = sum *100/total;
	printf("percentage: %f",percentage);
	getch();

}