#include <gtest/gtest.h>

#include "ast/StructAccessExpression.h"

using namespace cish::ast;


typedef StructAccessExpression::AccessType AccessType;

/**
 * At point of implementation ExpressionValues does not support
 * structs, so this dummy-class is used to avoid having to do
 * it straight away.
 */
class DummyExpression: public Expression
{
public:
    DummyExpression(TypeDecl type) {
        _type = type;
    }

    TypeDecl getType() const override {
        return _type;
    }

    ExpressionValue evaluate(cish::vm::ExecutionContext*) const override {
        Throw(cish::Exception, "Not implemented");
    }

private:
    TypeDecl _type;
};



TEST(StructAccessExpressionTest, objectAccessIntoPointerThrows)
{
    StructLayout layout("foo");
    layout.addField(TypeDecl::INT, "n");
    TypeDecl type = TypeDecl::getPointer(TypeDecl::getStruct(&layout));
    auto expr = std::make_shared<DummyExpression>(type);

    ASSERT_THROW(StructAccessExpression e(expr, "n", AccessType::OBJECT), InvalidAccessException);
}

TEST(StructAccessExpressionTest, pointerAccessIntoObjectThrows)
{
    StructLayout layout("foo");
    layout.addField(TypeDecl::INT, "n");
    TypeDecl type = TypeDecl::getStruct(&layout);
    auto expr = std::make_shared<DummyExpression>(type);

    ASSERT_THROW(StructAccessExpression e(expr, "n", AccessType::POINTER), InvalidAccessException);
}

TEST(StructAccessExpressionTest, expressionCannotBePointerToPointerToStruct)
{
    StructLayout layout("foo");
    layout.addField(TypeDecl::INT, "n");
    TypeDecl type = TypeDecl::getPointer(TypeDecl::getPointer(TypeDecl::getStruct(&layout)));
    auto expr = std::make_shared<DummyExpression>(type);

    ASSERT_THROW(StructAccessExpression e(expr, "n", AccessType::POINTER), InvalidTypeException);
}

TEST(StructAccessExpressionTest, expressionCannotBePointerToInt)
{
    StructLayout layout("foo");
    layout.addField(TypeDecl::INT, "n");
    TypeDecl type = TypeDecl::getPointer(TypeDecl::INT);
    auto expr = std::make_shared<DummyExpression>(type);

    ASSERT_THROW(StructAccessExpression e(expr, "n", AccessType::POINTER), InvalidTypeException);
}

TEST(StructAccessExpressionTest, expressionCannotBeInt)
{
    StructLayout layout("foo");
    layout.addField(TypeDecl::INT, "n");
    TypeDecl type = TypeDecl::INT;
    auto expr = std::make_shared<DummyExpression>(type);

    ASSERT_THROW(StructAccessExpression e(expr, "n", AccessType::POINTER), InvalidTypeException);
}

TEST(StructAccessExpressionTest, nonExistingMemberThrows)
{
    StructLayout layout("foo");
    layout.addField(TypeDecl::INT, "n");
    TypeDecl type = TypeDecl::getStruct(&layout);
    auto expr = std::make_shared<DummyExpression>(type);

    ASSERT_THROW(StructAccessExpression e(expr, "doesntexistlol", AccessType::OBJECT), NoSuchFieldException);
}


TEST(StructAccessExpressionTest, validPointerAccess)
{
    StructLayout layout("foo");
    layout.addField(TypeDecl::INT, "n");
    TypeDecl type = TypeDecl::getPointer(TypeDecl::getStruct(&layout));
    auto expr = std::make_shared<DummyExpression>(type);

    StructAccessExpression e(expr, "n", AccessType::POINTER);
}

TEST(StructAccessExpressionTest, validObjectAccess)
{
    StructLayout layout("foo");
    layout.addField(TypeDecl::INT, "n");
    TypeDecl type = TypeDecl::getStruct(&layout);
    auto expr = std::make_shared<DummyExpression>(type);

    StructAccessExpression e(expr, "n", AccessType::OBJECT);
}


TEST(StructAccessExpressionTest, typeIsInheritedFromMember)
{
    StructLayout subStruct("subbie");
    subStruct.addField(TypeDecl::FLOAT, "n");
    subStruct.finalize();

    std::vector<TypeDecl> types = {
        TypeDecl::getPointer(TypeDecl::INT),
        TypeDecl::getPointer(TypeDecl::VOID),
        TypeDecl::getPointer(TypeDecl::getPointer(TypeDecl::VOID)),
        TypeDecl::INT,
        TypeDecl::FLOAT,
        TypeDecl::getStruct(&subStruct),
        TypeDecl::getPointer(TypeDecl::getStruct(&subStruct)),
    };

    for (TypeDecl fieldType: types) {
        StructLayout layout("foo");
        layout.addField(fieldType, "n");
        TypeDecl type = TypeDecl::getStruct(&layout);
        auto expr = std::make_shared<DummyExpression>(type);

        StructAccessExpression e(expr, "n", AccessType::OBJECT);
        ASSERT_EQ(fieldType, e.getType());
    }
}
