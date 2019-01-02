#include <gtest/gtest.h>

#include "ast/FunctionDefinition.h"
#include "ast/DeclarationContext.h"
#include "ast/StructLayout.h"
#include "ast/StructField.h"

#include "vm/ExecutionContext.h"
#include "vm/Memory.h"

using namespace cish::ast;
using namespace cish::vm;


static StructLayout::Ptr createStruct()
{
    StructLayout *layout = new StructLayout("item_t");
    layout->addField(new StructField(TypeDecl::INT, "n"));
    layout->finalize();
    return StructLayout::Ptr(layout);
}


TEST(FunctionDefinitionTest, definitionImplicitlyDeclares)
{
    DeclarationContext dc;

    FuncDeclaration decl;
    decl.returnType = TypeDecl::INT;
    decl.name = "main";

    FunctionDefinition def(&dc, decl);

    ASSERT_NE(nullptr, dc.getFunctionDeclaration("main"));
}

TEST(FunctionDefinitionTest, structByValFuncsRequireReturnBuffer)
{
    StructLayout::Ptr layout = createStruct();

    Memory memory(128, 4);
    ExecutionContext ec(&memory);
    DeclarationContext dc;
    dc.declareStruct(layout);

    FuncDeclaration decl;
    decl.name = "foo";
    decl.returnType = TypeDecl::getStruct(layout);

    FunctionDefinition def(&dc, decl);

    ASSERT_ANY_THROW(def.execute(&ec, {}, nullptr));

    // Now let's allocate a return buffer
    Allocation::Ptr alloc = memory.allocate(layout->getSize());
    Variable returnBuffer(decl.returnType, std::move(alloc));
    ASSERT_NO_THROW(def.execute(&ec, {}, &returnBuffer));
}

