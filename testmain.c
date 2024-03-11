#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

int main()
{
	char test[1024];
	getcwd(test, sizeof(test));
	printf("Current working dir: %s\n", test);
}
