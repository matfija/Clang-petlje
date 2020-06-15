#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <string>
#include <system_error>
#include "WhileFromForASTConsumer.hpp"



using namespace clang;
  
 // Pretvaranje while petlji u for
 bool WhileFromForASTVisitor::VisitForStmt(ForStmt *s) {
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
    }else{
        cond = s->getCond();
    }
    
    WhileStmt petlja(TheASTContext, s->getConditionVariable(), cond, telo, SourceLocation());
    Stmt* initpet;
    if (s->getInit() != nullptr) {
      initpet = CompoundStmt::Create(TheASTContext,
                    std::vector<Stmt*>{s->getInit(), &petlja},
                    SourceLocation(), SourceLocation());
    }else{
        initpet = &petlja;
    }

    
    zameni(s, initpet);
   return true;
 }
  
 // Prekid obilaska kod while petlje
 bool WhileFromForASTVisitor::TraverseForStmt(ForStmt* s) {
   return WalkUpFromForStmt(s);
 }

bool WhileFromForASTConsumer::HandleTopLevelDecl(DeclGroupRef DR) {
   for (auto &x: DR)
     Visitor.TraverseDecl(x);
   return true;
 }