#include <gtest/gtest.h>

#include "ast/VariableReferenceExpression.h"
#include "ast/DeclarationContext.h"
#include "vm/Memory.h"
#include "vm/Variable.h"
#include "vm/ExecutionContext.h"

using namespace cish::ast;
using namespace cish::vm;


TEST(VariableReferenceExpressionTest, undeclaredVariableReferenceThrows)
{
    DeclarationContext dc;
    ASSERT_THROW(VariableReferenceExpression(&dc, "var"), VariableNotDeclaredException);
}

TEST(VariableReferenceExpressionTest, declaredVariableReferenceDoesNotThrow)
{
    DeclarationContext dc;
    dc.declareVariable(TypeDecl::INT, "var");
    ASSERT_NO_THROW(VariableReferenceExpression(&dc, "var"));
}

TEST(VariableReferenceExpressionTest, declaredVariableTypeIsInheritedByExpression)
{
    DeclarationContext dc;
    dc.declareVariable(TypeDecl::INT, "intVar");
    dc.declareVariable(TypeDecl::FLOAT, "floatVar");

    VariableReferenceExpression iexpr(&dc, "intVar");
    ASSERT_EQ(TypeDecl(TypeDecl::INT), iexpr.getType());

    VariableReferenceExpression fexpr(&dc, "floatVar");
    ASSERT_EQ(TypeDecl(TypeDecl::FLOAT), fexpr.getType());
}

TEST(VariableReferenceExpressionTest, undefinedVariableEvaluationAttemptThrows)
{
    Memory memory(100, 1);
    DeclarationContext dc;
    ExecutionContext ec(&memory);

    dc.declareVariable(TypeDecl::INT, "var");

    VariableReferenceExpression expr(&dc, "var");

    ASSERT_THROW(expr.evaluate(&ec), VariableNotDefinedException);
}

TEST(VariableReferenceExpressionTest, typeMismatchDuringEvaluationThrows)
{
    Memory memory(100, 1);
    DeclarationContext dc;
    ExecutionContext ec(&memory);

    dc.declareVariable(TypeDecl::INT, "var");

    ec.getStackFrame()->addVariable("var", new Variable(TypeDecl::SHORT, memory.allocate(2)));

    VariableReferenceExpression expr(&dc, "var");
    ASSERT_THROW(expr.evaluate(&ec), InvalidTypeException);
}


template<typename T>
void testSimpleVariableReferenceEvaluation()
{
    std::vector<T> testData = {
        T(10.5f),
        T(0.0f),
        T(0.5f),
        T(-15.1)
    };

    for (T t: testData) {
        Memory memory(100, 1);
        Allocation::Ptr allocation = memory.allocate(sizeof(T));
        Allocation *rawAlloc = allocation.get();
        rawAlloc->write<T>(T(0));

        TypeDecl type = TypeDecl::getFromNative<T>();
        Variable *var = new Variable(type, std::move(allocation));

        DeclarationContext dc;
        ExecutionContext ec(&memory);

        dc.declareVariable(type, "var");
        ec.getStackFrame()->addVariable("var", var);

        VariableReferenceExpression expr(&dc, "var");

        // Read the initial value
        ASSERT_NEAR(T(0), expr.evaluate(&ec).get<T>(), 0.001);

        // Write a new value and read it
        rawAlloc->write<T>(t);
        ASSERT_NEAR(t, expr.evaluate(&ec).get<T>(), 0.001);

        // And again for good measure :)
        rawAlloc->write<T>(T(0));
        ASSERT_NEAR(T(0), expr.evaluate(&ec).get<T>(), 0.001);
    }
}

TEST(VariableReferenceExpressionTest, simpleEvaluationTest)
{
    testSimpleVariableReferenceEvaluation<bool>();
    testSimpleVariableReferenceEvaluation<char>();
    testSimpleVariableReferenceEvaluation<short>();
    testSimpleVariableReferenceEvaluation<int>();
    testSimpleVariableReferenceEvaluation<long>();
    testSimpleVariableReferenceEvaluation<float>();
    testSimpleVariableReferenceEvaluation<double>();
}

TEST(VariableReferenceExpressionTest, shadowingWorksAsExpected)
{
    Memory memory(100, 1);
    DeclarationContext dc;
    ExecutionContext ec(&memory);


    Allocation::Ptr alloc1 = memory.allocate(4);
    Allocation::Ptr alloc2 = memory.allocate(2);

    alloc1->write<int>(1);
    alloc2->write<short>(2);

    Variable *var1 = new Variable(TypeDecl::INT, std::move(alloc1));
    Variable *var2 = new Variable(TypeDecl::SHORT, std::move(alloc2));

    dc.declareVariable(TypeDecl::INT, "var");
    VariableReferenceExpression ref1(&dc, "var");

    dc.pushVariableScope();
    dc.declareVariable(TypeDecl::SHORT, "var");
    VariableReferenceExpression ref2(&dc, "var");

    // Evaluate reference 1
    ec.getStackFrame()->addVariable("var", var1);
    ExpressionValue val1 = ref1.evaluate(&ec);
    ASSERT_EQ(TypeDecl(TypeDecl::INT), val1.getIntrinsicType());
    ASSERT_EQ(1, val1.get<int>());

    // Evaluate reference 2
    ec.pushStackFrame();
    ec.getStackFrame()->addVariable("var", var2);
    ExpressionValue val2 = ref2.evaluate(&ec);
    ASSERT_EQ(TypeDecl(TypeDecl::SHORT), val2.getIntrinsicType());
    ASSERT_EQ(2, val2.get<short>());

    // Evaluate reference 1 again
    ec.popStackFrame();
    val1 = ref1.evaluate(&ec);
    ASSERT_EQ(TypeDecl(TypeDecl::INT), val1.getIntrinsicType());
    ASSERT_EQ(1, val1.get<int>());
}


