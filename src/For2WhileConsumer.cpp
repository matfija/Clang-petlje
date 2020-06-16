#include "For2WhileConsumer.hpp"
  
// Posetilac koji for pretvara u while
bool For2WhileVisitor::VisitForStmt(ForStmt *s) {
  // Slozeni iskaz sa telom i inkrementacijom
  // ili samo telo ako nema inkrementacije
  Stmt* telo;
  if (s->getInc() != nullptr) {
    telo = CompoundStmt::Create(TheASTContext,
               std::vector<Stmt*>{s->getBody(), s->getInc()},
               SourceLocation(), SourceLocation());
  } else {
    telo = s->getBody();
  }
  
  // While petlja sa novim telom i uslovom
  // ili beskonacna petlja ako nema uslova
  Expr* cond = s->getCond();
  if (cond == nullptr) {
    const auto tip = TheASTContext.IntTy;
    llvm::APInt APValue(TheASTContext.getTypeSize(tip), 1);
    cond = IntegerLiteral::Create(TheASTContext, APValue,
                                  tip, SourceLocation());
  }
  WhileStmt petlja(TheASTContext, nullptr, cond,
                   telo, SourceLocation());
  
  // Slozeni iskaz sa inicijalizacijom i petljom
  // ili samo petlja ako nema inicijalizacije
  Stmt* initpet;
  if (s->getInit() != nullptr) {
    initpet = CompoundStmt::Create(TheASTContext,
                  std::vector<Stmt*>{s->getInit(), &petlja},
                  SourceLocation(), SourceLocation());
  } else {
    initpet = &petlja;
  }
  
  // Tekstualna zamena koda
  zameni(s, initpet);
  
  // Nastavljanje dalje
  return true;
}

// Prekid obilaska kod while petlje
bool For2WhileVisitor::TraverseForStmt(ForStmt* s) {
  return WalkUpFromForStmt(s);
}
