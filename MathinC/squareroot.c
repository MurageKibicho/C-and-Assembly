//Babylonian method
#include <stdio.h>

float squareRoot(float n) {
	float x = n;
	float y = 1;
	float e = 0.00001;
	while (x-y > e) {
		x = (x +y) / 2;
		y = n/x;
	}
	return x;
}
int main () {
	int n = 50;
	printf("Square root od %d is %f", n, squareRoot(n));
	getchar();
}