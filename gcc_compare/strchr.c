#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("is null: %d\n", strchr("abcdeeeeee", 'i') == NULL);
    printf("is null: %d\n", strchr("abcdeeeeee", 0) == NULL);
    printf("is null: %d\n", strchr("abcdeeeeee", 'a') == NULL);
    printf("is null: %d\n", strchr("abcdeeeeee", 'b') == NULL);
    printf("is null: %d\n", strchr("abcdeeeeee", 'e') == NULL);

    printf("%s\n", strchr("www.google.com", '.'));
    printf("%s\n", strchr("www.google.com", 'w'));
    printf("%s\n", strchr("www.google.com", 'g'));
    printf("%s\n", strchr("www.google.com", 'o'));
    printf("%s\n", strchr("www.google.com", 'c'));
    printf("%s\n", strchr("www.google.com", 'm'));
    printf("%s\n", strchr("www.google.com", 0));

    return 0;
}
