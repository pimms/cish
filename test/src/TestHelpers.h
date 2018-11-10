#pragma once

#include <gtest/gtest.h>
#include <memory>

#include "ast/AntlrContext.h"
#include "ast/AstBuilder.h"
#include "ast/Ast.h"

#include "vm/ExecutionContext.h"
#include "vm/VirtualMachine.h"
#include "vm/Variable.h"


typedef std::shared_ptr<cish::vm::VirtualMachine> VmPtr;


cish::ast::Ast::Ptr createAst(const std::string &source);
VmPtr createVm(const std::string &source);
cish::vm::Variable* getVar(VmPtr vm, const std::string &name);
