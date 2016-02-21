
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <iconv.h>
#include <errno.h>
#include <string.h>
int main ()
{
    //char in_string[] = "\xB6\xE2ʸ\xC2\xCE";
    char in_string[] = "王海军";
    char * out_string = NULL;
    out_string[1]='1';
	printf ("Final iconv output: %s\n", out_string);
    return 0;
}
