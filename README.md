# C. Ish.

Yet another (mostly) functional C interpreter.

I wrote this to use as the central component of a game centered around
programming low-level devices, similar to "Shenzen I/O" and "TIS-100", but
realized after beginning on the actual "game part" of the game that I
**really** don't like Unity or C#.

Now I don't know what to do with it.

    ¯\_(ツ)_/¯

### Usage

Cish only works with one file at a time. Like with an actual C program, there
must be a `main` function defined. The standard library can be included and
used, although it's mostly unimplemented. The most useful functions from
`<string.h>` and `<stdlib.h>` _(and a few from `<stdio.h>`)_ are implemented.

The command line client `cish_cli` only supports a single argument, which must
be a valid C source file.

### Building

Antlr 4 must be installed on your system. Via Homebrew I've installed `antlr`
and `antlr4-cpp-runtime`, I'm not sure if both are needed.

    mkdir build
    cd build
    cmake ..
    make

    (optionally)
    make install

The make-script builds two artifacts - `libcish.a` and _(unless excluded via)
CMake options)_ `cish_cli`. The library can be used to add C interpretation to
your favorite application, and the command line client can be used to combine a
subset of C with the speed of a turtle.

#### Testing

All of Cish is under heavy unit-test barrage from `gtest`, but there are also
multiple _cish vs gcc_ test-programs in the `gcc_compare` directory. The latter
executes a simple program and compares `stdout` and `$?`. I've found that if I
ever break something, no matter how trivial, it breaks at least one of the many
tests.

`make check` runs the unit-tests, and `gcc_compare/compare.sh` runs the gcc
comparison tests.

### Major missing features:

- Most of the standard library
- Switch-statements
- Literal arrays
- Break & continue
- Enums
- Main does not support command-line arguments

# Technical Details

## Syntax Parsing

Antlr is used for parsing the AST, and is converted _(nearly)_ one-to-one to
an _"execution-tree"_.

Antlr is used to (a) make sure that the program is syntactically valid, and (b)
express the program in a manageable structure. Each node in the AST is then
visited and (often) converted into either an `Expression` or a `Statement`, the
sum of which makes up the "execution tree". During this conversion, it is
verified that the program makes _logical_ and _functional_ sense. Referred
variables must have been declared, expected parameters must be passed,
dereferencing an `int` makes no sense, etc.

Note that because I have no idea what I'm really doing, the "execution tree" I'm
referring to is called `cish::AST` in the code :)

## Virtual Machine

### Memory Allocation

Cish attempts to emulate the real workings of C as closely as possible, and
with it unsafe pointers, wild-west style casting & other neat tricks. The Cish
VM does however not have the concept of "stack" or "heap". Instead, everything
is allocated in a "heap-like" fashion.

### Execution

The execution tree consists of `Statement` and `Expression` nodes. The nodes
are _intrinsically (recursively?) evaluated_, so the execution process really
only involves finding the node that defines the `main()`-function and
evaluating it. It will in turn evaluate all of its child-nodes, and so on until
the program returns.

