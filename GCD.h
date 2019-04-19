#include<stdio.h>

int GCD(int a,int b){
	if(a%b==0)
		return b;
	else
		return GCD(b,a%b);
}


