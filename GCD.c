#include<stdio.h>
#include<GCD.h>

int main(){
	int a,b;
	printf("Please input two integers\n");
	scanf("%d %d",&a,&b);
	printf("The GCD of a and b is:%d \n",GCD(a,b));
}

