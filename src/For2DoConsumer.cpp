#include "For2DoConsumer.hpp"

// Upotreba Clangovog imenskog prostora
using namespace clang;

bool For2DoVisitor::VisitForStmt(ForStmt *s) {
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
  
  // Do-while petlja sa novim telom i uslovom
  // ili beskonacna petlja ako nema uslova
  Expr* cond = s->getCond();
  if (cond == nullptr) {
    const auto tip = TheASTContext.IntTy;
    llvm::APInt APValue(TheASTContext.getTypeSize(tip), 1);
    cond = IntegerLiteral::Create(TheASTContext, APValue,
                                  tip, SourceLocation());
  }
  DoStmt petlja(telo, cond, SourceLocation(),
                SourceLocation(), SourceLocation());
    
  // If iskaz za proveru uslova petlje
  IfStmt uslov(TheASTContext, SourceLocation(),
               false, nullptr, nullptr, cond, &petlja);
    
  // Slozeni iskaz sa inicijalizacijom i petljom
  // ili samo petlja ako nema inicijalizacije
  Stmt* initpet;
  if (s->getInit() != nullptr) {
    initpet = CompoundStmt::Create(TheASTContext,
                  std::vector<Stmt*>{s->getInit(), &uslov},
                  SourceLocation(), SourceLocation());
  } else {
    initpet = &uslov;
  }
  
  // Tekstualna zamena koda
  zameni(s, initpet);
  
  // Nastavljanje dalje
  return true;
}
  
// Prekid obilaska kod for petlje
bool For2DoVisitor::TraverseForStmt(ForStmt* s) {
  return WalkUpFromForStmt(s);
}

bool For2DoConsumer::HandleTopLevelDecl(DeclGroupRef DR) {
  for (auto &x: DR)
    Visitor.TraverseDecl(x);
  return true;
}
