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
class Lvalue
{
public:
    typedef std::shared_ptr<Lvalue> Ptr;

    virtual ~Lvalue() = default;
    virtual const TypeDecl& getType() const = 0;
    virtual vm::MemoryView getMemoryView(vm::ExecutionContext *context) = 0;
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

    virtual const TypeDecl& getType() const override;
    virtual vm::MemoryView getMemoryView(vm::ExecutionContext *context) override;

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

    virtual const TypeDecl& getType() const override;
    virtual vm::MemoryView getMemoryView(vm::ExecutionContext *context) override;

private:
    std::string _varName;
    int _numDerefs;
    TypeDecl _finalType;
    TypeDecl _declaredType;
};


}
}