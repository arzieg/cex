#include <stdio.h>
#include <string.h>
int main()
{
	char source[20] = "String";
	char destination[20];
	strncpy(destination, source,20);
	printf("\nCopied string is: %s, length=%d\n", destination, strlen(destination));
	return 0;
}
