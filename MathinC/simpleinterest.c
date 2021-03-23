#include <stdio.h>
#include "conio.h"

void main() {
	int p,r,t,sinterest;
	clrscr();
	printf("Enter principle, interest rate, time");
	scanf("%d %d %d", &p, &r, &t);
	sinterest = p * r *t;
	printf("\nSimple interest = %d",sinterest);
	getch();
}