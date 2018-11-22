#include <gtest/gtest.h>

#include <functional>

#include "module/ModuleContext.h"
#include "module/Module.h"
#include "module/Function.h"
#include "../TestHelpers.h"


using namespace cish::vm;
using namespace cish::ast;
using namespace cish::module;

typedef std::function<int(int)> TestCallback;

class TestFunction: public Function
{
public:
    TestFunction(const std::string name, TestCallback callback):
        Function(FuncDeclaration{TypeDecl::INT, name, {VarDeclaration{TypeDecl::INT, "n"}}}),
        _callback(callback),
        _callCount(0)
    {

    }

    void verifyParams(const std::vector<ExpressionValue>& params) const
    {
        ASSERT_EQ(1, params.size());
        ASSERT_EQ(TypeDecl::INT, params[0].getIntrinsicType().getType());
    }

    ExpressionValue execute(ExecutionContext *context,
                            const std::vector<ExpressionValue>& params) const override
    {
        verifyParams(params);
        _callCount++;
        const int pval = params[0].get<int>();
        return ExpressionValue(_callback(pval));
    }

    int getCallCount()
    {
        return _callCount;
    }

private:
    TestCallback _callback;
    mutable int _callCount;
};

ModuleContext::Ptr singletonModuleContext(std::string name, Callable::Ptr func)
{
    Module::Ptr module = Module::Ptr(new Module(name));
    module->addFunction(func);
    ModuleContext::Ptr moduleContext = ModuleContext::create();
    moduleContext->addModule(module);
    return std::move(moduleContext);
}


TEST(ModuleTest, simpleEchoModule)
{
    TestFunction *rawFunc = new TestFunction("echo", [](int n) -> int { return n; });
    TestFunction::Ptr func = TestFunction::Ptr(rawFunc);
    auto modContext = singletonModuleContext("echo.h", func);

    const std::string source =
        "#include <echo.h>"
        "int main() {"
        "   return echo(5);"
        "}";

    assertExitCode(std::move(modContext), source, 5);
    ASSERT_EQ(1, rawFunc->getCallCount());
}
