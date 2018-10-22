int main()
{
    printf("Hello, World!");

    const int asInt = 15;
    const float asFloat = (float)*(&asInt) + 1.3f;
    const char *floatStr = "float";
    printf("As int: %d\n", asInt);
    printf("As %s: %f\n", floatStr, asFloat);
    return 0;
}
