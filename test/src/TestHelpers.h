#pragma once

#include <gtest/gtest.h>
#include <memory>

#include "ast/AntlrContext.h"
#include "ast/AstBuilder.h"
#include "ast/Ast.h"

#include "vm/ExecutionContext.h"
#include "vm/VirtualMachine.h"
#include "vm/Variable.h"

#include "module/ModuleContext.h"

typedef std::shared_ptr<cish::vm::VirtualMachine> VmPtr;


cish::ast::Ast::Ptr createAst(const std::string &source);
cish::ast::Ast::Ptr createAst(cish::module::ModuleContext::Ptr moduleContext, const std::string &source);

VmPtr createVm(const std::string &source);
VmPtr createVm(cish::module::ModuleContext::Ptr moduleContext, const std::string &source);

cish::vm::Variable* getVar(VmPtr vm, const std::string &name);

void assertExitCode(const std::string &source, int expectedExitCode);
void assertExitCode(cish::module::ModuleContext::Ptr moduleContext, const std::string &source, int expectedExitCode);
void assertExitCodeStdlib(const std::string &source, int expectedExitCode);

void assertRuntimeFailure(const std::string &source);
void assertCompilationFailure(const std::string &source);

