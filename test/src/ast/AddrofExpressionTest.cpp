#include <gtest/gtest.h>

#include "vm/Memory.h"
#include "vm/ExecutionContext.h"
#include "ast/DeclarationContext.h"
#include "ast/AddrofExpression.h"
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

    AddrofExpression expr(&dc, "i");
    ExpressionValue value = expr.evaluate(&ec);

    ASSERT_EQ(TypeDecl::POINTER, value.getIntrinsicType().getType());
    ASSERT_EQ(TypeDecl::INT, value.getIntrinsicType().getReferencedType()->getType());
    ASSERT_TRUE(value.get<uint32_t>() >= Memory::firstUsableMemoryAddress());
}
