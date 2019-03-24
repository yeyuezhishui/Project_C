#include<stdio.h> 
#include "mylibrary.h" 

int main(void)
{
	int i = 2017;
	float f = 1.1;
	double ff = 1.11;
	char c='m';
	char array[4]="yxy";
	myprintf ("hellow, %d, %c, %s, %f, %lf\n", i, c, array, f,ff);
	myscanf ("%d%f%lf%c", &i, &f, &ff, &c);
	myprintf ("hellow, %d, %c, %s, %f, %lf\n", i, c, array, f,ff);
	mygets(array);
	myputs(array);
	return 0;
}
