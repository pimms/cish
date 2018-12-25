#include "ArithmeticAssignmentStatement.h"
#include "LiteralExpression.h"
#include "../vm/Allocation.h"


namespace cish
{
namespace ast
{


ArithmeticAssignmentStatement::ArithmeticAssignmentStatement(Lvalue::Ptr lvalue,
                                                             BinaryExpression::Operator op,
                                                             Expression::Ptr expr):
    _lvalue(lvalue),
    _operator(op),
    _expression(expr)
{
    // Construct the BinaryExpression with dummy values to catch any type incompatibilities.
    ExpressionValue dummyLeft(lvalue->getType(), 0);
    ExpressionValue dummyRight(expr->getType(), 0);

    _leftExpr = std::make_shared<MutableLiteralExpression>(dummyLeft);
    _rightExpr = std::make_shared<MutableLiteralExpression>(dummyRight);
    _binaryExpression = std::make_shared<BinaryExpression>(_operator, _leftExpr, _rightExpr);
}

void ArithmeticAssignmentStatement::execute(vm::ExecutionContext *ctx) const
{
    ExpressionValue lhs = getLeftValue(ctx);
    ExpressionValue rhs = _expression->evaluate(ctx);

    ((MutableLiteralExpression*)_leftExpr.get())->setValue(lhs);
    ((MutableLiteralExpression*)_rightExpr.get())->setValue(rhs);
    ExpressionValue nval = _binaryExpression->evaluate(ctx);

    vm::MemoryView memView = _lvalue->getMemoryView(ctx);
    writeResult(memView, nval);
}

ExpressionValue ArithmeticAssignmentStatement::getLeftValue(vm::ExecutionContext *context) const
{
    TypeDecl intrinsicType = _lvalue->getType();
    vm::MemoryView memView = _lvalue->getMemoryView(context);

    switch (_lvalue->getType().getType()) {
        case TypeDecl::BOOL:
            return ExpressionValue(intrinsicType, memView.read<bool>());
        case TypeDecl::CHAR:
            return ExpressionValue(intrinsicType, memView.read<int8_t>());
        case TypeDecl::SHORT:
            return ExpressionValue(intrinsicType, memView.read<int16_t>());
        case TypeDecl::INT:
            return ExpressionValue(intrinsicType, memView.read<int32_t>());
        case TypeDecl::FLOAT:
            return ExpressionValue(intrinsicType, memView.read<float>());
        case TypeDecl::DOUBLE:
            return ExpressionValue(intrinsicType, memView.read<double>());
        case TypeDecl::POINTER:
            return ExpressionValue(intrinsicType, memView.read<uint32_t>());
        default:
            Throw(InvalidTypeException, "Unable to handle type %s in arithmetic assignment",
                    intrinsicType.getName());
    }
}

void ArithmeticAssignmentStatement::writeResult(vm::MemoryView &memView,
                                                const ExpressionValue &value) const
{
    TypeDecl intrinsicType = _lvalue->getType();

    switch (_lvalue->getType().getType()) {
        case TypeDecl::BOOL:
            memView.write<bool>(value.get<bool>());
            break;
        case TypeDecl::CHAR:
            memView.write<int8_t>(value.get<int8_t>());
            break;
        case TypeDecl::SHORT:
            memView.write<int16_t>(value.get<int16_t>());
            break;
        case TypeDecl::INT:
            memView.write<int32_t>(value.get<int32_t>());
            break;
        case TypeDecl::FLOAT:
            memView.write<float>(value.get<float>());
            break;
        case TypeDecl::DOUBLE:
            memView.write<double>(value.get<double>());
            break;
        case TypeDecl::POINTER:
            memView.write<uint32_t>(value.get<uint32_t>());
            break;
        default:
            Throw(InvalidTypeException, "Unable to handle type %s in arithmetic assignment",
                    intrinsicType.getName());
    }
}


}
}

