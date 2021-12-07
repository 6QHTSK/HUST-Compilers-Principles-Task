 
#include <iostream>

#include <string>
#include <vector>

#include <algorithm>
#include <assert.h>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Utils.h"

#include <fstream>

using namespace llvm;

std::unique_ptr<LLVMContext> theContext;
std::unique_ptr<Module> theModule;
std::unique_ptr<IRBuilder<>> builder;
std::map<std::string, AllocaInst *> namedValues;
std::unique_ptr<legacy::FunctionPassManager> theFPM;

std::map<std::string, AllocaInst *> curNamedValues;

BasicBlock *continueBasicBlock = nullptr;

void InitializeModuleAndPassManager() {
  // Open a new module.
  theContext = std::make_unique<LLVMContext>();
  theModule = std::make_unique<Module>("test", *theContext);

  // theModule->setDataLayout(dL);

  // Create a new builder for the module.
  builder = std::make_unique<IRBuilder<>>(*theContext);

  // Create a new pass manager attached to it.
  theFPM = std::make_unique<legacy::FunctionPassManager>(theModule.get());

  // Promote allocas to registers.
  //theFPM->add(createPromoteMemoryToRegisterPass());
  // Do simple "peephole" optimizations and bit-twiddling optzns.
  //theFPM->add(createInstructionCombiningPass());
  // Reassociate expressions.
  //theFPM->add(createReassociatePass());
  // Eliminate Common SubExpressions.
  //theFPM->add(createGVNPass());
  // Simplify the control flow graph (deleting unreachable blocks, etc).
  //theFPM->add(createCFGSimplificationPass());

  theFPM->doInitialization();
}

int main(int argc, char *argv[]) {
  // Init
  InitializeModuleAndPassManager();

  //默认输出函数putchar
  std::vector<Type *> putArgs;
  putArgs.push_back(Type::getInt32Ty(*theContext));

  FunctionType *putType =
      FunctionType::get(builder->getInt32Ty(), putArgs, false);
  Function *putFunc = Function::Create(putType, Function::ExternalLinkage,
                                       "putchar", theModule.get());
  //默认输入函数getchar
  std::vector<Type *> getArgs;

  FunctionType *getType =
      FunctionType::get(builder->getInt32Ty(), getArgs, false);
  Function *getFunc = Function::Create(getType, Function::ExternalLinkage,
                                       "getchar", theModule.get());
  //根据输入的单字符，判断，如果是'a'，则输出'Y'，否则输出'N'。
  //设置返回类型
  //begin
  //设置返回类型

  Type *retType = Type::getInt32Ty(*theContext);
  std::vector<Type *> argsTypes;     //参数类型
  std::vector<std::string> argNames; //参数名
  //无参，所以不push内容
  //得到函数类型
  FunctionType *ft = FunctionType::get(retType, argsTypes, false);
  //创建函数
  Function *f =
      Function::Create(ft, Function::ExternalLinkage, "main", theModule.get());
  //为函数的参数设置名字
  unsigned idx = 0;
  for (auto &arg : f->args()) {
    arg.setName(argNames[idx++]);
  }
  //创建第一个基本块 函数入口
  BasicBlock *bb = BasicBlock::Create(*theContext, "entry", f);
  builder->SetInsertPoint(bb);
  // 为参数变量申请空间
  // 无参
  AllocaInst *alloca_a =
      builder->CreateAlloca(Type::getInt32Ty(*theContext), nullptr, "a");
  AllocaInst *alloca_b =
      builder->CreateAlloca(Type::getInt32Ty(*theContext), nullptr, "b");
  Value *const_0 = ConstantInt::get(*theContext, APInt(32, 0, true));
  builder->CreateStore(const_0, alloca_a);
  Value *const_1 = ConstantInt::get(*theContext, APInt(32, 1, true));
  builder->CreateStore(const_1, alloca_a);
  Function *calleeF1 = theModule->getFunction("getchar");
  std::vector<Value *> argsV0;
  Value *callgetchar = builder->CreateCall(calleeF1,argsV0,"callgetchar");
  builder->CreateStore(callgetchar, alloca_b);
  Value* load_b1 = builder->CreateLoad(alloca_b->getAllocatedType(), alloca_b, "b");
  Value *const_2 = ConstantInt::get(*theContext, APInt(32, 'a', true));
  Value *compare =
      builder->CreateICmpEQ(load_b1, const_2, "comp");
  Value *condVal = builder->CreateICmpNE(
      compare, Constant::getNullValue(compare->getType()), "cond");
  BasicBlock *thenb = BasicBlock::Create(*theContext, "then", f);
  BasicBlock *elseb = BasicBlock::Create(*theContext, "else", f);
  BasicBlock *ifcontb = BasicBlock::Create(*theContext, "ifcont");
  builder->CreateCondBr(condVal, thenb, elseb);
  builder->SetInsertPoint(thenb);
  Value *const_3 = ConstantInt::get(*theContext, APInt(32, 'Y', true));
  builder->CreateStore(const_3, alloca_a);
  builder->CreateBr(ifcontb);
  builder->SetInsertPoint(elseb);
  Value *const_4 = ConstantInt::get(*theContext, APInt(32, 'N', true));
  builder->CreateStore(const_4, alloca_a);
  builder->CreateBr(ifcontb);

  f->getBasicBlockList().push_back(ifcontb);
  builder->SetInsertPoint(ifcontb);
  Value* load_a1 = builder->CreateLoad(alloca_a->getAllocatedType(),alloca_a,"a");
  Function *calleeF2 = theModule->getFunction("putchar");
  std::vector<Value *> argsV1;
  argsV1.push_back(load_a1);
  //判断参数是否符合 自行处理
  Value *callputchar = builder->CreateCall(calleeF2, argsV1, "callputchar");
  Value* const_5 = ConstantInt::get(*theContext, APInt(32, 10, true));
  std::vector<Value *> argsV2;
  argsV2.push_back(const_5);
  //判断参数是否符合 自行处理
  Value *callputchar2 = builder->CreateCall(calleeF2, argsV2, "callputchar");
  // end
  //设置返回值
  builder->CreateRet(const_0);
  verifyFunction(*f);
  // Run the optimizer on the function.
  // theFPM->run(*f);
  //输出
  theModule->print(outs(), nullptr);

  return 0;
}
