#include "astnode.h"

extern int spaces;
extern std::unique_ptr<LLVMContext> theContext;
extern std::unique_ptr<Module> theModule;
extern std::unique_ptr<IRBuilder<>> builder;
extern std::map<std::string, AllocaInst *> namedValues;
extern std::unique_ptr<legacy::FunctionPassManager> theFPM;
extern int grammererror;
extern std::map<std::string, AllocaInst *> curNamedValues;

extern BasicBlock *continueBasicBlock;
void printspaces() {
  for (int i = 0; i < spaces; ++i)
    std::cout << " ";
}
void printGrammerInfo(std::string nodeName, int line) {
  printspaces();
  std::cout << nodeName << " (" << line << ")" << std::endl;
}

void printSemanticError(int type, int line, std::string info = "") {
  grammererror = 1;
  std::cout << "Error type " << type << " at Line " << line << "."
            << std::endl;
}

int parseNIdentifier(NIdentifier &nIdentifier) {
  printspaces();
  std::cout << "ID: " << nIdentifier.name << std::endl;
  return 0;
}

Value *LogErrorV(const char *Str) {
  // std::cout << Str << std::endl;
  return nullptr;
}

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

Function *getFunction(std::string Name) {
  // First, see if the function has already been added to the current module.
  if (auto *F = theModule->getFunction(Name))
    return F;
}

/// CreateEntryBlockAlloca - Create an alloca instruction in the entry block
/// of the function.  This is used for mutable variables etc.
static AllocaInst *CreateEntryBlockAlloca(Function *TheFunction,
                                          StringRef VarName, Type *varType) {
  IRBuilder<> TmpB(&TheFunction->getEntryBlock(),
                   TheFunction->getEntryBlock().begin());
  return TmpB.CreateAlloca(varType, nullptr, VarName);
}

