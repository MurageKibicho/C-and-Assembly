//Finds sum of two numbers, will create separate header file later
#include <stdio.h>
#include <stdlib.h>
//function definition
void clrscr();
char getch();

//function to clearscreen
void clrscr(){
	printf("\x1b[2J");
}

char getch(){
	char c;
	system("stty raw");
	system("stty -echo");
	c = getchar();
	system("stty cooked");
	system("stty echo");
	return c;

}

void main(){
	int a, b,s;
	clrscr();
	printf("Enter two nmbers\n");
	scanf("%d%d", &a, &b);
	s = a + b;
	printf("sum = %d", s);
	getch();
}