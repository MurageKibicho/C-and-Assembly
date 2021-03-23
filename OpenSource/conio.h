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

#endif