int NInteger::parse() {
  printGrammerInfo(getNodeName(), line);
  spaces += 2;
  printspaces();
  std::cout << "INT"
            << ": " << value << std::endl;
  spaces -= 2;
  return 0;
}
int NFloat::parse() {
  printGrammerInfo(getNodeName(), line);
  spaces += 2;
  printspaces();
  std::cout << "FLOAT"
            << ": " << value << std::endl;
  spaces -= 2;
  return 0;
}
int NChar::parse() {
  printGrammerInfo(getNodeName(), line);
  spaces += 2;
  printspaces();
  std::cout << "CHAR"
            << ": " << value << std::endl;
  spaces -= 2;
  return 0;
}
int NIdentifier::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  printspaces();
  std::cout << "ID"
            << ": " << name << std::endl;
  spaces -= 2;
  return 0;
}
int NDotOperator::parse() {
  printGrammerInfo(getNodeName(), line);
  spaces += 2;
  exp.parse();
  printspaces();
  std::cout << "DOT" << std::endl;
  parseNIdentifier(id);
  // id.parse();
  spaces -= 2;
  return 0;
}
int NListOperator::parse() {
  printGrammerInfo(getNodeName(), line);
  spaces += 2;
  lhs.parse();
  printspaces();
  std::cout << "LB" << std::endl;
  rhs.parse();
  printspaces();
  std::cout << "RB" << std::endl;
  spaces -= 2;
  return 0;
}
int NArgs::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  exp.parse();
  if (nArgs) {
    printspaces();
    std::cout << "COMMA" << std::endl;
    nArgs->parse();
  }
  spaces -= 2;
  return 0;
}
int NMethodCall::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  parseNIdentifier(id);
  // id.parse();
  printspaces();
  std::cout << "LP" << std::endl;
  if (nargs) {
    nargs->parse();
  }
  printspaces();
  std::cout << "RP" << std::endl;
  spaces -= 2;
  return 0;
}
int NParenOperator::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  printspaces();
  std::cout << "LP" << std::endl;
  printspaces();
  exp.parse();
  printspaces();
  std::cout << "RP" << std::endl;
  spaces -= 2;
  return 0;
}
int NSingleOperator::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  printspaces();
  std::cout << name << std::endl;
  hs.parse();
  spaces -= 2;
  return 0;
}
int NBinaryOperator::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  lhs.parse();
  printspaces();
  if (name.substr(0, 5) == "RELOP")
    std::cout << "RELOP" << std::endl;
  else
    std::cout << name << std::endl;
  rhs.parse();
  spaces -= 2;
  return 0;
}
int NAssignment::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  lhs.parse();
  printspaces();
  std::cout << name << std::endl;
  rhs.parse();
  spaces -= 2;
  return 0;
}
int NSpecifier::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  printspaces();
  std::cout << "TYPE: " << type << std::endl;
  spaces -= 2;
  return 0;
}
int NVarDec::parse() {
  printGrammerInfo(getNodeName(), line);

  if (v.size()) {
    spaces += 2;
    for (int i = 0; i < v.size(); ++i) {
      printGrammerInfo(getNodeName(), line);

      spaces += 2;
    }
    parseNIdentifier(Id);
    // Id.parse();
    spaces -= 2;
    for (int i = 0; i < v.size(); ++i) {
      printspaces();
      std::cout << "LB" << std::endl;
      printspaces();
      std::cout << "INT: " << v[i] << std::endl;
      printspaces();
      std::cout << "RB" << std::endl;
      spaces -= 2;
    }
  } else {
    spaces += 2;
    parseNIdentifier(Id);
    // Id.parse();
    spaces -= 2;
  }
  return 0;
}
int NParamDec::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  nSpecifier.parse();
  varDec.parse();
  spaces -= 2;
  return 0;
}
int NVarList::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  nParamDec.parse();
  if (nVarList) {
    printspaces();
    std::cout << "COMMA" << std::endl;
    nVarList->parse();
  }
  spaces -= 2;
  return 0;
}
int NFunDec::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  parseNIdentifier(Id);
  // Id.parse();
  printspaces();
  std::cout << "LP" << std::endl;
  if (arguments)
    arguments->parse();
  printspaces();
  std::cout << "RP" << std::endl;
  spaces -= 2;
  return 0;
}
int NDec::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  vardec.parse();
  if (exp) {
    printspaces();
    std::cout << "ASSIGNOP" << std::endl;
    exp->parse();
  }
  spaces -= 2;
  return 0;
}
int NDecList::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  dec.parse();
  if (nDecList) {
    printspaces();
    std::cout << "COMMA" << std::endl;
    nDecList->parse();
  }
  spaces -= 2;
  return 0;
}
int NDef::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  nSpecifier.parse();
  if (nDecList)
    nDecList->parse();
  printspaces();
  std::cout << "SEMI" << std::endl;
  spaces -= 2;
  return 0;
}
int NDefList::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  nDef.parse();
  if (nDefList) {
    nDefList->parse();
  }
  spaces -= 2;
  return 0;
}
int NStructSpecifier::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  printGrammerInfo("StructSpecifier", line);

  spaces += 2;
  printspaces();
  std::cout << "STRUCT" << std::endl;
  if (deflist) {
    if (tag) {
      printGrammerInfo("OptTag", line);
      spaces += 2;
      parseNIdentifier(*tag);
      spaces -= 2;
      printspaces();
      std::cout << "LC" << std::endl;
      deflist->parse();
      printspaces();
      std::cout << "RC" << std::endl;
    } else {
      deflist->parse();
    }
  } else if (tag) {
    printGrammerInfo("Tag", line);

    spaces += 2;
    parseNIdentifier(*tag);
    spaces -= 2;
  }
  spaces -= 2;
  spaces -= 2;
  return 0;
}
int NStmtList::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  nStmt.parse();
  if (nStmtList)
    nStmtList->parse();
  spaces -= 2;
  return 0;
}

