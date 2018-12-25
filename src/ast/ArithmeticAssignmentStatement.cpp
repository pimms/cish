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
{ }

void ArithmeticAssignmentStatement::execute(vm::ExecutionContext *ctx) const
{
    // TODO: This is horribly inefficient!!
    //
    // 1. Rewrite LiteralExpression slightly so we are able to change the value post-creation,
    //    so we don't have to allocate new instances every single execution.
    // 2. Bind the functions at creation so we don't need to switch the TypeDecl each exec.
    ExpressionValue lhs = 0, rhs = 0;
    getOperands(ctx, lhs, rhs);

    LiteralExpression::Ptr lexpr = std::make_shared<LiteralExpression>(lhs);
    LiteralExpression::Ptr rexpr = std::make_shared<LiteralExpression>(rhs);
    BinaryExpression::Ptr binExpr = std::make_shared<BinaryExpression>(_operator, lexpr, rexpr);

    ExpressionValue nval = binExpr->evaluate(ctx);
    vm::MemoryView memView = _lvalue->getMemoryView(ctx);
    writeResult(memView, nval);
}

void ArithmeticAssignmentStatement::getOperands(vm::ExecutionContext *context,
                                                ExpressionValue &outLeft, 
                                                ExpressionValue &outRight) const
{
    TypeDecl intrinsicType = _lvalue->getType();
    vm::MemoryView memView = _lvalue->getMemoryView(context);

    ExpressionValue reval = _expression->evaluate(context);
    outRight = reval;

    switch (_lvalue->getType().getType()) {
        case TypeDecl::BOOL:
            outLeft = ExpressionValue(intrinsicType, memView.read<bool>());
            break;
        case TypeDecl::CHAR:
            outLeft = ExpressionValue(intrinsicType, memView.read<int8_t>());
            break;
        case TypeDecl::SHORT:
            outLeft = ExpressionValue(intrinsicType, memView.read<int16_t>());
            break;
        case TypeDecl::INT:
            outLeft = ExpressionValue(intrinsicType, memView.read<int32_t>());
            break;
        case TypeDecl::FLOAT:
            outLeft = ExpressionValue(intrinsicType, memView.read<float>());
            break;
        case TypeDecl::DOUBLE:
            outLeft = ExpressionValue(intrinsicType, memView.read<double>());
            break;
        case TypeDecl::POINTER:
            outLeft = ExpressionValue(intrinsicType, memView.read<uint32_t>());
            break;
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

