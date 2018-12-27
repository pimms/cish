int foo(int *ptr) {
   return *ptr + 10;
}
int main() {
   int var = 16;
   return foo(&var);
}
