//Swap two numbers without using third variable
#include <stdio.h>
#include "conio.h"
//Might not work well with big numbers
void main() {
	int a,b;
	clrscr();
	printf("Enter value for a and b\n");
	scanf("%d\n%d",&a,&b);
	a+=b;
	b-=a;
	a-=b;
	printf("After swap: %d %d",a, b);
	getch();
}