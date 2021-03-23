//ALgo to find roots for f(x) = 0
//Tangent line at a guess point and x intercept used

#include <stdio.h>
#include <math.h>
#define EPS 1.0e-10

double f(double x) {
	return x*x-2;
}

double fp(double x) {
	return 2*x;

}

double newton(double x) {
	return x-f(x)/fp(x);
}
int main() {
	double x1, x2;
	int i;
	
	do {printf("ENter initial guess = ");
	scanf("%lf", &x1);
}
 	while (fp(x1) == 0.0);
 	for ( i = 0; i <100; i++) {
 		x2 = newton(x1);
 	
 	if (fabs(x1-x2)< EPS) break;
 	x1 = x2;
 }
 printf("iteration = %d\n", i);
 printf("x + %lf\n", x1);
 return 0;
}