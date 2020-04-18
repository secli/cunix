#include <stdio.h>
int main()
{
	printf("       Line number: %d\n", __LINE__);
	printf("         File name: %s\n", __FILE__);
	printf("        Today date: %s\n", __DATE__);
	printf("      Current time: %s\n", __TIME__);
	printf("        Standard C: %d\n", __STDC__);
	printf("Standard C version: %ldL\n", __STDC_VERSION__);
}