int NCompSt::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  printspaces();
  std::cout << "LC" << std::endl;
  if (ndeflist)
    ndeflist->parse();
  if (nstmtlist)
    nstmtlist->parse();
  printspaces();
  std::cout << "RC" << std::endl;
  spaces -= 2;
  return 0;
}
int NExpStmt::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  this->exp.parse();
  printspaces();
  std::cout << "SEMI" << std::endl;
  spaces -= 2;
  return 0;
}
int NCompStStmt::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  compst.parse();
  spaces -= 2;
  return 0;
}
int NRetutnStmt::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  printspaces();
  std::cout << "RETURN" << std::endl;
  this->exp.parse();
  printspaces();
  std::cout << "SEMI" << std::endl;
  spaces -= 2;
  return 0;
}
int NIfStmt::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  printspaces();
  std::cout << "IF" << std::endl;
  printspaces();
  std::cout << "LP" << std::endl;
  this->exp.parse();
  printspaces();
  std::cout << "RP" << std::endl;
  this->stmt.parse();
  spaces -= 2;
  return 0;
}
int NIfElseStmt::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  printspaces();
  std::cout << "IF" << std::endl;
  printspaces();
  std::cout << "LP" << std::endl;
  this->exp.parse();
  printspaces();
  std::cout << "RP" << std::endl;
  this->stmt.parse();
  printspaces();
  std::cout << "ELSE" << std::endl;
  this->stmt_else.parse();
  spaces -= 2;
  return 0;
}
int NWhileStmt::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  printspaces();
  std::cout << "WHILE" << std::endl;
  printspaces();
  std::cout << "LP" << std::endl;
  this->exp.parse();
  printspaces();
  std::cout << "RP" << std::endl;
  this->stmt.parse();
  spaces -= 2;
  return 0;
}
int NBreakStmt::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  printspaces();
  std::cout << "BREAK" << std::endl;
  printspaces();
  std::cout << "SEMI" << std::endl;
  spaces -= 2;
  return 0;
}
int NExtDecList::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  nVarDec.parse();
  if (nExtDecList) {
    printspaces();
    std::cout << "COMMA" << std::endl;
    nExtDecList->parse();
  }
  spaces -= 2;
  return 0;
}
int NExtDef::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  specifier.parse();
  if (fundec) {
    fundec->parse();
    if (compst) {
      compst->parse();
    }
  } else {
    if (nextdeclist) {
      nextdeclist->parse();
    }
    printspaces();
    std::cout << "SEMI" << std::endl;
  }

  spaces -= 2;
  return 0;
}
int NExtDefList::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  nExtDef.parse();
  if (nExtDefList)
    nExtDefList->parse();
  spaces -= 2;
  return 0;
}
int NProgram::parse() {
  printGrammerInfo("Program", line);
  spaces += 2;
  if (nextdeflist)
    nextdeflist->parse();
  spaces -= 2;
  return 0;
}

