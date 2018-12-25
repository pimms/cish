#include <gtest/gtest.h>

#include "vm/VirtualMachine.h"
#include "vm/Memory.h"
#include "vm/ExecutionContext.h"
#include "vm/Variable.h"

#include "ast/DeclarationContext.h"
#include "ast/LiteralExpression.h"
#include "ast/ArithmeticAssignmentStatement.h"
#include "ast/Lvalue.h"

using namespace cish;
using namespace cish::ast;
using namespace cish::vm;


template<typename LHS, typename RHS>
void create(LHS initialValue, 
            RHS operandValue,
            BinaryExpression::Operator oper, 
            TypeDecl leftType,
            TypeDecl rightType)
{
    Memory memory(128, 4);
    ExecutionContext ec(&memory);

    DeclarationContext dc;
    dc.declareVariable(leftType, "var");

    Allocation::Ptr alloc = memory.allocate(leftType.getSize());
    Variable *var = new Variable(leftType, std::move(alloc));
    ec.getScope()->addVariable("var", var);

    // -- 

    Lvalue::Ptr lvalue = std::make_shared<VariableReference>(&dc, "var");
    Expression::Ptr expr = std::make_shared<LiteralExpression>(ExpressionValue(rightType, operandValue));

    var->getAllocation()->write<LHS>(initialValue);

    ArithmeticAssignmentStatement stmt(lvalue, oper, expr);
}

template<typename LHS, typename RHS>
LHS exec(LHS initialValue, 
         RHS operandValue, 
         BinaryExpression::Operator oper,
         TypeDecl leftType, 
         TypeDecl rightType)
{
    Memory memory(128, 4);
    ExecutionContext ec(&memory);

    DeclarationContext dc;
    dc.declareVariable(leftType, "var");

    Allocation::Ptr alloc = memory.allocate(leftType.getSize());
    Variable *var = new Variable(leftType, std::move(alloc));
    ec.getScope()->addVariable("var", var);

    // -- 

    Lvalue::Ptr lvalue = std::make_shared<VariableReference>(&dc, "var");
    Expression::Ptr expr = std::make_shared<LiteralExpression>(ExpressionValue(rightType, operandValue));

    var->getAllocation()->write<LHS>(initialValue);

    ArithmeticAssignmentStatement stmt(lvalue, oper, expr);
    stmt.execute(&ec);

    return var->getAllocation()->read<LHS>();
}

template<typename LHS, typename RHS>
LHS exec(LHS initialValue, RHS operandValue, BinaryExpression::Operator oper)
{
    const TypeDecl leftType = TypeDecl::getFromNative<LHS>();
    const TypeDecl rightType = TypeDecl::getFromNative<RHS>();
    return exec(initialValue, operandValue, oper, leftType, rightType);
}


TEST(ArithmeticAssignmentStatementTest, simpleWithInts)
{
    EXPECT_EQ(100, exec(90, 10, BinaryExpression::PLUS));
    EXPECT_EQ(80, exec(90, -10, BinaryExpression::PLUS));

    EXPECT_EQ(80, exec(90, 10, BinaryExpression::MINUS));
    EXPECT_EQ(100, exec(90, -10, BinaryExpression::MINUS));

    EXPECT_EQ(30, exec(5, 6, BinaryExpression::MULTIPLY));
    EXPECT_EQ(30, exec(60, 2, BinaryExpression::DIVIDE));

    EXPECT_EQ(1, exec(31, 30, BinaryExpression::MODULO));


    EXPECT_EQ(0x42, exec(0x21, 1, BinaryExpression::BITWISE_LSHIFT));
    EXPECT_EQ(0x84, exec(0x21, 2, BinaryExpression::BITWISE_LSHIFT));

    EXPECT_EQ(0x10, exec(0x21, 1, BinaryExpression::BITWISE_RSHIFT));
    EXPECT_EQ(0x08, exec(0x21, 2, BinaryExpression::BITWISE_RSHIFT));

    EXPECT_EQ(0x91, exec(0xF3, 0x95, BinaryExpression::BITWISE_AND));
    EXPECT_EQ(0x66, exec(0xF3, 0x95, BinaryExpression::BITWISE_XOR));
    EXPECT_EQ(0xF4, exec(0xF0, 0x04, BinaryExpression::BITWISE_OR));
}

TEST(ArithmeticAssignmentStatementTest, intMutatedByFloat)
{
    EXPECT_EQ(49, exec(10, 4.95f, BinaryExpression::MULTIPLY, TypeDecl::INT, TypeDecl::FLOAT));
    EXPECT_EQ(40, exec(100, 2.5f, BinaryExpression::DIVIDE, TypeDecl::INT, TypeDecl::FLOAT));
}

TEST(ArithmeticAssignmentStatementTest, pointersIncrementBySizeOfReferredType)
{
    TypeDecl ptrType = TypeDecl::getPointer(TypeDecl::INT);

    EXPECT_EQ(0x4004, exec(0x4000, 1, BinaryExpression::PLUS, ptrType, TypeDecl::INT));
    EXPECT_EQ(0x4008, exec(0x4000, 2, BinaryExpression::PLUS, ptrType, TypeDecl::INT));
    EXPECT_EQ(0x3ffc, exec(0x4000, 1, BinaryExpression::MINUS, ptrType, TypeDecl::INT));
}

