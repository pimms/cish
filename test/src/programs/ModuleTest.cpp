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
                            const std::vector<ExpressionValue>& params,
                            Variable*) const override
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

class StructFunction: public Function {
public:
    StructFunction(const StructLayout *lib_t):
        Function(FuncDeclaration { TypeDecl::getStruct(lib_t), "struct_func", {}}),
        _structLayout(lib_t)
    {}

    ExpressionValue execute(ExecutionContext *context,
                            const std::vector<ExpressionValue>& params,
                            Variable *returnBuffer) const override
    {
        returnBuffer->getAllocation()->write<int32_t>(999, 0);
        returnBuffer->getAllocation()->write<int32_t>(888, 4);
        return ExpressionValue(TypeDecl::getStruct(_structLayout), returnBuffer->getHeapAddress());
    }

private:
    const StructLayout *_structLayout;
};

ModuleContext::Ptr singletonModuleContext(std::string name, Callable::Ptr func)
{
    Module::Ptr module = Module::Ptr(new Module(name));

    if (func) {
        module->addFunction(func);
    }

    StructLayout *structLayout = new StructLayout("lib_t");
    structLayout->addField(new StructField(TypeDecl::INT, "a"));
    structLayout->addField(new StructField(TypeDecl::INT, "b"));
    structLayout->finalize();
    module->addStruct(StructLayout::Ptr(structLayout));
    module->addFunction(std::make_shared<StructFunction>(structLayout));

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

TEST(ModuleTest, moduleDeclaredStruct)
{
    auto modContext = singletonModuleContext("module.h", nullptr);

    const std::string source = 
        "#include <module.h>"
        "int main() {"
        "   struct lib_t var = struct_func();"
        "   return var.a + var.b;"
        "}";

    assertExitCode(std::move(modContext), source, 999 + 888);

}
