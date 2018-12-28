#include <stdlib.h>

int main()
{
  return "foo1" != NULL
	 && "foo2" != (const char *) ((void *) 0)
	 && "foo5" != "foo6";
}
