#include <gtest/gtest.h>

#include "ast/Lvalue.h"
#include "ast/VariableAssignmentStatement.h"
#include "ast/DeclarationContext.h"
#include "ast/LiteralExpression.h"
#include "ast/AstNodes.h"

#include "vm/Memory.h"
#include "vm/ExecutionContext.h"


using namespace cish::vm;
using namespace cish::ast;

void declareVariable(DeclarationContext &context, TypeDecl type, std::string name)
{
    context.declareVariable(type, name);
}

void defineVariable(ExecutionContext &context, TypeDecl type, std::string name)
{
    auto alloc = context.getMemory()->allocate(type.getSize());
    Variable *var = new Variable(type, std::move(alloc));
    context.getScope()->addVariable(name, var);
}

template<typename To, typename From>
void testAssignmentCasting()
{
    std::vector<From> testData = {
        From(100.5f),   From(0.0f),  From(0.5f),
        From(1.f),      From(-10.f), From(-5.f),
        From(399553.f), From(99.f),  From(-0.1f),
    };

    Memory memory(100, 1);
    DeclarationContext dc;
    ExecutionContext ec(&memory);

    declareVariable(dc, TypeDecl::getFromNative<To>(), "var");

    for (auto val: testData) {
        ec.pushFunctionFrame();

        auto alloc = memory.allocate(sizeof(To));
        auto rawAlloc = alloc.get();
        Variable *var = new Variable(TypeDecl::getFromNative<To>(), std::move(alloc));
        ec.getScope()->addVariable("var", var);

        ExpressionValue exprValue(TypeDecl::getFromNative<From>(), val);
        auto expr = new LiteralExpression(exprValue);

        ASSERT_NO_THROW({
            VariableAssignmentStatement stmt(&dc, new VariableReference(&dc, "var"), expr);
            stmt.execute(&ec);
        });

        ASSERT_EQ((To)val, rawAlloc->read<To>());

        ec.popFunctionFrame();
    }
}

TEST(VariableAssignmentStatementTest, assignmentCastTest)
{
    testAssignmentCasting<bool,bool>();
    testAssignmentCasting<bool,char>();
    testAssignmentCasting<bool,short>();
    testAssignmentCasting<bool,int>();
    testAssignmentCasting<bool,float>();
    testAssignmentCasting<bool,double>();

    testAssignmentCasting<char,bool>();
    testAssignmentCasting<char,char>();
    testAssignmentCasting<char,short>();
    testAssignmentCasting<char,int>();
    testAssignmentCasting<char,float>();
    testAssignmentCasting<char,double>();

    testAssignmentCasting<short,bool>();
    testAssignmentCasting<short,char>();
    testAssignmentCasting<short,short>();
    testAssignmentCasting<short,int>();
    testAssignmentCasting<short,float>();
    testAssignmentCasting<short,double>();

    testAssignmentCasting<int,bool>();
    testAssignmentCasting<int,char>();
    testAssignmentCasting<int,short>();
    testAssignmentCasting<int,int>();
    testAssignmentCasting<int,float>();
    testAssignmentCasting<int,double>();

    testAssignmentCasting<float,bool>();
    testAssignmentCasting<float,char>();
    testAssignmentCasting<float,short>();
    testAssignmentCasting<float,int>();
    testAssignmentCasting<float,float>();
    testAssignmentCasting<float,double>();

    testAssignmentCasting<double,bool>();
    testAssignmentCasting<double,char>();
    testAssignmentCasting<double,short>();
    testAssignmentCasting<double,int>();
    testAssignmentCasting<double,float>();
    testAssignmentCasting<double,double>();
}

TEST(VariableAssignmentStatementTest, undefinedVariableThrows)
{
    Memory memory(100, 1);
    DeclarationContext dc;
    ExecutionContext ec(&memory);

    declareVariable(dc, TypeDecl::INT, "var");

    auto expr = new LiteralExpression(ExpressionValue(TypeDecl::INT, 10));
    VariableAssignmentStatement stmt(&dc, new VariableReference(&dc, "var"), expr);
    ASSERT_THROW(stmt.execute(&ec), VariableNotDefinedException);
}

TEST(VariableAssignmentStatemenTest, assigningToConstVariablesNotAllowed)
{
    DeclarationContext dc;

    TypeDecl type = TypeDecl::INT;
    type.setConst(true);

    dc.declareVariable(type, "i");

    auto expr = new LiteralExpression(ExpressionValue(TypeDecl::INT, 10));
    auto ref = new VariableReference(&dc, "i");
    ASSERT_THROW(VariableAssignmentStatement stmt(&dc, ref, expr), InvalidOperationException);
    delete expr;
    delete ref;
}
