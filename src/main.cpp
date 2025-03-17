#include "../include/classFile/class_file.hpp"
#include <array>
#include <cstddef>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <llvm-20/llvm/IR/IRBuilder.h>
#include <llvm-20/llvm/IR/PassManager.h>
#include <llvm-20/llvm/Support/raw_ostream.h>
#include <memory>
#include <string>
#include <spdlog/spdlog.h>
#include <mutex>

#include <llvm-20/llvm/IR/BasicBlock.h>
#include <llvm-20/llvm/IR/DerivedTypes.h>
#include <llvm-20/llvm/IR/Instruction.h>
#include <condition_variable>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>

using namespace llvm;

struct MyFunctionPass : public PassInfoMixin<MyFunctionPass> {
    PreservedAnalyses run(Function& F, FunctionAnalysisManager& AM) {
        // 在这里添加你的自定义分析或转换代码
        errs() << "Running MyFunctionPass on function: " << F.getName() << "\n";
        return PreservedAnalyses::all();
    }
};

bool modifyModule(Module& M) {
    return false;
}

Function* createFunction(LLVMContext& Context, Module& Owner) {
    auto* FT = FunctionType::get(Type::getInt32Ty(Context),
                                 {Type::getInt32Ty(Context), Type::getInt32Ty(Context)}, false);
    auto* F = Function::Create(FT, Function::ExternalLinkage, "add function", Owner);
    auto* ArgX = F->getArg(0);
    auto* ArgY = F->getArg(1);

    auto* EntryBlock = BasicBlock::Create(Context, "entry", F);
    auto* ReturnBlock = BasicBlock::Create(Context, "return", F);

    IRBuilder<> EntryBlockIRBuilder(EntryBlock);
    IRBuilder<> ReturnBlockIRBuilder(ReturnBlock);

    auto* Value = EntryBlockIRBuilder.CreateAdd(ArgX, ArgY);
    ReturnBlockIRBuilder.CreateRet(Value);

    return F;
}

int main(void) {
    LLVMContext Context;
    std::unique_ptr<Module> Owner(new Module("demo", Context));
    createFunction(Context, *Owner);

    auto* F = Owner->getFunction("llvm.memcpy.p0.p0.i64");
    if (!F) {
        errs() << "Function not found\n";
        return 1;
    }
    auto* FT = F->getFunctionType();
    for (auto& Attr : F->getAttributes()) {
        errs() << Attr.getAsString() << "\n";
    }
    return 0;
}