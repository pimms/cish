#pragma once

#include "DeclarationContext.h"
#include "../vm/Allocation.h"

#include <memory>

namespace cish::vm
{
class ExecutionContext;
}

namespace cish::ast
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

    /**
     * IMPORTANT!!! This method MUST be considered to have side-effects, and must only
     * be called once per evaluation.
     */
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
class DereferenceExpression: public Lvalue
{
public:
    DereferenceExpression(Expression::Ptr expression);
    virtual ~DereferenceExpression() = default;

    virtual TypeDecl getType() const override;
    virtual vm::MemoryView getMemoryView(vm::ExecutionContext *context) const override;

private:
    Expression::Ptr _expr;
    TypeDecl _type;
};


/*
==================
SubscriptExpression
==================
*/
class SubscriptExpression: public Lvalue
{
public:
    SubscriptExpression(Expression::Ptr ptrExpr, Expression::Ptr indexExpr);

    virtual TypeDecl getType() const override;
    virtual vm::MemoryView getMemoryView(vm::ExecutionContext *context) const override;

private:
    Expression::Ptr _ptrExpr;
    Expression::Ptr _indexExpr;

    TypeDecl _intrinsicType;
};

}
