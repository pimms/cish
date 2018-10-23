const int CONSTANT = 100;

int fib(int n);

// Externally defined somewhere
int print_num(const char *format, int /* omitted as edge case */);

void foo()
{
    int v = fib(15) + 10;
}

int fib(int n)
{
    int v = n + 2;
    print_num("n: %d\n", v);
    return v;
}
