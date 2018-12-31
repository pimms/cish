#include <stdio.h>
#include <stdlib.h>

void add_one(int *arr, int n)
{
    for (int i=0; i<n; i++) {
        arr[i]++;
    }
}

void sub_one(int *arr, int n)
{
    for (int i=0; i<n; i++) {
        --arr[i];
    }
}

void mult_two(int *arr, int n)
{
    for (int i=0; i<n; i++) {
        arr[i] *= 2;
    }
}

void div_three(int *arr, int n)
{
    for (int i=0; i<n; i++) {
        arr[i] = arr[i] / 3;
    }
}

void print_arr(const int *arr, int n)
{
    for (int i=0; i<n; i++) {
        printf("[%d: %d]", i, *(arr + i));
    }
}

int main()
{
    const int N = 10;
    int *buf = (int*)malloc(sizeof(int) * N);

    for (int i=0; i<N; i++) {
        *(buf + i) = 1 << i;
    }

    add_one(buf, N);
    print_arr(buf, N);

    sub_one(buf, N);
    print_arr(buf, N);

    mult_two(buf, N);
    print_arr(buf, N);

    div_three(buf, N);
    print_arr(buf, N);

    free(buf);
    return 0;
}
