//Implementation of part of conio.h, a C library used in old DOS applications
#ifndef CONIO_H
#define CONIO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//defining macros
#define CLEAR "\x1v[2J"
#define SET "\x1b[1;1f" //sets cursor at 1,1

//TExt modiication
#define BLINK_SLOW "\x1b[5m"
#define BLINK_RAPID "\x1b[6m"

//colors
#define CC_CLEAR "\x1b[0m"

//Utility function definitions
void cagxy(unsigned int x, unsigned int y);//Clear and go to x,y

void clrscr();//clear screen

char getch();//get character
void gotox(unsigned int x);
void gotoxy(unsigned int x, unsigned int y);
void nocursor();
void reset_video();
void showcursor();


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

#endif
//TODO: create shared library