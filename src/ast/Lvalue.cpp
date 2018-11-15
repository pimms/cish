#include "Lvalue.h"
#include "../vm/ExecutionContext.h"

namespace cish
{
namespace ast
{


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

const TypeDecl& VariableReference::getType() const
{
    return _varDecl.type;
}

vm::MemoryView VariableReference::getMemoryView(vm::ExecutionContext *context)
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

const TypeDecl& DereferencedVariableReference::getType() const
{
    return _finalType;
}

vm::MemoryView DereferencedVariableReference::getMemoryView(vm::ExecutionContext *context)
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


}
}