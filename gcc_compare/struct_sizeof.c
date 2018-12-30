#include <stdio.h>
#include <stdlib.h>

/**
 * This is potentially super flaky, as GCC performs proper alignment
 * and packing, where CISH does not.
 *
 * Also keep in mind the default word-size of your compiler/cpu when
 * this test starts to flake. CISH integers are always 32 bit.
 */
struct type_t {
    int n;
    int m;
};

int main()
{
    struct type_t t;
    
    printf("sizeof(struct type_t) = %d\n", (int)sizeof(struct type_t));
    printf("sizeof(t) = %d\n", (int)sizeof(t));
    return 0;
}
