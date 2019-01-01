#include <gtest/gtest.h>

#include "ast/StructAccessExpression.h"
#include "ast/StructField.h"

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


StructLayout::Ptr createStruct()
{
    StructLayout *layout = new StructLayout("foo");
    layout->addField(new StructField(TypeDecl::INT, "n"));
    layout->finalize();
    return StructLayout::Ptr(layout);
}


TEST(StructAccessExpressionTest, objectAccessIntoPointerThrows)
{
    auto layout = createStruct();
    TypeDecl type = TypeDecl::getPointer(TypeDecl::getStruct(layout));
    auto expr = std::make_shared<DummyExpression>(type);

    ASSERT_THROW(StructAccessExpression e(expr, "n", AccessType::OBJECT), InvalidAccessException);
}

TEST(StructAccessExpressionTest, pointerAccessIntoObjectThrows)
{
    auto layout = createStruct();
    TypeDecl type = TypeDecl::getStruct(layout);
    auto expr = std::make_shared<DummyExpression>(type);

    ASSERT_THROW(StructAccessExpression e(expr, "n", AccessType::POINTER), InvalidAccessException);
}

TEST(StructAccessExpressionTest, expressionCannotBePointerToPointerToStruct)
{
    auto layout = createStruct();
    TypeDecl type = TypeDecl::getPointer(TypeDecl::getPointer(TypeDecl::getStruct(layout)));
    auto expr = std::make_shared<DummyExpression>(type);

    ASSERT_THROW(StructAccessExpression e(expr, "n", AccessType::POINTER), InvalidTypeException);
}

TEST(StructAccessExpressionTest, expressionCannotBePointerToInt)
{
    TypeDecl type = TypeDecl::getPointer(TypeDecl::INT);
    auto expr = std::make_shared<DummyExpression>(type);

    ASSERT_THROW(StructAccessExpression e(expr, "n", AccessType::POINTER), InvalidTypeException);
}

TEST(StructAccessExpressionTest, expressionCannotBeInt)
{
    TypeDecl type = TypeDecl::INT;
    auto expr = std::make_shared<DummyExpression>(type);

    ASSERT_THROW(StructAccessExpression e(expr, "n", AccessType::POINTER), InvalidTypeException);
}

TEST(StructAccessExpressionTest, nonExistingMemberThrows)
{
    auto layout = createStruct();
    TypeDecl type = TypeDecl::getStruct(layout);
    auto expr = std::make_shared<DummyExpression>(type);

    ASSERT_THROW(StructAccessExpression e(expr, "doesntexistlol", AccessType::OBJECT), NoSuchFieldException);
}


TEST(StructAccessExpressionTest, validPointerAccess)
{
    auto layout = createStruct();
    TypeDecl type = TypeDecl::getPointer(TypeDecl::getStruct(layout));
    auto expr = std::make_shared<DummyExpression>(type);

    StructAccessExpression e(expr, "n", AccessType::POINTER);
}

TEST(StructAccessExpressionTest, validObjectAccess)
{
    auto layout = createStruct();
    TypeDecl type = TypeDecl::getStruct(layout);
    auto expr = std::make_shared<DummyExpression>(type);

    StructAccessExpression e(expr, "n", AccessType::OBJECT);
}


TEST(StructAccessExpressionTest, typeIsInheritedFromMember)
{
    StructLayout *__subStruct = new StructLayout("subbie");
    __subStruct->addField(new StructField(TypeDecl::FLOAT, "n"));
    __subStruct->finalize();
    StructLayout::Ptr subStruct = StructLayout::Ptr(__subStruct);
    

    std::vector<TypeDecl> types = {
        TypeDecl::getPointer(TypeDecl::INT),
        TypeDecl::getPointer(TypeDecl::VOID),
        TypeDecl::getPointer(TypeDecl::getPointer(TypeDecl::VOID)),
        TypeDecl::INT,
        TypeDecl::FLOAT,
        TypeDecl::getStruct(subStruct),
        TypeDecl::getPointer(TypeDecl::getStruct(subStruct)),
    };

    for (TypeDecl fieldType: types) {
        StructLayout *rawLayout = new StructLayout("foo");
        rawLayout->addField(new StructField(fieldType, "n"));
        StructLayout::Ptr layout = StructLayout::Ptr(rawLayout);

        TypeDecl type = TypeDecl::getStruct(layout);
        auto expr = std::make_shared<DummyExpression>(type);

        StructAccessExpression e(expr, "n", AccessType::OBJECT);
        ASSERT_EQ(fieldType, e.getType());
    }
}