// codegen()
Value *Node::codegen() {
  assert(false); // Never use this function.
  // This is a list.
  return ConstantInt::get(*theContext, APInt(32, 0, true));
}
Value *NExpression::codegen() {
  return ConstantInt::get(*theContext, APInt(32, 0, true));
}
Value *NInteger::codegen() {
  return ConstantInt::get(*theContext, APInt(32, value, true));
}
Value *NFloat::codegen() {
  // begin
  return ConstantFP::get(*theContext, APFloat(value));
  // end
}
Value *NChar::codegen() {
  // begin

  return nullptr;
  // end
}
Value *NIdentifier::codegen() {
  // begin
  auto iter = namedValues.find(name);
  if(iter == namedValues.end()){
    iter = curNamedValues.find(name);
    if(iter == curNamedValues.end()){
      printSemanticError(1,line,"Not Defined" + name);
      return nullptr;
    }
  }
  AllocaInst* alloca = iter->second;
  return builder->CreateLoad(alloca->getAllocatedType(),alloca,name);
  // end
}
Value *NArgs::codegen() { return exp.codegen(); }
Value *NMethodCall::codegen() {
  // begin
  Function* callee = theModule->getFunction(id.name);
  if(callee == nullptr){
    printSemanticError(2,line,"Not Defined" + id.name);
    return nullptr;
  }
  std::vector<Value *>argsV;
  NArgs *current = nargs;
  auto calleeFt = callee->getFunctionType();
  auto paramCount = calleeFt->getNumParams();
  int i;
  for(int i=0;i<paramCount;i++){
    if(current == nullptr){
      printSemanticError(8,line,"BUPIPEI");
      return nullptr;
    }
    Value* exp = current->codegen();
    if(calleeFt->getParamType(i)->getPointerTo() != exp->getType()->getPointerTo()){
      printSemanticError(8,line,"BUPIPEI");
      return nullptr;
    }
    argsV.push_back(exp);
    current = current->nArgs;
  }
  Value* callValue = builder->CreateCall(callee,argsV,id.name);
  return callValue;
  // end
}
Value *NParenOperator::codegen() { return exp.codegen(); }
Value *NSingleOperator::codegen() {
  // begin

  return nullptr;
  // end
}
Value *NBinaryOperator::codegen() {
  // begin
  Value* leftExp = lhs.codegen();
  Value* rightExp = rhs.codegen();
  Value* returnExp = nullptr;
  if(leftExp == nullptr || rightExp == nullptr){
    return nullptr;
  }
  if(name == "PLUS"){
    returnExp = builder->CreateAdd(leftExp,rightExp,"add");
  }else if(name == "MINUS"){
    returnExp = builder->CreateSub(leftExp,rightExp,"sub");
  }else if(name == "STAR"){
    returnExp = builder->CreateMul(leftExp,rightExp,"mul");
  }else if(name == "DIV"){
    returnExp = builder->CreateSDiv(leftExp,rightExp,"div");
  }else if(name.substr(0, 5) == "RELOP"){
    std::string op=name.substr(5);
    ICmpInst::Predicate pred;
    if(op == "=="){
      pred = ICmpInst::ICMP_EQ;
    }else if(op == "<="){
      pred = ICmpInst::ICMP_SLE;
    }else if(op == ">="){
      pred = ICmpInst::ICMP_SGE;
    }else if(op == "!="){
      pred = ICmpInst::ICMP_NE;
    }
    returnExp = builder->CreateICmp(pred,leftExp,rightExp,"comp");
  }else if(name == "OR"){
    returnExp = builder->CreateOr(leftExp,rightExp,"or");
  }
  return returnExp;
  // end
}
Value *NAssignment::codegen() {
  // Assignment requires the LHS to be an identifier.
  // begin
  if(lhs.name == ""){
    printSemanticError(6,line,"YOU ZHI FANG ZUO BIAN");
    return nullptr;
  }
  auto iter = namedValues.find(lhs.name);
  if(iter == namedValues.end()){
    iter = curNamedValues.find(lhs.name);
    if(iter == curNamedValues.end()){
      printSemanticError(1,line,"Undefined Varieable"+lhs.name);
      return nullptr;
    }
  }
  AllocaInst* alloca = iter->second;
  Value* nextExp = rhs.codegen();
  if(nextExp == nullptr){
    return nullptr;
  }
  Type* nextExpType = nextExp->getType()->getPointerTo();
  if(nextExpType == alloca->getType()){
    return builder->CreateStore(nextExp,alloca); // ?
  }else{
    printSemanticError(5,line,"UnMatch Type");
    return nullptr;
  }
  // end
}
Value *NSpecifier::codegen() {
  // begin

  return nullptr;
  // end
}
Type *NSpecifier::getType() {
  if (type == "int")
    return Type::getInt32Ty(*theContext);
  if (type == "float")
    return Type::getFloatTy(*theContext);
  if (type == "char")
    return Type::getInt8Ty(*theContext);
  assert(false);
  return Type::getInt32Ty(*theContext);
}
Value *NVarDec::codegen() {
  // begin

  return nullptr;
  // end
}
Value *NParamDec::codegen() {
  // begin

  return nullptr;
  // end
}

