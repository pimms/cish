#pragma once

#include "DeclarationContext.h"
#include "../vm/Allocation.h"

#include <memory>

namespace cish
{
namespace vm { class ExecutionContext; }
namespace ast
{


/*
==================
Lvalue
==================
*/
class Lvalue: public Expression
{
public:
    typedef std::shared_ptr<Lvalue> Ptr;

    virtual ~Lvalue() = default;
    virtual ExpressionValue evaluate(vm::ExecutionContext *context) const override;
    virtual vm::MemoryView getMemoryView(vm::ExecutionContext *context) const = 0;
};


/*
==================
VariableReference
==================
*/
class VariableReference: public Lvalue
{
public:
    VariableReference(DeclarationContext *context, const std::string &varName);
    virtual ~VariableReference() = default;

    virtual TypeDecl getType() const override;
    virtual vm::MemoryView getMemoryView(vm::ExecutionContext *context) const override;

private:
    VarDeclaration _varDecl;
};


/*
==================
DereferencedVariableReference
==================
*/
class DereferencedVariableReference: public Lvalue
{
public:
    DereferencedVariableReference(DeclarationContext *context, const std::string &varName, int numDerefs);
    virtual ~DereferencedVariableReference() = default;

    virtual TypeDecl getType() const override;
    virtual vm::MemoryView getMemoryView(vm::ExecutionContext *context) const override;

private:
    std::string _varName;
    int _numDerefs;
    TypeDecl _finalType;
    TypeDecl _declaredType;
};


}
}
