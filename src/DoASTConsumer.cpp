#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <string>
#include <system_error>
#include "DoASTConsumer.hpp"


// Upotreba Clangovog imenskog prostora
using namespace clang;


bool DoASTVisitor::VisitWhileStmt(WhileStmt *s) {
    // Odgovarajuca do-while verzija
    DoStmt petlja(s->getBody(), s->getCond(),
                  SourceLocation(), SourceLocation(), SourceLocation());
    
    // If iskaz za proveru uslova petlje
    IfStmt uslov(TheASTContext, SourceLocation(),
                 false, nullptr, nullptr, s->getCond(), &petlja);
    
    // Tekstualna zamena koda
    zameni(s, &uslov);
    //radi();
    // Nastavljanje dalje
    return true;
}

bool DoASTVisitor::VisitForStmt(ForStmt *s) {
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

  // Prekid obilaska kod while petlje
bool DoASTVisitor::TraverseWhileStmt(WhileStmt* s) {
    return WalkUpFromWhileStmt(s);
}
  
  // Prekid obilaska kod for petlje
bool DoASTVisitor::TraverseForStmt(ForStmt* s) {
    return WalkUpFromForStmt(s);
}

bool DoASTConsumer::HandleTopLevelDecl(DeclGroupRef DR) {
    for (auto &x: DR)
      Visitor.TraverseDecl(x);
    return true;
}