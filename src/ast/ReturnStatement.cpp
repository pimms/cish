#include "ReturnStatement.h"

#include "DeclarationContext.h"
#include "FunctionDefinition.h"

#include "../vm/ExecutionContext.h"

namespace cish::ast
{

ReturnStatement::ReturnStatement(DeclarationContext *context, Expression::Ptr expr):
    _expression(expr)
{
    const FunctionDefinition::Ptr functionDefinition = context->getCurrentFunction();
    if (functionDefinition == nullptr) {
        Throw(InvalidStatementException, "Cannot return outside of a function");
    }

    const auto returnType = functionDefinition->getDeclaration()->returnType;

    if (expr == nullptr) {
        if (returnType.getType() != TypeDecl::VOID) {
            Throw(InvalidTypeException, "Cannot return without value from non-void function");
        }
    } else {
        const auto actualType = expr->getType();

        if (actualType == TypeDecl(TypeDecl::VOID)) {
            Throw(InvalidCastException, "Cannot return void");
        }
        if (!actualType.castableTo(returnType)) {
            Throw(InvalidCastException, "Cannot convert type of value '%s' to expected return type '%s'",
                    actualType.getName(), returnType.getName());
        }
    }
}

void ReturnStatement::virtualExecute(vm::ExecutionContext *context) const
{
    if (_expression) {
        ExpressionValue value = getReturnValue(context);
        context->returnCurrentFunction(value);
    } else {
        context->returnCurrentFunction(TypeDecl::VOID);
    }
}

ExpressionValue ReturnStatement::getReturnValue(vm::ExecutionContext *context) const
{
    ExpressionValue value = _expression->evaluate(context);
    const TypeDecl &type = value.getIntrinsicType();

    if (type != TypeDecl::STRUCT) {
        return ExpressionValue(value);
    }

    // -- Struct-by-value --
    //
    // We need to copy the contents of 'value' into the call-site defined
    // return-buffer, retrievable from the execution-context.
    //
    // We then return the address of the return-buffer; keep in mind that
    // the struct being returned is allocated on the stack and will be
    // freed shortly!
    const uint32_t sourceAddr = value.get<uint32_t>();
    const uint32_t size = type.getSize();
    vm::MemoryView sourceView = context->getMemory()->getView(sourceAddr);
    const uint8_t *sourceBuf = sourceView.readBuf(size);

    vm::Variable *returnBuffer = context->getCurrentFunctionReturnBuffer();
    returnBuffer->getAllocation()->writeBuf(sourceBuf, size);

    return ExpressionValue(type, returnBuffer->getHeapAddress());
}

}
