int main() {
   int value = 13;
   int *ptr = &value;
   int *ptr1 = ptr+ 1;
   int *ptr2 = &ptr[1];
   int *ptr3 = &(ptr[1]);
   return ptr1 == ptr2 && ptr1 == ptr3;
}
