 
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
  // 创建第一个变量 a
  AllocaInst *alloca_a =
      builder->CreateAlloca(Type::getInt32Ty(*theContext), nullptr, "a");

  //得到常量1 H
  Value *const_1 = ConstantInt::get(*theContext, APInt(32, 'H', true));
  //初始化
  builder->CreateStore(const_1, alloca_a);
  Function *calleeF = theModule->getFunction("putchar");
  //处理参数
  std::vector<Value *> argsV;
  //加载a
  Value *load_a1 =
      builder->CreateLoad(alloca_a->getAllocatedType(), alloca_a, "a");
  argsV.push_back(load_a1);
  //判断参数是否符合 自行处理
  Value *callputchar = builder->CreateCall(calleeF, argsV, "callputchar");
  // begin
  //得到常量2 U
  Value *const_2 = ConstantInt::get(*theContext, APInt(32, 'U', true));
  //初始化
  builder->CreateStore(const_2, alloca_a);
  std::vector<Value *> argsV2;
  //加载a
  Value *load_a2 =
      builder->CreateLoad(alloca_a->getAllocatedType(), alloca_a, "a");
  argsV2.push_back(load_a2);
  //判断参数是否符合 自行处理
  Value *callputchar2 = builder->CreateCall(calleeF, argsV2, "callputchar");

  //得到常量3 S
  Value *const_3 = ConstantInt::get(*theContext, APInt(32, 'S', true));
  //初始化
  builder->CreateStore(const_3, alloca_a);
  std::vector<Value *> argsV3;
  //加载a
  Value *load_a3 =
      builder->CreateLoad(alloca_a->getAllocatedType(), alloca_a, "a");
  argsV3.push_back(load_a3);
  //判断参数是否符合 自行处理
  Value *callputchar3 = builder->CreateCall(calleeF, argsV3, "callputchar");

  //得到常量4 T
  Value *const_4 = ConstantInt::get(*theContext, APInt(32, 'T', true));
  //初始化
  builder->CreateStore(const_4, alloca_a);
  std::vector<Value *> argsV4;
  //加载a
  Value *load_a4 =
      builder->CreateLoad(alloca_a->getAllocatedType(), alloca_a, "a");
  argsV4.push_back(load_a4);
  //判断参数是否符合 自行处理
  Value *callputchar4 = builder->CreateCall(calleeF, argsV4, "callputchar");

  //得到常量5 C
  Value *const_5 = ConstantInt::get(*theContext, APInt(32, 'C', true));
  //初始化
  builder->CreateStore(const_5, alloca_a);
  std::vector<Value *> argsV5;
  //加载a
  Value *load_a5 =
      builder->CreateLoad(alloca_a->getAllocatedType(), alloca_a, "a");
  argsV5.push_back(load_a5);
  //判断参数是否符合 自行处理
  Value *callputchar5 = builder->CreateCall(calleeF, argsV5, "callputchar");

  //得到常量6 S
  Value *const_6 = ConstantInt::get(*theContext, APInt(32, 'S', true));
  //初始化
  builder->CreateStore(const_6, alloca_a);
  std::vector<Value *> argsV6;
  //加载a
  Value *load_a6 =
      builder->CreateLoad(alloca_a->getAllocatedType(), alloca_a, "a");
  argsV6.push_back(load_a6);
  //判断参数是否符合 自行处理
  Value *callputchar6 = builder->CreateCall(calleeF, argsV6, "callputchar");

  //得到常量7 T
  Value *const_7 = ConstantInt::get(*theContext, APInt(32, 'E', true));
  //初始化
  builder->CreateStore(const_7, alloca_a);
  std::vector<Value *> argsV7;
  //加载a
  Value *load_a7 =
      builder->CreateLoad(alloca_a->getAllocatedType(), alloca_a, "a");
  argsV7.push_back(load_a7);
  //判断参数是否符合 自行处理
  Value *callputchar7 = builder->CreateCall(calleeF, argsV7, "callputchar");
  // end
  //设置返回值
  builder->CreateRet(const_1);
  verifyFunction(*f);
  // Run the optimizer on the function.
  // theFPM->run(*f);
  //输出
  theModule->print(outs(), nullptr);

  return 0;
}
