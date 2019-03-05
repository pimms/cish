# Struct Implementation

I've tried to implement structs as true to the real-world implementations as
possible, with a structure only being a transparent mask overlaid arbitrary
memory.

The "registers" of Cish _(`cish::ast::ExpressionValue`)_ are also quite true to
their real-world counterpart, in that they are severely restricted in size. An
`ExpressionValue` can never exceed 8 bytes _(for doubles)_, and is therefore
unsuited for storing rich structures.

References to struct-objects are implemented as pointers, as a struct
_(almost)_ always is an lvalue, and therefore has an associated memory address.
The only exception to this is when a structure is returned from a function.


### Passing & assigning structs by value

Consider the following program:

    struct item_t { int a; int b; };

    /* -- 3 -- */
    void foo(struct item_t i) { /* ... */ }

    int main() {
        /* -- 1 -- */
        struct item_t item;
        ...

        /* -- 2 -- */
        foo(item);
    }

1. Declaring the struct
   The struct `item` is allocated and given 8 bytes of memory.
2. Calling `foo`
   Behind the scenes, `cish` only passes the memory address of `item`. The
   corresponding `ExpressionValue` has an intrinsic type of `TypeDecl::STRUCT`.
3. `foo` allocates its parameters
   When a function is called in Cish, the function is given an array of
   `ExpressionValue` objects that are allocated in the VM memory before the
   function executes. When a function parameter is of type `STRUCT`, it copies
   the number of bytes that struct occupies from the address stored in the
   `ExpressionValue` and stores it in a newly allocated variable.


### Returning a struct by value

Consider the following program:

    struct item_t { int a; int b; int c; };
    struct item_t foo();
    void bar(struct item_t item);
    int main() {
        /* -- 1 -- */
        struct item_t item = foo();
        /* -- 2 -- */
        bar(foo());
    }

Both of these calls are **much** easier to evaluate if the expression `foo()`
resulted in a variable allocated at the call-site. **1** can treat the call in
no different way from a normal assignment-by-value, and **2** can simply pass
along the memory address returned from `foo()`.

The typical ABI for complex return structures is that the call-site itself
allocates the space in which the function will place the rich structure. With
this method, the ephemeral return value has a memory address, and we can simply
deal with it as any other struct.

However, this has some complicatations. A `cish::ast::Expression` has no
insight in it's encapsuling lifecycle, apart from the instant it is being
evaluated. Without this insight, we'd have to allocate the ephemeral variable
in the current scope, which would end up *"soft-leaking"* memory. Consider
the following scope:

    {
        foo();
        foo();
        foo();
        foo();
    }

This scope should not allocate more than exactly **one** `struct item_t`.

In order to achieve this, the `cish::ast::Statement` is able to allocate
ephemeral storage that will be valid only within the current `execute`-call.
The `cish::vm::ExecutionContext` exposes the current `Statement`, which the
needing `Expressions` can utilize to allocate temporary variables.

