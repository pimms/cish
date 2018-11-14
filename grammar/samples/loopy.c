
void foo(int n)
{
	while (n < 100) {
		n = n + 1;
	}

	int a = 0;
	do {
		a = a + 1;
		if (a >= 50)
			return;
	} while (a < 10);
}


int main()
{
	for (int i=0; i<10; i++) {
		foo(i);
	}

	return 0;
}
