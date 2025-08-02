#include <stdio.h>
#include <stdlib.h>

int main()
{
  int v1 = -2147483637;

  int ret = atoi("-2147483637");

  printf("%u\n", (unsigned int)(v1 * 4));
  printf("%d\n", ret);
}
