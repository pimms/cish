// cish_cli -m 1000000
#include <stdio.h>
#include <stdlib.h>

void swap(int *x,int *y);
int choose_pivot(int i,int j );
void quicksort(int *list,int m,int n);
void display(int *list,const int n);
int prng();

int main()
{
    const int SIZE = 50;
    int *list = (int*)malloc(sizeof(int) * SIZE);

    int i = 0;

    for(i = 0; i < SIZE; i++ )
        list[i] = prng();

    printf("The list before sorting is:\n");
    display(list, SIZE);

    quicksort(list, 0, SIZE-1);

    printf("\n\nThe list after sorting:\n");
    display(list, SIZE);
    return 0;
}

void swap(int *x, int *y)
{
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

int choose_pivot(int i,int j )
{
    return ((i+j) / 2);
}

void quicksort(int *list,int m,int n)
{
    if (m < n) {
        int k = choose_pivot(m,n);
        swap(&list[m], &list[k]);
        int key = list[m];
        int i = m+1;
        int j = n;
        while (i <= j) {
            while ((i <= n) && (list[i] <= key))
                i++;
            while ((j >= m) && (list[j] > key))
                j--;
            if (i < j)
                swap(&list[i],&list[j]);
        }

        swap(&list[m], &list[j]);

        quicksort(list,m,j-1);
        quicksort(list,j+1,n);
    }
}

void display(int *list,const int n)
{
    for (int i=0; i<n; i++) {
        printf("%d\t", list[i]);
    }
}

int g_next = 938;
int prng()
{
    g_next += 458929;
    g_next %= 3173;
    return g_next;
}

