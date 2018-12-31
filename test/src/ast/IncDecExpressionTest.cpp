#include <gtest/gtest.h>

#include <map>
#include <vector>
#include <tuple>

#include "ast/IncDecExpression.h"
#include "ast/DeclarationContext.h"
#include "ast/Lvalue.h"
#include "vm/ExecutionContext.h"
#include "vm/Memory.h"
#include "vm/Variable.h"


using namespace cish::vm;
using namespace cish::ast;


void verifyPrefixIncrement()
{
}


TEST(IncDecExpressionTest, verifyOperationsOnPointers)
{
	const std::vector<TypeDecl> types = {
		TypeDecl::getPointer(TypeDecl::VOID),
		TypeDecl::getPointer(TypeDecl::BOOL),
		TypeDecl::getPointer(TypeDecl::CHAR),
		TypeDecl::getPointer(TypeDecl::SHORT),
		TypeDecl::getPointer(TypeDecl::INT),
		TypeDecl::getPointer(TypeDecl::FLOAT),
		TypeDecl::getPointer(TypeDecl::DOUBLE),
		TypeDecl::getPointer(TypeDecl::getPointer(TypeDecl::INT)),
	};

	const std::map<IncDecExpression::Operation, std::tuple<int,int>> expectations = {
		{ IncDecExpression::Operation::POSTFIX_INCREMENT, {0, 1} },
		{ IncDecExpression::Operation::PREFIX_INCREMENT, {1, 1} },
		{ IncDecExpression::Operation::POSTFIX_DECREMENT, {0, -1} },
		{ IncDecExpression::Operation::PREFIX_DECREMENT, {-1, -1} },
	};

	for (TypeDecl type: types) {
		for (auto pair: expectations) {
			Memory memory(100, 1);
			DeclarationContext dc;
			ExecutionContext ec(&memory);

			Variable *var = new Variable(type, memory.allocate(type.getSize()));
			var->getAllocation()->write<int>(10);

			dc.declareVariable(type, "i");
			ec.getScope()->addVariable("i", var);

            auto lvalue = std::make_shared<VariableReference>(&dc, "i");
			IncDecExpression expr(pair.first, lvalue);
			ExpressionValue value = expr.evaluate(&ec);

            const auto refType = *type.getReferencedType();
            const int factor = (refType.getType() == TypeDecl::VOID ? 1 : refType.getSize());
            const int expectedExpressionValue = 10 + std::get<0>(pair.second) * factor;
            const int expectedVariableValue = 10 + std::get<1>(pair.second) * factor;

			ASSERT_EQ(expectedVariableValue, var->getAllocation()->read<int>());
			ASSERT_EQ(expectedExpressionValue, value.get<int>());
		}
	}
}

TEST(IncDecExpressionTest, verifyOperationsOnPrimitives)
{
	const std::vector<TypeDecl> types = {
		TypeDecl::BOOL,
		TypeDecl::CHAR,
		TypeDecl::SHORT,
		TypeDecl::INT,
	};

	const std::map<IncDecExpression::Operation, std::tuple<int,int>> expectations = {
		{ IncDecExpression::Operation::POSTFIX_INCREMENT, {0, 1} },
		{ IncDecExpression::Operation::PREFIX_INCREMENT, {1, 1} },
		{ IncDecExpression::Operation::POSTFIX_DECREMENT, {0, -1} },
		{ IncDecExpression::Operation::PREFIX_DECREMENT, {-1, -1} },
	};

	for (TypeDecl type: types) {
		for (auto pair: expectations) {
			Memory memory(100, 1);
			DeclarationContext dc;
			ExecutionContext ec(&memory);

			Variable *var = new Variable(type, memory.allocate(type.getSize()));
			var->getAllocation()->write<uint8_t>(10);

			dc.declareVariable(type, "i");
			ec.getScope()->addVariable("i", var);

            auto lvalue = std::make_shared<VariableReference>(&dc, "i");
			IncDecExpression expr(pair.first, lvalue);
			ExpressionValue value = expr.evaluate(&ec);
			ASSERT_EQ(10 + std::get<0>(pair.second), value.get<uint8_t>());
			ASSERT_EQ(10 + std::get<1>(pair.second), var->getAllocation()->read<uint8_t>());
		}
	}
}

TEST(IncDecExpressionTest, charOverflow)
{
    Memory memory(100, 1);
    DeclarationContext dc;
    ExecutionContext ec(&memory);

    Variable *var = new Variable(TypeDecl::CHAR, memory.allocate(1));
    var->getAllocation()->write<uint8_t>(127);

    dc.declareVariable(TypeDecl::CHAR, "i");
    ec.getScope()->addVariable("i", var);

    auto lvalue = std::make_shared<VariableReference>(&dc, "i");
    IncDecExpression expr(IncDecExpression::POSTFIX_INCREMENT, lvalue);
    ExpressionValue value = expr.evaluate(&ec);

    ASSERT_EQ(-128, var->getAllocation()->read<char>());
    ASSERT_EQ(127, value.get<char>());
}

TEST(IncDecExpressionTest, charUnderflow)
{
    Memory memory(100, 1);
    DeclarationContext dc;
    ExecutionContext ec(&memory);

    Variable *var = new Variable(TypeDecl::CHAR, memory.allocate(1));
    var->getAllocation()->write<char>(-128);

    dc.declareVariable(TypeDecl::CHAR, "i");
    ec.getScope()->addVariable("i", var);

    auto lvalue = std::make_shared<VariableReference>(&dc, "i");
    IncDecExpression expr(IncDecExpression::POSTFIX_DECREMENT, lvalue);
    ExpressionValue value = expr.evaluate(&ec);

    ASSERT_EQ(127, var->getAllocation()->read<char>());
    ASSERT_EQ(-128, value.get<char>());
}

TEST(IncDecExpressionTest, operatingOnConstVariablesNotAllowed)
{
    DeclarationContext dc;

    TypeDecl type = TypeDecl::INT;
    type.setConst(true);
    dc.declareVariable(type, "i");

    auto lvalue = std::make_shared<VariableReference>(&dc, "i");
    ASSERT_THROW(IncDecExpression expr(IncDecExpression::POSTFIX_DECREMENT, lvalue),
                 InvalidOperationException);
}

