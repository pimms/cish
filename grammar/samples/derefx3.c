int main() {
   int core = 0;
   int *ptr1 = &core;
   int **ptr2 = &ptr1;
   int ***ptr3 = &ptr2;
   ***ptr3 = 9;
   return core;
}

