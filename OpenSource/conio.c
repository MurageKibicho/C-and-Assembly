#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conio.h"

//Utility functions
void cagxy(unsigned int x, unsigned int y)//Clear and go to x,y
{
printf("%s\x1b[%d;%df", CLEAR, y,x);
}

void clrscr()//clear screen
{
	printf("%s", CLEAR);
}

char getch()//get character
{
	char c;
	system("stty raw");
	system("stty -echo");
	c = getchar();
	system("stty cooked");
	system("stty echo");
	return c;
}

void gotox(unsigned int x)
{
	printf("\x1b[%dG",x );
}

void gotoxy(unsigned int x, unsigned int y) 
{
	printf("\x1b[%d,%df", y, x);
}
void nocursor()
{
	printf("\x1b[?25L");
}

void reset_video()
{
	printf("\x1b[0m");
}
void showcursor() {
	printf("\x1b[?25h");
}
//TODO: create shared library
