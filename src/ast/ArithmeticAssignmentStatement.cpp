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

    // Ensure that the lvalue is non-const
    if (_lvalue->getType().isConst()) {
        Throw(InvalidOperationException, "Cannot alter a constant variable");
    }

    /* EDGECASE
     *
     * Expressions of the form (<int> + <pointer>) are valid in all other contexts
     * but arithmetic assignments, so this is a case we must handle ourselves. This
     * can be simplified to checking if our operand is a pointer type, as no valid
     * statement will have a RHS-pointer.
     */
    if (_expression->getType().getType() == TypeDecl::POINTER) {
        Throw(InvalidOperationException,
              "Operand of type '%s' cannot be used in arithmetic assignment",
              _expression->getType().getName());
    }
}

void ArithmeticAssignmentStatement::virtualExecute(vm::ExecutionContext *ctx) const
{
    vm::MemoryView memView = _lvalue->getMemoryView(ctx);

    ExpressionValue lhs = getLeftValue(memView);
    ExpressionValue rhs = _expression->evaluate(ctx);

    ((MutableLiteralExpression*)_leftExpr.get())->setValue(lhs);
    ((MutableLiteralExpression*)_rightExpr.get())->setValue(rhs);
    ExpressionValue nval = _binaryExpression->evaluate(ctx);

    writeResult(memView, nval);
}

ExpressionValue ArithmeticAssignmentStatement::getLeftValue(vm::MemoryView &memoryView) const
{
    TypeDecl intrinsicType = _lvalue->getType();

    switch (_lvalue->getType().getType()) {
        case TypeDecl::BOOL:
            return ExpressionValue(intrinsicType, memoryView.read<bool>());
        case TypeDecl::CHAR:
            return ExpressionValue(intrinsicType, memoryView.read<int8_t>());
        case TypeDecl::SHORT:
            return ExpressionValue(intrinsicType, memoryView.read<int16_t>());
        case TypeDecl::INT:
            return ExpressionValue(intrinsicType, memoryView.read<int32_t>());
        case TypeDecl::LONG:
            return ExpressionValue(intrinsicType, memoryView.read<int64_t>());
        case TypeDecl::FLOAT:
            return ExpressionValue(intrinsicType, memoryView.read<float>());
        case TypeDecl::DOUBLE:
            return ExpressionValue(intrinsicType, memoryView.read<double>());
        case TypeDecl::POINTER:
            return ExpressionValue(intrinsicType, memoryView.read<uint32_t>());
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
        case TypeDecl::LONG:
            memView.write<int64_t>(value.get<int64_t>());
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

