#include <gtest/gtest.h>

#include "vm/Memory.h"
#include "vm/ExecutionContext.h"
#include "ast/DeclarationContext.h"
#include "ast/AddrofExpression.h"
#include "ast/Lvalue.h"
#include "ast/Type.h"

using namespace cish::vm;
using namespace cish::ast;


TEST(AddrofExpressionTest, verifyReturnValue)
{
    Memory memory(100, 1);
    DeclarationContext dc;
    ExecutionContext ec(&memory);

    dc.declareVariable(TypeDecl::INT, "i");
    ec.getScope()->addVariable("i", new Variable(TypeDecl::INT, memory.allocate(4)));

    VariableReference::Ptr varRef = std::make_shared<VariableReference>(&dc, "i");
    
    AddrofExpression expr(varRef);
    ExpressionValue value = expr.evaluate(&ec);

    ASSERT_EQ(TypeDecl::POINTER, value.getIntrinsicType().getType());
    ASSERT_EQ(TypeDecl::INT, value.getIntrinsicType().getReferencedType()->getType());
    ASSERT_TRUE(value.get<uint32_t>() >= Memory::firstUsableMemoryAddress());
}

TEST(AddrofExpressionTest, addorfOfPointerEqualsPointerToPointer)
{
    Memory memory(100, 1);
    ExecutionContext ec(&memory);
    DeclarationContext dc;

    TypeDecl type = TypeDecl::getPointer(TypeDecl::INT);
    dc.declareVariable(type, "ptr");
    ec.getScope()->addVariable("ptr", new Variable(type, memory.allocate(4)));

    VariableReference::Ptr varRef = std::make_shared<VariableReference>(&dc, "ptr");

    AddrofExpression expr(varRef);
    ASSERT_EQ(TypeDecl::POINTER, expr.getType().getType());
    ASSERT_EQ(TypeDecl::POINTER, expr.getType().getReferencedType()->getType());
    ASSERT_EQ(TypeDecl::INT, expr.getType().getReferencedType()->getReferencedType()->getType());

    ExpressionValue value = expr.evaluate(&ec);
    ASSERT_EQ(TypeDecl::POINTER, value.getIntrinsicType().getType());
    ASSERT_EQ(TypeDecl::POINTER, value.getIntrinsicType().getReferencedType()->getType());
    ASSERT_EQ(TypeDecl::INT, value.getIntrinsicType().getReferencedType()->getReferencedType()->getType());
}
