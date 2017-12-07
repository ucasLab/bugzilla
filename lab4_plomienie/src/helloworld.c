/*
 ============================================================================
 Name        : helloworld.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
extern void strcopy(char *d, const char *s);
int main(void) {
	const char *srcstr = "First string - source ";
	    char dststr[] = "Second string - destination ";
	/* dststr is an array since weﾒre going to change it */
	    printf("Before copying:\n");
	    printf("  %s\n  %s\n",srcstr,dststr);
	    strcopy(dststr,srcstr);
	    printf("After copying:\n");
	    printf("  %s\n  %s\n",srcstr,dststr);
	    return (0);
}
