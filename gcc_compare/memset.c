#include <string.h>
#include <stdio.h>
#include <stdlib.h>

const int N = 9;
void *buf = NULL;

void print_buf()
{
    for (int i=0; i<N; i++)
        printf("%c", ((char*)buf)[i]);
    puts("");
}

int main()
{
    buf = malloc(N);

    memset(buf, 'a', N);
    print_buf();

    for (int i=9; i>0; i--) {
        memset(buf, '0' + i, i);
        print_buf();
    }

    for (int i=0; i<N; i++) {
        memset(((char*)buf) + i, 'a'+i, N-i);
        print_buf();
    }

    free(buf);
    return 0;
}
