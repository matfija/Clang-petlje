#include "For2WhileConsumer.hpp"
  
// Pretvaranje while petlji u for
bool For2WhileVisitor::VisitForStmt(ForStmt *s) {
  Stmt* telo;
  if (s->getInc() != nullptr) {
    telo = CompoundStmt::Create(TheASTContext,
               std::vector<Stmt*>{s->getBody(), s->getInc()},
               SourceLocation(), SourceLocation());
  } else {
    telo = s->getBody();
  }
  
  Expr* cond = s->getCond();
  if (cond == nullptr) {
    const auto tip = TheASTContext.IntTy;
    llvm::APInt APValue(TheASTContext.getTypeSize(tip), 1);
    cond = IntegerLiteral::Create(TheASTContext, APValue,
                                  tip, SourceLocation());
  } else {
    cond = s->getCond();
  }
  WhileStmt petlja(TheASTContext, s->getConditionVariable(), cond, telo, SourceLocation());
  
  Stmt* initpet;
  if (s->getInit() != nullptr) {
    initpet = CompoundStmt::Create(TheASTContext,
                  std::vector<Stmt*>{s->getInit(), &petlja},
                  SourceLocation(), SourceLocation());
  } else {
    initpet = &petlja;
  }
  
  zameni(s, initpet);
  return true;
 }

// Prekid obilaska kod while petlje
bool For2WhileVisitor::TraverseForStmt(ForStmt* s) {
  return WalkUpFromForStmt(s);
}

bool For2WhileConsumer::HandleTopLevelDecl(DeclGroupRef DR) {
  for (auto &x: DR)
    Visitor.TraverseDecl(x);
  return true;
}
