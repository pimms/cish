#pragma once

#include "../Module.h"

namespace cish::module::string {
/**
* Functions not yet implemented:
*  - strcspn
*  - strpbrk
*  - strrchr
*  - strspn
*  - strtok
*
*  Functions that won't be implemented:
*  - strcoll
*  - strerror
*  - strxfrm
*/
Module::Ptr buildModule();

}

namespace cish::module::string::impl
{

/*
==================
void *memchr(const void *str, int c, int n)
==================
*/
class Memchr : public Function
{
public:
    static ast::FuncDeclaration getSignature();

    Memchr();
    ast::ExpressionValue execute(vm::ExecutionContext *context,
                                 FuncParams params,
                                 vm::Variable*) const override;
};


/*
==================
int memcmp(const void *str1, const void *str2, size_t n)
==================
*/
class Memcmp : public Function
{
public:
    static ast::FuncDeclaration getSignature();
    Memcmp();
    ast::ExpressionValue execute(vm::ExecutionContext *context,
                                 FuncParams params,
                                 vm::Variable*) const override;
};


/*
==================
void *memcpy(void *dest, const void *src, size_t n)
==================
*/
class Memcpy : public Function
{
public:
    static ast::FuncDeclaration getSignature();
    Memcpy();
    ast::ExpressionValue execute(vm::ExecutionContext *context,
                                 FuncParams params,
                                 vm::Variable*) const override;
};


/*
==================
void *memset(void *str, int c, size_t n)
==================
*/
class Memset : public Function
{
public:
    static ast::FuncDeclaration getSignature();
    Memset();
    ast::ExpressionValue execute(vm::ExecutionContext *context,
                                 FuncParams params,
                                 vm::Variable*) const override;
};


/*
==================
char *strcat(char *dest, const char *src)
==================
*/
class Strcat : public Function
{
public:
    static ast::FuncDeclaration getSignature();
    Strcat();
    ast::ExpressionValue execute(vm::ExecutionContext *context,
                                 FuncParams params,
                                 vm::Variable*) const override;
};


/*
==================
char *strncat(char *dest, const char *src, size_t n)
==================
*/
class Strncat : public Function
{
public:
    static ast::FuncDeclaration getSignature();
    Strncat();
    ast::ExpressionValue execute(vm::ExecutionContext *context,
                                 FuncParams params,
                                 vm::Variable*) const override;
};


/*
==================
char *strchr(const char *str, int character)
==================
*/
class Strchr : public Function
{
public:
    static ast::FuncDeclaration getSignature();
    Strchr();
    ast::ExpressionValue execute(vm::ExecutionContext *context,
                                 FuncParams params,
                                 vm::Variable*) const override;
};


/*
==================
char *strstr(const char *haystack, const char *needle)
==================
*/
class Strstr: public Function
{
public:
    static ast::FuncDeclaration getSignature();
    Strstr();
    ast::ExpressionValue execute(vm::ExecutionContext *context,
                                 FuncParams params,
                                 vm::Variable*) const override;
};

/*
==================
int strcmp(const char *str1, const char *str2)
==================
*/
class Strcmp : public Function
{
public:
    static ast::FuncDeclaration getSignature();
    Strcmp();
    ast::ExpressionValue execute(vm::ExecutionContext *context,
                                 FuncParams params,
                                 vm::Variable*) const override;
};


/*
==================
int strncmp(const char *str1, const char *str2, size_t n)
==================
*/
class Strncmp : public Function
{
public:
    static ast::FuncDeclaration getSignature();
    Strncmp();
    ast::ExpressionValue execute(vm::ExecutionContext *context,
                                 FuncParams params,
                                 vm::Variable*) const override;
};


/*
==================
char *strcpy(char *dest, const char *src)
==================
*/
class Strcpy : public Function
{
public:
    static ast::FuncDeclaration getSignature();
    Strcpy();
    ast::ExpressionValue execute(vm::ExecutionContext *context,
                                 FuncParams params,
                                 vm::Variable*) const override;
};


/*
==================
char *strncpy(char *dest, const char *src, size_t n)
==================
*/
class Strncpy : public Function
{
public:
    static ast::FuncDeclaration getSignature();
    Strncpy();
    ast::ExpressionValue execute(vm::ExecutionContext *context,
                                 FuncParams params,
                                 vm::Variable*) const override;
};


/*
==================
size_t strlen (const char *str)
==================
*/
class Strlen : public Function
{
public:
    static ast::FuncDeclaration getSignature();
    Strlen();
    ast::ExpressionValue execute(vm::ExecutionContext *context,
                                 FuncParams params,
                                 vm::Variable*) const override;
};

}