TEST(ArithmeticAssignmentStatementTest, invalidOperationsForFloats)
{
    EXPECT_ANY_THROW(create(10.f, 1, BinaryExpression::BITWISE_LSHIFT, TypeDecl::FLOAT, TypeDecl::INT));
    EXPECT_ANY_THROW(create(1, 10.f, BinaryExpression::BITWISE_LSHIFT, TypeDecl::INT, TypeDecl::FLOAT));
    EXPECT_ANY_THROW(create(10.f, 1, BinaryExpression::BITWISE_LSHIFT, TypeDecl::DOUBLE, TypeDecl::INT));
    EXPECT_ANY_THROW(create(1, 10.f, BinaryExpression::BITWISE_LSHIFT, TypeDecl::INT, TypeDecl::DOUBLE));

    EXPECT_ANY_THROW(create(10.f, 1, BinaryExpression::BITWISE_RSHIFT, TypeDecl::FLOAT, TypeDecl::INT));
    EXPECT_ANY_THROW(create(1, 10.f, BinaryExpression::BITWISE_RSHIFT, TypeDecl::INT, TypeDecl::FLOAT));
    EXPECT_ANY_THROW(create(10.f, 1, BinaryExpression::BITWISE_RSHIFT, TypeDecl::DOUBLE, TypeDecl::INT));
    EXPECT_ANY_THROW(create(1, 10.f, BinaryExpression::BITWISE_RSHIFT, TypeDecl::INT, TypeDecl::DOUBLE));

    EXPECT_ANY_THROW(create(10.f, 1, BinaryExpression::BITWISE_AND, TypeDecl::FLOAT, TypeDecl::INT));
    EXPECT_ANY_THROW(create(1, 10.f, BinaryExpression::BITWISE_AND, TypeDecl::INT, TypeDecl::FLOAT));
    EXPECT_ANY_THROW(create(10.f, 1, BinaryExpression::BITWISE_AND, TypeDecl::DOUBLE, TypeDecl::INT));
    EXPECT_ANY_THROW(create(1, 10.f, BinaryExpression::BITWISE_AND, TypeDecl::INT, TypeDecl::DOUBLE));

    EXPECT_ANY_THROW(create(10.f, 1, BinaryExpression::BITWISE_XOR, TypeDecl::FLOAT, TypeDecl::INT));
    EXPECT_ANY_THROW(create(1, 10.f, BinaryExpression::BITWISE_XOR, TypeDecl::INT, TypeDecl::FLOAT));
    EXPECT_ANY_THROW(create(10.f, 1, BinaryExpression::BITWISE_XOR, TypeDecl::DOUBLE, TypeDecl::INT));
    EXPECT_ANY_THROW(create(1, 10.f, BinaryExpression::BITWISE_XOR, TypeDecl::INT, TypeDecl::DOUBLE));

    EXPECT_ANY_THROW(create(10.f, 1, BinaryExpression::BITWISE_OR, TypeDecl::FLOAT, TypeDecl::INT));
    EXPECT_ANY_THROW(create(1, 10.f, BinaryExpression::BITWISE_OR, TypeDecl::INT, TypeDecl::FLOAT));
    EXPECT_ANY_THROW(create(10.f, 1, BinaryExpression::BITWISE_OR, TypeDecl::DOUBLE, TypeDecl::INT));
    EXPECT_ANY_THROW(create(1, 10.f, BinaryExpression::BITWISE_OR, TypeDecl::INT, TypeDecl::DOUBLE));

    EXPECT_ANY_THROW(create(10.f, 1, BinaryExpression::MODULO, TypeDecl::FLOAT, TypeDecl::INT));
    EXPECT_ANY_THROW(create(10, 1.5f, BinaryExpression::MODULO, TypeDecl::INT, TypeDecl::FLOAT));
    EXPECT_ANY_THROW(create(10.f, 1, BinaryExpression::MODULO, TypeDecl::DOUBLE, TypeDecl::INT));
    EXPECT_ANY_THROW(create(10, 1.5f, BinaryExpression::MODULO, TypeDecl::INT, TypeDecl::DOUBLE));
}

TEST(ArithmeticAssignmentStatementTest, invalidOperationsForPointers)
{
    TypeDecl ptrType = TypeDecl::getPointer(TypeDecl::INT);

    EXPECT_ANY_THROW(create(0x4000, 1, BinaryExpression::BITWISE_LSHIFT, ptrType, TypeDecl::INT));
    EXPECT_ANY_THROW(create(0x4000, 1, BinaryExpression::BITWISE_RSHIFT, ptrType, TypeDecl::INT));
    EXPECT_ANY_THROW(create(0x4000, 1, BinaryExpression::BITWISE_AND, ptrType, TypeDecl::INT));
    EXPECT_ANY_THROW(create(0x4000, 1, BinaryExpression::BITWISE_XOR, ptrType, TypeDecl::INT));
    EXPECT_ANY_THROW(create(0x4000, 1, BinaryExpression::BITWISE_OR, ptrType, TypeDecl::INT));

    EXPECT_ANY_THROW(create(0x4000, 1, BinaryExpression::MULTIPLY, ptrType, TypeDecl::INT));
    EXPECT_ANY_THROW(create(0x4000, 1, BinaryExpression::DIVIDE, ptrType, TypeDecl::INT));
    EXPECT_ANY_THROW(create(0x4000, 1, BinaryExpression::MODULO, ptrType, TypeDecl::INT));

    EXPECT_ANY_THROW(create(1, 0x4000, BinaryExpression::PLUS, TypeDecl::INT, ptrType));
    EXPECT_ANY_THROW(create(1, 0x4000, BinaryExpression::MINUS, TypeDecl::INT, ptrType));
}

