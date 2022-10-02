#include "Lvalue.h"
#include "../vm/ExecutionContext.h"

namespace cish::ast
{

/*
==================
Lvalue
==================
*/
ExpressionValue Lvalue::evaluate(vm::ExecutionContext *context) const
{
    return getMemoryView(context).evaluateAs(getType());
}


/*
==================
VariableReference
==================
*/
VariableReference::VariableReference(DeclarationContext *context, const std::string &varName)
{
    const VarDeclaration *decl = context->getVariableDeclaration(varName);
    if (decl == nullptr) {
        Throw(VariableNotDeclaredException,
              "Variable '%s' not declared in the current scope",
              varName.c_str());
    }

    _varDecl = *decl;
}

TypeDecl VariableReference::getType() const
{
    return _varDecl.type;
}

vm::MemoryView VariableReference::getMemoryView(vm::ExecutionContext *context) const
{
    vm::Variable *var = context->getScope()->getVariable(_varDecl.name);
    if (!var) {
        Throw(VariableNotDefinedException,
              "Variable '%s' not defined",
              _varDecl.name.c_str());
    }

    return context->getMemory()->getView(var->getAllocation()->getAddress());
}


/*
==================
DereferencedVariableReference
==================
*/
DereferenceExpression::DereferenceExpression(Expression::Ptr expr):
    _expr(expr)
{
    if (expr->getType() != TypeDecl::POINTER) {
        Throw(InvalidTypeException, "Can only dereference pointer-types");
    }

    if (expr->getType().getReferencedType() == nullptr) {
        Throw(Exception, "Expected a referenced type, found NULL");
    }

    _type = *expr->getType().getReferencedType();
}

TypeDecl DereferenceExpression::getType() const
{
    return _type;
}
 
vm::MemoryView DereferenceExpression::getMemoryView(vm::ExecutionContext *context) const
{
    ExpressionValue value = _expr->evaluate(context);
    const uint32_t addr = value.get<uint32_t>();
    return context->getMemory()->getView(addr);
}


/*
==================
SubscriptExpression
==================
*/
SubscriptExpression::SubscriptExpression(Expression::Ptr ptrExpr, Expression::Ptr indexExpr):
    _ptrExpr(ptrExpr),
    _indexExpr(indexExpr)
{
    const TypeDecl ptrType = ptrExpr->getType();
    if (ptrType != TypeDecl::POINTER) {
        Throw(InvalidOperationException,
              "Cannot use subscript operator on expression of type '%s'", ptrType.getName());
    }

    TypeDecl idxType = indexExpr->getType();
    if (idxType.isFloating() || idxType == TypeDecl::POINTER) {
        Throw(InvalidOperationException,
              "Cannot use expression of type '%s' as subscript index", idxType.getName());
    }

    _intrinsicType = *ptrExpr->getType().getReferencedType();
}

TypeDecl SubscriptExpression::getType() const
{
    return *_ptrExpr->getType().getReferencedType();
}

vm::MemoryView SubscriptExpression::getMemoryView(vm::ExecutionContext *context) const
{
    const int32_t stride = std::max(1, (int32_t)_intrinsicType.getSize());
    const int32_t offset = stride * _indexExpr->evaluate(context).get<int32_t>();
    const uint32_t addr = _ptrExpr->evaluate(context).get<uint32_t>() + offset;
    return context->getMemory()->getView(addr);
}

}