std::pair<std::string, Type *> NParamDec::getType() {
  assert(varDec.v.size() == 0);
  std::pair<std::string, Type *> tmp(varDec.Id.name, nSpecifier.getType());
  return tmp;
}
Value *NVarList::codegen() {
  assert(false); // Never use this function.
  // This is a list.
  return ConstantInt::get(*theContext, APInt(32, 0, true));
}
Function *NFunDec::funcodegen(Type *retType) {
  // check if it exists the same name of fun
  if (theModule->getFunction(Id.name)) {
    printSemanticError(4, line, "Redefined " + Id.name);
    return nullptr;
  }

  std::vector<Type *> argsTypes;
  std::vector<std::string> argNames;
  for (NVarList *item = arguments; item; item = item->nVarList) {
    auto tmp = item->nParamDec.getType();
    argNames.push_back(tmp.first);
    argsTypes.push_back(tmp.second);
  }

  FunctionType *ft = FunctionType::get(retType, argsTypes, false);
  Function *f =
      Function::Create(ft, Function::ExternalLinkage, Id.name, theModule.get());
  unsigned idx = 0;
  for (auto &arg : f->args()) {
    arg.setName(argNames[idx++]);
  }
  return f;
}
Value *NDef::codegen() {
  // begin
  // 获得此def的type
  Type *retType = nSpecifier.getType();
  auto current = nDecList;
  // 遍历declist
  while(current != nullptr){
    auto variableID = current->dec.vardec.Id;
    if (namedValues[std::string(variableID.name)]) {
      printSemanticError(3, line, "Redefined " + variableID.name);
      return LogErrorV("Unknown Varieable referenced"); // ?
    }
    AllocaInst* alloca = builder->CreateAlloca(retType,nullptr,variableID.name);
    if(current->dec.exp != nullptr){
      Value* exp;
      exp = current->dec.exp->codegen();
      builder->CreateStore(exp,alloca);
    }
    namedValues[std::string(variableID.name)] = alloca;
    current = current->nDecList;
  }
  return nullptr; // 暂时return nullptr
  // end
}
Value *NDefList::codegen() {
  // begin
  auto *retval = nDef.codegen();
  if(nDefList)
    retval = nDefList->codegen();
  return retval;
  // end
}
Value *NStmtList::codegen() {
  auto *retVal = nStmt.codegen();
  if (nStmtList)
    retVal = nStmtList->codegen();
  return retVal;
}
Value *NCompSt::codegen() {
  // 自行处理变量作用域的问题
  Value *retVal = nullptr;
  if (ndeflist)
    retVal = ndeflist->codegen();
  if (nstmtlist)
    retVal = nstmtlist->codegen();
  return retVal;
}
Value *NExpStmt::codegen() { return exp.codegen(); }
Value *NCompStStmt::codegen() {
  // begin
  std::map<std::string, AllocaInst *> oldNamedValues, oldCurNamedValues ;
  copy(namedValues.begin(),namedValues.end(),std::inserter(oldNamedValues,oldNamedValues.begin()));
  copy(curNamedValues.begin(),curNamedValues.end(),std::inserter(oldCurNamedValues,oldCurNamedValues.begin()));
  curNamedValues.insert(namedValues.begin(),namedValues.end());
  namedValues.clear();
  Value* retVal = compst.codegen();
  curNamedValues.clear();
  namedValues.clear();
  copy(oldNamedValues.begin(),oldNamedValues.end(),std::inserter(namedValues,namedValues.begin()));
  copy(oldCurNamedValues.begin(),oldCurNamedValues.end(),std::inserter(curNamedValues,curNamedValues.begin()));
  return retVal;
  // end
}
Value *NRetutnStmt::codegen() {
  Function *theFun = builder->GetInsertBlock()->getParent();
  BasicBlock *bb = BasicBlock::Create(*theContext, "ret", theFun);
  builder->CreateBr(bb);
  builder->SetInsertPoint(bb);
  auto *retVal = exp.codegen();
  // check the return type and fundec type
  // begin
  if(retVal->getType()->getPointerTo() != theFun->getReturnType()->getPointerTo()){
    printSemanticError(7,line,"UNMATCH return");
    return nullptr;
  }
  // end>
    builder->CreateRet(retVal);
  return retVal;
}
Value *NIfStmt::codegen() {
  Function *theFun = builder->GetInsertBlock()->getParent();
  // begin
  Value* cmp = exp.codegen();
  Value* retVal = nullptr;
  if(cmp == nullptr){
    return nullptr;
  }
  Value* condVal = builder->CreateICmpNE(cmp,Constant::getNullValue(cmp->getType()),"cond");
  BasicBlock *thenb = BasicBlock::Create(*theContext,"then",theFun);
  BasicBlock *ifcontb = BasicBlock::Create(*theContext,"ifcont");
  builder->CreateCondBr(condVal,thenb,ifcontb);
  builder->SetInsertPoint(thenb);
  retVal = stmt.codegen();
  builder->CreateBr(ifcontb);

  theFun->getBasicBlockList().push_back(ifcontb);
  builder->SetInsertPoint(ifcontb);
  return retVal;
  // end
}
Value *NIfElseStmt::codegen() {
  Function *theFun = builder->GetInsertBlock()->getParent();
  // begin
  Value* cmp = exp.codegen();
  Value* retVal = nullptr;
  if(cmp == nullptr){
    return nullptr;
  }
  Value* condVal = builder->CreateICmpNE(cmp,Constant::getNullValue(cmp->getType()),"cond");
  BasicBlock *thenb = BasicBlock::Create(*theContext,"then",theFun);
  BasicBlock *elseb = BasicBlock::Create(*theContext,"else",theFun);
  BasicBlock *ifcontb = BasicBlock::Create(*theContext,"ifcont");
  builder->CreateCondBr(condVal,thenb,elseb);
  builder->SetInsertPoint(thenb);
  retVal = stmt.codegen();
  builder->CreateBr(ifcontb);

  builder->SetInsertPoint(elseb);
  retVal = stmt_else.codegen();
  builder->CreateBr(ifcontb);

  theFun->getBasicBlockList().push_back(ifcontb);
  builder->SetInsertPoint(ifcontb);
  return retVal;
  // end
}
Value *NWhileStmt::codegen() {
  Function *theFun = builder->GetInsertBlock()->getParent();
  BasicBlock *condb = BasicBlock::Create(*theContext, "cond", theFun);
  // begin
  BasicBlock *thenb = BasicBlock::Create(*theContext,"then",theFun);
  BasicBlock *whilecontb = BasicBlock::Create(*theContext,"whilecont");
  builder->CreateBr(condb);
  builder->SetInsertPoint(condb);
  Value* cmp = exp.codegen();
  Value* retVal = nullptr;
  if(cmp == nullptr){
    return nullptr;
  }
  Value* condVal = builder->CreateICmpNE(cmp,Constant::getNullValue(cmp->getType()),"cond");
  builder->CreateCondBr(condVal,thenb,whilecontb);

  builder->SetInsertPoint(thenb);
  retVal = stmt.codegen();
  builder->CreateBr(condb);

  theFun->getBasicBlockList().push_back(whilecontb);
  builder->SetInsertPoint(whilecontb);
  return nullptr;
  // end
}
Value *NBreakStmt::codegen() {
  // begin

  return nullptr;
  // end
}
Value *NExtDefVarDec::codegen() {
  // begin

  return nullptr;
  // end
}
Value *NExtDefFunDec::codegen() {
  Type *retType = specifier.getType();

  Function *f = fundec->funcodegen(retType);
  if (!f) {
    return nullptr;
  }
  assert(compst != nullptr); // Assert compst is not null.
  BasicBlock *bb = BasicBlock::Create(*theContext, "entry", f);
  builder->SetInsertPoint(bb);
  namedValues.clear();
  for (auto &arg : f->args()) {
    // Create an alloca for this variable.
    AllocaInst *alloca =
        CreateEntryBlockAlloca(f, arg.getName(), arg.getType());

    if (curNamedValues.count(std::string(arg.getName()))>0) {
      printSemanticError(3, line, "Redefined " + arg.getName().str());
      return LogErrorV("Unknown function referenced");
    }
    // Store the initial value into the alloca.
    builder->CreateStore(&arg, alloca);
    // Add arguments to variable symbol table.
    namedValues[std::string(arg.getName())] = alloca;
    //curNamedValues[std::string(arg.getName())] = alloca;
  }
  if (Value *retVal = compst->codegen()) {
    // Finish off the function.

    // Validate the generated code, checking for consistency.
    verifyFunction(*f);

    // Run the optimizer on the function.
    // theFPM->run(*f);
    return f;
  }
  // Error reading body, remove function.
  f->eraseFromParent();

  return nullptr;
}
Value *NExtDefList::codegen() {
  auto *lastCode = nExtDef.codegen();
  // lastCode->print(errs());
  // assert(nExtDefList == nullptr);
  if (nExtDefList)
    lastCode = nExtDefList->codegen();
  return lastCode;
}
Value *NProgram::codegen() {

  //默认输出函数putchar
  std::vector<Type *> putArgs;
  putArgs.push_back(Type::getInt32Ty(*theContext));

  FunctionType *putType =
      FunctionType::get(builder->getInt32Ty(), putArgs, false);
  Function *putFunc = Function::Create(putType, Function::ExternalLinkage,
                                       "putchar", theModule.get());

  //默认输入函数getchar
  std::vector<Type *> getArgs;
  // getArgs.push_back(Type::getInt32Ty(*theContext));

  FunctionType *getType =
      FunctionType::get(builder->getInt32Ty(), getArgs, false);
  Function *getFunc = Function::Create(getType, Function::ExternalLinkage,
                                       "getchar", theModule.get());

  Value *lastCode = nextdeflist->codegen();
  if (grammererror)
    return nullptr;
  return lastCode;
}
