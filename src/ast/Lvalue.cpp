#include "Lvalue.h"
#include "../vm/ExecutionContext.h"

namespace cish
{
namespace ast
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
DereferencedVariableReference::DereferencedVariableReference(DeclarationContext *context,
                                                             const std::string &varName,
                                                             int numDerefs)
{
    if (numDerefs <= 0) {
        Throw(Exception, "'%d' dereferences does not make sense", numDerefs);
    }

    const VarDeclaration *decl = context->getVariableDeclaration(varName);
    if (decl == nullptr) {
        Throw(VariableNotDeclaredException,
              "Variable '%s' not declared in the current scope",
              varName.c_str());
    }

    const TypeDecl *curType = &decl->type;
    for (int i=0; i<numDerefs; i++) {
        if (curType->getType() != TypeDecl::POINTER) {
            Throw(Exception, "Cannot dereference non-pointer type '%s'", curType->getName());
        }

        curType = curType->getReferencedType();
        if (curType == nullptr) {
            Throw(Exception, "Accidentally ended up with a NULL TypeDecl");
        }
    }

    _varName = decl->name;
    _numDerefs = numDerefs;
    _finalType = *curType;
    _declaredType = decl->type;
}

TypeDecl DereferencedVariableReference::getType() const
{
    return _finalType;
}
 
vm::MemoryView DereferencedVariableReference::getMemoryView(vm::ExecutionContext *context) const
{
    vm::Variable *var = context->getScope()->getVariable(_varName);
    if (var->getType() != _declaredType) {
        Throw(Exception, "invalid type found at runtime");
    }

    vm::MemoryView view = context->getMemory()->getView(var->getAllocation()->getAddress());

    for (int i=0; i<_numDerefs; i++) {
        uint32_t addr = view.read<uint32_t>();
        view = context->getMemory()->getView(addr);
    }

    return view;
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
}
